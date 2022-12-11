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

#include "xxtea.h"

namespace
{

inline constexpr int32_t DELTA = 0x243f6a88;

inline constexpr int32_t
mx(int32_t z, int32_t y, int32_t sum, int32_t e, int32_t p, const std::array<int32_t, xxtea::KEY_LEN>& key)
{
    return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) + (key.at((p & 3) ^ e) ^ z)));
}

}

namespace xxtea
{

std::vector<int32_t>
decrypt(const std::vector<int32_t>& v, const std::array<int32_t, KEY_LEN>& key)
{
    int32_t y;
    int32_t z;
    int32_t sum;
    int32_t p;
    int32_t e;

    auto n = static_cast<int32_t>(v.size());
    int32_t rounds = 6 + 52 / n;
    std::vector<int32_t> ret{v};

    if (n > 1)
    {
        sum = rounds * DELTA;
        y = ret.at(0);

        while (rounds-- > 0)
        {
            e = (sum >> 2) & 3;

            for (p = n - 1; p > 0; --p)
            {
                z = ret.at(p - 1);
                y = ret.at(p) -= mx(z, y, sum, e, p, key);
            }

            z = ret.at(n - 1);
            y = ret.at(0) -= mx(z, y, sum, e, p, key);
            sum -= DELTA;
        }
    }
    else
    {
        throw std::invalid_argument("v size must be > 1, is " + std::to_string(n));
    }

    return ret;
}

std::vector<int32_t> bytes2ints(const std::vector<uint8_t>& bytes, bool padding)
{
    int32_t i;
    int32_t pad;

    const auto in_size = static_cast<int32_t>(bytes.size());
    std::vector<int32_t> out(in_size / 4);
    // out.reserve(in_size / 4);

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

std::vector<uint8_t> ints2bytes(const std::vector<int32_t>& ints, bool padding)
{
    int32_t i;
    int32_t pad;
    int32_t out_size;

    const auto in_size = static_cast<int32_t>(ints.size());

    out_size = in_size * 4;
    std::vector<uint8_t> out(out_size);
    // out.reserve(out_size);

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

        if (out_size < 0)
        {
            // return -2;
            throw std::invalid_argument("invalid padding: " + std::to_string(-2));
        }

        const auto total_size = in_size * 4;
        for (i = out_size; i < total_size; ++i)
        {
            if (out[i] != pad)
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
