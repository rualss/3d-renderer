#pragma once

#include <vector>
#include "linalg.h"
#include "picture.h"

namespace renderer {

class ZBuffer {
public:
    ZBuffer() = default;
    void SetDefaultValue(const Picture& picture);
    CoordType operator()(Index x, Index y) const;
    CoordType& operator()(Index x, Index y);

private:
    Index width_ = 0;
    std::vector<CoordType> z_buffer_;
};

}  // namespace renderer
