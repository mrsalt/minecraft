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

    bool operator<(const Color& rhs) const
    {
        return intVal() < rhs.intVal();
    }

    int intVal() const
    {
        return R + (G << 8) + (B << 16);
    }

};
