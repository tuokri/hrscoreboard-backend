/*
 * XXTEA adapted for 32-bit signed integers for Unreal Engine 3.
 * Based on https://github.com/ifduyue/xxtea by Yue Du.
 *
 * Copyright (c) 2022, tuokri / fluudah
 * Copyright (c) 2014-2020, Yue Du
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdexcept>
#include <iostream>

#include "xxtea.h"

namespace
{

inline constexpr uint32_t DELTA = 0x9e3779b9;

inline constexpr uint32_t
mx(uint32_t z, uint32_t y, uint32_t sum, uint32_t e, uint32_t p, const std::array<uint32_t, xxtea::KEY_LEN>& key)
{
    return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) + (key.at((p & 3) ^ e) ^ z)));
}

//inline uint32_t
//mx(uint32_t z, uint32_t y, uint32_t sum, uint32_t e, uint32_t p, const std::array<uint32_t, xxtea::KEY_LEN>& key)
//{
//    const auto a = (z >> 5 ^ y << 2);
//    std::cout << "a :" << std::hex << a << std::endl;
//    const auto b = (y >> 3 ^ z << 4);
//    std::cout << "b :" << std::hex << b << std::endl;
//    const auto c = a + b;
//    std::cout << "c :" << std::hex << c << std::endl;
//
//    const auto d = (sum ^ y);
//    std::cout << "d :" << std::hex << d << std::endl;
//    const auto ee = (key.at((p & 3) ^ e) ^ z);
//    std::cout << "ee:" << std::hex << ee << std::endl;
//    const auto f = d + ee;
//    std::cout << "f :" << std::hex << f << std::endl;
//
//    const auto g = c ^ f;
//    std::cout << "g :" << std::hex << g << std::endl;
//
//    return g;
//}

// #define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

}

namespace xxtea
{

std::vector<uint32_t>
decrypt(const std::vector<uint32_t>& v, const std::array<uint32_t, KEY_LEN>& key)
{
    uint32_t y;
    uint32_t z;
    uint32_t sum;
    uint32_t p;
    uint32_t e;

    const auto n = v.size();
    uint32_t rounds = 6 + 52 / n;
    std::vector<uint32_t> ret{v};

    if (n > 1)
    {
        sum = rounds * DELTA;
        y = ret.at(0);

        while (rounds-- > 0)
        {
            e = (sum >> 2) & 3;

//            std::cout << "rounds : " << std::dec << rounds << std::endl;
//            std::cout << "sum    : " << std::hex << sum << std::endl;
//            std::cout << "e      : " << std::hex << e << std::endl;

            for (p = n - 1; p > 0; --p)
            {
                z = ret.at(p - 1);
                ret.at(p) -= mx(z, y, sum, e, p, key);
                y = ret.at(p);

//                std::cout << "p         : " << std::dec << p << std::endl;
//                std::cout << "y         : " << std::hex << y << " " << static_cast<int32_t>(y) << std::endl;
//                std::cout << "z         : " << std::hex << z << std::endl;
//                std::cout << "ret.at(p) : " << std::hex << ret.at(p) << std::endl;
            }

            z = ret.at(n - 1);
            ret.at(0) -= mx(z, y, sum, e, p, key);
            y = ret.at(0);
            sum -= DELTA;

//            std::cout << "ret.at(n - 1) : " << std::hex << ret.at(n - 1) << std::endl;
//            std::cout << "y             : " << std::hex << y << std::endl;
//            std::cout << "z             : " << std::hex << z << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("v size must be > 1, is " + std::to_string(n));
    }

    return ret;
}

std::vector<uint32_t> bytes2ints(const std::vector<uint8_t>& bytes, bool padding)
{
    uint32_t i;
    uint32_t pad;

    const auto in_size = bytes.size();
    std::vector<uint32_t> out(in_size / 4);

    // (i & 3) << 3 -> [0, 8, 16, 24]
    for (i = 0; i < in_size; i++)
    {
        out.at(i >> 2) |= bytes.at(i) << ((i & 3) << 3);
    }

    if (padding)
    {
        // PKCS#7 padding.
        pad = 4 - (in_size & 3);
        // Make sure length of out >= 2.
        pad = (in_size < 4) ? pad + 4 : pad;
        for (i = in_size; i < in_size + pad; ++i)
        {
            out.at(i >> 2) |= (pad << ((i & 3) << 3));
        }
    }

    // Divided by 4, and then rounded up (ceil) to an integer.
    // Which is the number of how many longs we've got.
    // return ((i - 1) >> 2) + 1;
    return out;
}

std::vector<uint8_t> ints2bytes(const std::vector<uint32_t>& ints, bool padding)
{
    uint32_t i;
    uint32_t pad;
    uint32_t out_size;

    const auto in_size = ints.size();
    const auto total_size = in_size * 4;

    out_size = in_size * 4;
    std::vector<uint8_t> out(out_size);

    for (i = 0; i < in_size; i++)
    {
        out.at(4 * i) = ints.at(i) & 0xFF;
        out.at(4 * i + 1) = (ints.at(i) >> 8) & 0xFF;
        out.at(4 * i + 2) = (ints.at(i) >> 16) & 0xFF;
        out.at(4 * i + 3) = (ints.at(i) >> 24) & 0xFF;
    }

    // Remove PKCS#7 padding.
    if (padding)
    {
        pad = out.at(out_size - 1);
        out_size -= pad;

        if (pad < 1 || pad > 8)
        {
            /* invalid padding */
            // return -1;
            throw std::invalid_argument("invalid padding: " + std::to_string(-1));
        }

        if (out_size >= total_size)
        {
            // return -2;
            throw std::invalid_argument("invalid padding: " + std::to_string(-2));
        }

        for (i = out_size; i < total_size; ++i)
        {
            if (out.at(i) != pad)
            {
                // return -3;
                throw std::invalid_argument("invalid padding: " + std::to_string(-3));
            }
        }
    }

    // s[outlen] = '\0';

    /* How many bytes we've got */
    // return outlen;

    out.resize(out_size);
    return out;
}

}
