#pragma once
#ifndef HRSCOREBOARD_BACKEND_DH_H
#define HRSCOREBOARD_BACKEND_DH_H

#include <cstdint>

namespace dh
{

inline constexpr uint32_t P = 0x71d0d8bf;
inline constexpr uint32_t G = 7;

void gen_shared_secret();

}

#endif // HRSCOREBOARD_BACKEND_DH_H
