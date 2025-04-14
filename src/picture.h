#pragma once

#include <vector>
#include "linalg.h"

namespace renderer {

class Picture {
public:
    Picture(Height height, Width width);
    Picture(Height height, Width width, unsigned char* data);

    Color& operator()(Index x, Index y);
    const Color& operator()(Index x, Index y) const;
    const std::vector<Color>& GetPixels() const;
    Index GetHeight() const;
    Index GetWidth() const;
    void Reset();

private:
    Index height_;
    Index width_;
    std::vector<Color> pixels_;  // X axis directed right Y axis directed downwards
};

}  // namespace renderer
