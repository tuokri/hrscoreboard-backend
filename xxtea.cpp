/*
 * XXTEA adapted for 32-bit signed integers for Unreal Engine 3.
 * Based on https://github.com/ifduyue/xxtea by Yue Du.
 *
 * Copyright (c) 2022, tuokri / fluudah
 * Copyright (c) 2014-2020, Yue Du
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "xxtea.h"

namespace
{

constexpr int32_t DELTA = 0x243f6a88;

inline int32_t
mx(int32_t z, int32_t y, int32_t sum, int32_t e, int32_t p, const std::array<int32_t, xxtea::KEY_LEN>& key)
{
    return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)));
}

}

namespace xxtea
{

inline static std::vector<int32_t>
decrypt(const std::vector<int32_t>& v, const std::array<int32_t, KEY_LEN>& key)
{
    int32_t y = 0;
    int32_t z = 0;
    int32_t sum = 0;
    int32_t p = 0;
    int32_t e = 0;

    auto n = static_cast<int32_t>(v.size());
    int32_t rounds = 6 + 52 / n;
    std::vector<int32_t> ret{v};

    if (n > 1)
    {
        sum = rounds * DELTA;
        y = ret[0];

        while (rounds-- > 0)
        {
            e = (sum >> 2) & 3;

            for (p = n - 1; p > 0; p--)
            {
                z = ret[p - 1];
                y = ret[p] -= mx(z, y, sum, e, p, key);
            }

            z = ret[n - 1];
            y = ret[0] -= mx(z, y, sum, e, p, key);
            sum -= DELTA;
        }
    }

    return ret;
}

}
