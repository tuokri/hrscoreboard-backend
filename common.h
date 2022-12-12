#pragma once
#ifndef HRSCOREBOARD_BACKEND_COMMON_H
#define HRSCOREBOARD_BACKEND_COMMON_H

#include <array>
#include <cstdint>

#include "xxtea.h"

// TODO: Config class to set these dynamically.
namespace hr
{

inline constexpr uint16_t PORT = 54231;

inline constexpr std::array<uint32_t, xxtea::KEY_LEN> XXTEA_KEY = {0x2b959f13, 0x330de56a, 0x583e0f76, 0x6b8f3054};

}

#endif // HRSCOREBOARD_BACKEND_COMMON_H
