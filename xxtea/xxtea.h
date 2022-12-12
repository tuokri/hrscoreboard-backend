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

inline static constexpr uint32_t KEY_LEN = 4;

/**
 * XXTEA decryption.
 *
 * @param v input ciphertext with PKCS#7 padding
 * @param key 16-byte XXTEA key
 * @return @v decrypted and padding removed
 */
std::vector<uint32_t>
decrypt(const std::vector<uint32_t>& v, const std::array<uint32_t, KEY_LEN>& key);

std::vector<uint32_t> bytes2ints(const std::vector<uint8_t>& bytes, bool padding);

std::vector<uint8_t> ints2bytes(const std::vector<uint32_t>& ints, bool padding);

}

#endif // HRSCOREBOARD_BACKEND_XXTEA_H
