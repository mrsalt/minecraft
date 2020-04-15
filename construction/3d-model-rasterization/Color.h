#pragma once
#include <cstdint>

struct Color
{
    uint8_t R{0};
    uint8_t G{0};
    uint8_t B{0};

    bool operator==(const Color &rhs) const
    {
        return R == rhs.R &&
               G == rhs.G &&
               B == rhs.B;
    }
};
