#include "picture.h"
#include "linalg.h"
#include <cassert>
#include <execution>
#include <algorithm>

namespace renderer {

Picture::Picture(Height height, Width width)
    : height_(static_cast<Index>(height)), width_(static_cast<Index>(width)) {
    assert(height_ > 0 && "Height must be positive");
    assert(width_ > 0 && "Width must be positive");
    pixels_.resize(width_ * height_, kBlack);
}

Picture::Picture(Height height, Width width, unsigned char* data)
    : height_(static_cast<Index>(height)), width_(static_cast<Index>(width)) {
    assert(height_ > 0 && "Height must be positive");
    assert(width_ > 0 && "Width must be positive");
    for (Index y = 0; y < height_; ++y) {
        for (Index x = 0; x < width_; ++x) {
            Index index = (y * width_ + x) * 3;
            pixels_.emplace_back(data[index], data[index + 1], data[index + 2]);
        }
    }
}

Color& Picture::operator()(Index x, Index y) {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    return pixels_[width_ * y + x];
}

const Color& Picture::operator()(Index x, Index y) const {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    return pixels_[width_ * y + x];
}

const std::vector<Color>& Picture::GetPixels() const {
    return pixels_;
}

Index Picture::GetHeight() const {
    return height_;
}

Index Picture::GetWidth() const {
    return width_;
}

void Picture::Reset() {
    std::fill(std::execution::par, pixels_.begin(), pixels_.end(), kBlack);
}

}  // namespace renderer
