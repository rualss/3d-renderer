#include "picture.h"
#include "color.h"
#include "linalg.h"
#include <cassert>
#include <execution>
#include <algorithm>
#include <iostream>

namespace renderer {

Picture::Picture() : pixels_(kDefaultPixelsSize, kDefaultColor) {
}

Picture::Picture(Height height, Width width) : width_(width) {
    assert(height > 0 && "Height must be positive");
    assert(width > 0 && "Width must be positive");
    pixels_.resize(static_cast<Index>(width) * height, kDefaultColor);
}

Picture::Picture(Width width, std::vector<DiscreteColor>&& pixels)
    : width_(width), pixels_(std::move(pixels)) {
    assert(pixels.size() % width == 0 && "Width must divide number of pixels");
}

DiscreteColor& Picture::operator()(Index x, Index y) {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < GetHeight() && "y coordinates out of bounds");
    return pixels_[width_ * y + x];
}

const DiscreteColor& Picture::operator()(Index x, Index y) const {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < GetHeight() && "y coordinates out of bounds");
    return pixels_[width_ * y + x];
}

const std::vector<DiscreteColor>& Picture::GetPixels() const {
    return pixels_;
}

Index Picture::GetHeight() const {
    return pixels_.size() / width_;
}

Index Picture::GetWidth() const {
    return width_;
}

void Picture::SetDefaultColor() {
    std::fill(std::execution::par, pixels_.begin(), pixels_.end(), kDefaultColor);
}

const DiscreteColor& Picture::SampleColor(Vec2 coords) const {
    assert(0 <= coords.x <= 1 && 0 <= coords.y <= 1 && "Coords must be in [0, 1]");
    return (*this)(static_cast<Index>(std::round(coords.x * (GetWidth() - 1))),
                   static_cast<Index>(std::round(coords.y * (GetHeight() - 1))));
}

}  // namespace renderer
