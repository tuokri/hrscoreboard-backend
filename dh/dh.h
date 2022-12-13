#pragma once
#ifndef HRSCOREBOARD_BACKEND_DH_H
#define HRSCOREBOARD_BACKEND_DH_H

#include <vector>
#include <cstdint>

namespace dh
{

inline constexpr uint32_t P = 0x71d0d8bf;
inline constexpr uint32_t G = 5;

std::vector<uint32_t>
gen_shared_secret(const std::vector<uint32_t>& public_key, const std::vector<uint32_t>& private_key);

}

#endif // HRSCOREBOARD_BACKEND_DH_H
