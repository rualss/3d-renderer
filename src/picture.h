#pragma once

#include <vector>
#include "linalg.h"

namespace renderer {

class Picture {
public:
    Picture(Height height, Width width);
    void SetPixel(Index x, Index y, const Color& color);
    const Color& GetPixel(Index x, Index y) const;
    const std::vector<Color>& GetPixels() const;
    void SetZBufferValue(Index x, Index y, CoordType z);
    CoordType GetZBufferValue(Index x, Index y) const;
    Index GetHeight() const;
    Index GetWidth() const;
    void Reset();

private:
    Index height_;
    Index width_;
    std::vector<Color> pixels_;  // X axis directed right Y axis directed downwards
    std::vector<CoordType> z_buffer_;
};

}  // namespace renderer
