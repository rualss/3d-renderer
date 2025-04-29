#pragma once

#include <vector>
#include "linalg.h"
#include "alias.h"
#include "color.h"

namespace renderer {

class Picture {
public:
    Picture();
    Picture(Height height, Width width);
    Picture(Width width, std::vector<DiscreteColor>&& pixels);

    DiscreteColor& operator()(Index x, Index y);
    const DiscreteColor& operator()(Index x, Index y) const;
    const std::vector<DiscreteColor>& GetPixels() const;
    Index GetHeight() const;
    Index GetWidth() const;
    void SetDefaultColor();
    const DiscreteColor& SampleColor(Vec2 coords) const;

private:
    static constexpr Width kDefaultWidth = Width{1280};
    static constexpr Index kDefaultPixelsSize = 1280 * 720;
    static constexpr DiscreteColor kDefaultColor = kBlack;

    Index width_ = kDefaultWidth;
    std::vector<DiscreteColor> pixels_;  // X axis directed right Y axis directed downwards
};

}  // namespace renderer
