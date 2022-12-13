#include <iostream>
#include <cstdint>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/stacktrace.hpp>

#include "dh.h"
#include "common.h"
#include "xxtea.h"

boost::asio::awaitable<void> echo_once(boost::asio::ip::tcp::socket& socket)
{
    std::array<uint8_t, 3> header_data{};
    std::array<uint8_t, 248> payload_data{};
    std::size_t n;

    std::cout << "reading header" << std::endl;
    n = co_await socket.async_read_some(boost::asio::buffer(header_data, 3), boost::asio::use_awaitable);
    std::cout << "got " << std::dec << n << " bytes" << std::endl;
    const auto packet_size = header_data.at(0);
    std::cout << "size             : " << std::dec << +packet_size << " bytes" << std::endl;
    std::cout << "protocol version : " << +header_data.at(1) << std::endl;
    std::cout << "packet ID        : " << +header_data.at(2) << std::endl;

    std::cout << "reading payload" << std::endl;
    n = co_await socket.async_read_some(boost::asio::buffer(payload_data, packet_size - 3),
                                        boost::asio::use_awaitable);
    std::cout << "got " << n << " payload bytes" << std::endl;

    std::cout << "making payload vector" << std::endl;
    const auto payload_vector = std::vector<uint8_t>{payload_data.cbegin(), payload_data.cbegin() + n};

    for (const auto& p: payload_vector)
    {
        std::cout << +p << std::endl;
    }

    std::cout << "making ints" << std::endl;
    const auto ints = xxtea::bytes2ints(payload_vector, false);

    std::cout << "making plain" << std::endl;
    const auto plain = xxtea::decrypt(ints, hr::XXTEA_KEY);
    std::cout << "making plain bytes" << std::endl;
    const auto plain_bytes = xxtea::ints2bytes(plain, true);

    for (const auto& p: plain_bytes)
    {
        std::cout << std::hex << +p << std::endl;
    }

    std::vector<uint8_t> echo_data(3 + n);
    echo_data.insert(echo_data.begin(), header_data.cbegin(), header_data.cend());
    echo_data.insert(echo_data.begin() + 3, plain_bytes.cbegin(), plain_bytes.cend());
    co_await socket.async_write_some(boost::asio::buffer(echo_data, 3 + n), boost::asio::use_awaitable);
}

boost::asio::awaitable<void> echo(boost::asio::ip::tcp::socket socket)
{
    try
    {
        // The asynchronous operations to echo a single chunk of data have been
        // refactored into a separate function. When this function is called, the
        // operations are still performed in the context of the current
        // coroutine, and the behaviour is functionally equivalent.
        co_await echo_once(socket);
        std::cout << socket.remote_endpoint() << " finished" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << boost::stacktrace::stacktrace() << std::endl;
        std::cout << "echo error: " << e.what() << std::endl;
        throw e;
    }
}

boost::asio::awaitable<void> listener()
{
    auto executor = co_await boost::asio::this_coro::executor;
    boost::asio::ip::tcp::acceptor acceptor(executor, {boost::asio::ip::tcp::v4(), hr::PORT});
    for (;;)
    {
        boost::asio::ip::tcp::socket socket = co_await acceptor.async_accept(boost::asio::use_awaitable);
        std::cout << "accepted " << socket.remote_endpoint() << std::endl;
        co_spawn(executor, echo(std::move(socket)), boost::asio::detached);
    }
}

int main()
{
    std::cout << "Hello, World!" << std::endl;

    try
    {
        const std::vector<uint32_t> test{0x94755ABD, 0x99170141, 0xF7CA9CCE, 0x0B3E5009};
        std::vector<uint32_t> test_u322;
        std::vector<int32_t> test_i32{-1016684621, 2124612455, -1604102981, -1110030535};
        std::copy(test.cbegin(), test.cend(), std::back_inserter(test_u322));
        std::vector<uint32_t> test_u32{0xA67E1350, 0x15607BFA, 0xF8ADE187, 0xCC3113AC};
        const auto plain1 = xxtea::decrypt(test_u32, hr::XXTEA_KEY);
        const auto plain2 = xxtea::decrypt(test_u322, hr::XXTEA_KEY);

        std::vector<uint8_t> b{8, 157, 240, 85, 218, 166, 172, 43, 123, 20, 158, 147, 0, 83, 42, 241};
        auto ints = xxtea::bytes2ints(b, false);
        for (const auto& i: ints)
        {
            std::cout << std::hex << i << std::endl;
        }
        std::cout << std::endl;

        const auto plain_b = xxtea::decrypt(ints, hr::XXTEA_KEY);
        for (const auto& pb: plain_b)
        {
            std::cout << std::hex << pb << std::endl;
        }
        std::cout << "\n";

        for (const auto& p: plain1)
        {
            std::cout << std::hex << p << std::endl;
        }
        std::cout << "\n";

        const auto shared = dh::gen_shared_secret({0x94755ABD, 0x99170141, 0xF7CA9CCE, 0x0B3E5009},
                                                  {0xA67E1350, 0x15607BFA, 0xF8ADE187, 0xCC3113AC});
        std::cout << "shared:\n";
        for (const auto& s: shared)
        {
            std::cout << std::hex << s << std::endl;
        }
        std::cout << "\n";


        boost::asio::io_context io_context;

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto)
                           { io_context.stop(); });

        boost::asio::co_spawn(io_context, listener(), boost::asio::detached);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "unhandled exception in main" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
