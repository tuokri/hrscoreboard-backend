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
