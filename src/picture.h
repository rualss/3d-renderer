#pragma once

#include <vector>
#include "linalg.h"

namespace renderer {

class Picture {
public:
    Picture(Height height, Width width);
    Picture(Height height, Width width, const unsigned char* data);

    DiscreteColor& operator()(Index x, Index y);
    const DiscreteColor& operator()(Index x, Index y) const;
    const std::vector<DiscreteColor>& GetPixels() const;
    Index GetHeight() const;
    Index GetWidth() const;
    void Reset();

private:
    Index height_;
    Index width_;
    std::vector<DiscreteColor> pixels_;  // X axis directed right Y axis directed downwards
};

}  // namespace renderer
