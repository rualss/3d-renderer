#include "picture.h"
#include <cassert>
#include "linalg.h"

namespace renderer {

Picture::Picture(Height height, Width width) {
    assert(static_cast<Index>(height) > 0 && "Height less or equal to 0");
    assert(static_cast<Index>(width) > 0 && "Width less or equal to 0");
    height_ = static_cast<Index>(height);
    width_ = static_cast<Index>(width);
    pixels_.resize(width_ * height_, kBlack);
    z_buffer_.resize(width_ * height_, 2);
}

void Picture::SetPixel(Index x, Index y, const Color& color) {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    assert(color[0] >= 0 && color[0] <= 255 && color[0] >= 0 && color[0] <= 255 && color[0] >= 0 &&
           color[0] <= 255 && "Color value out of bounds");

    pixels_[width_ * y + x] = color;
}

const Color& Picture::GetPixel(Index x, Index y) const {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    return pixels_[width_ * y + x];
}

const std::vector<Color>& Picture::GetPixels() const {
    return pixels_;
}
void Picture::SetZBufferValue(Index x, Index y, CoordType z) {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    z_buffer_[width_ * y + x] = z;
}
CoordType Picture::GetZBufferValue(Index x, Index y) const {
    assert(x >= 0 && x < width_ && "x coordinates out of bounds");
    assert(y >= 0 && y < height_ && "y coordinates out of bounds");
    return z_buffer_[width_ * y + x];
}

Index Picture::GetHeight() const {
    return height_;
}

Index Picture::GetWidth() const {
    return width_;
}

}  // namespace renderer
