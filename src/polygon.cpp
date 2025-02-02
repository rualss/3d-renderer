#include "polygon.h"

namespace renderer {

std::array<Vec3, Polygon::kVertexCount> Polygon::GetVertices() const {
    return vertices_;
}

}  // namespace renderer
