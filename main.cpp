#include <iostream>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "xxtea.h"

using boost::asio::ip::tcp;

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
//        message_ = "";
//
//        boost::asio::async_write(socket_, boost::asio::buffer(message_),
//                                 boost::bind(&tcp_connection::handle_write, shared_from_this(),
//                                             boost::asio::placeholders::error,
//                                             boost::asio::placeholders::bytes_transferred));
    }

private:
    explicit tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context)
    {
    }

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/)
    {
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    explicit tcp_server(boost::asio::io_context& io_context)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), 54231))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
            tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(const tcp_connection::pointer& new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

int main()
{
    std::cout << "Hello, World!" << std::endl;

    try
    {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
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
