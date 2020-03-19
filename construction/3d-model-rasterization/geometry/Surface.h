#pragma once
#include "Color.h"

class SurfaceInfo
{
public:
    virtual Color getRGB() const = 0;
};

class SolidColorSurface : public SurfaceInfo, public Color
{
public:
    SolidColorSurface(Color c)
        : Color{c}
    {
    }

    virtual Color getRGB() const override { return *this; }
};
