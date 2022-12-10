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

#pragma once
#ifndef HRSCOREBOARD_BACKEND_XXTEA_H
#define HRSCOREBOARD_BACKEND_XXTEA_H

#include <array>
#include <cstdint>
#include <vector>

namespace xxtea
{
static constexpr int32_t KEY_LEN = 4;

/**
 * XXTEA decryption specialized for signed 32-bit integers.
 * Common XXTEA implementations operate on 32-bit unsigned integers,
 * whereas this version operates on signed integers due to Unreal Engine 3 /
 * UnrealScript integers being exclusively limited to signed type.
 *
 * @param v input ciphertext with PKCS#7 padding
 * @param key 16-byte XXTEA key
 * @return @v decrypted and padding removed
 */
inline static std::vector<int32_t>
decrypt(const std::vector<int32_t>& v, const std::array<int32_t, KEY_LEN>& key);

//inline static int bytes2longs(const char* in, int inlen, unsigned int* out, int padding);
//
//inline static int longs2bytes(unsigned int* in, int inlen, char* out, int padding);
}

#endif // HRSCOREBOARD_BACKEND_XXTEA_H
