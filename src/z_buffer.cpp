#include "z_buffer.h"
#include <algorithm>
#include <cassert>
#include <execution>
#include "linalg.h"

namespace renderer {

void ZBuffer::SetDefaultValue(const Picture& picture) {
    if (z_buffer_.size() < picture.GetPixels().size()) {
        z_buffer_.resize(picture.GetPixels().size());
    }
    width_ = picture.GetWidth();
    std::fill(std::execution::par, z_buffer_.begin(), z_buffer_.end(), 2.);
}

CoordType ZBuffer::operator()(Index x, Index y) const {
    Index index_in_buffer = y * width_ + x;
    assert(0 <= index_in_buffer && index_in_buffer < z_buffer_.size() &&
           "Position must be in buffer");
    return z_buffer_[index_in_buffer];
}

CoordType& ZBuffer::operator()(Index x, Index y) {
    Index index_in_buffer = y * width_ + x;
    assert(0 <= index_in_buffer && index_in_buffer < z_buffer_.size() &&
           "Position must be in buffer");
    return z_buffer_[index_in_buffer];
}

}  // namespace renderer
