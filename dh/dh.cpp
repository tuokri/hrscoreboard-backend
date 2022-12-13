#include <stdexcept>
#include <iostream>

#include <boost/range/combine.hpp>

#include <botan/bigint.h>
#include <botan/numthry.h>

#include "dh.h"

namespace dh
{

std::vector<uint32_t>
gen_shared_secret(const std::vector<uint32_t>& public_key, const std::vector<uint32_t>& private_key)
{
    if (public_key.size() != private_key.size())
    {
        throw std::invalid_argument("key sizes do not match");
    }

    std::vector<uint32_t> shared_secret(public_key.size());

    std::cout << "shared (Botan):\n";
    for (const auto& tup: boost::combine(public_key, private_key))
    {
        uint32_t pubk;
        uint32_t privk;
        boost::tie(pubk, privk) = tup;

        Botan::BigInt pubk_big(pubk);
        Botan::BigInt privk_big(privk);

        std::cout << Botan::power_mod(pubk_big, privk_big, P) << std::endl;

        shared_secret.emplace_back(Botan::power_mod(pubk_big, privk_big, P).to_u32bit());
    }

    return shared_secret;
}

}
