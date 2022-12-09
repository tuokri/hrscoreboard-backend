#pragma once
#ifndef HRSCOREBOARD_BACKEND_XXTEA_H
#define HRSCOREBOARD_BACKEND_XXTEA_H

#include <array>
#include <cstdint>
#include <vector>

namespace xxtea
{
static constexpr int32_t KEY_LEN = 4;

inline static std::vector<int32_t>
decrypt(const std::vector<int32_t>& v, int32_t n, const std::array<int32_t, KEY_LEN>& key);

//inline static int bytes2longs(const char* in, int inlen, unsigned int* out, int padding);
//
//inline static int longs2bytes(unsigned int* in, int inlen, char* out, int padding);
}

#endif // HRSCOREBOARD_BACKEND_XXTEA_H
