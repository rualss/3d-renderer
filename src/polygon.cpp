#include "polygon.h"
#include "linalg.h"

namespace renderer {

Polygon::Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3) : vertices_({v1, v2, v3}) {
}

Polygon::Polygon(const std::array<Vec3, kVertexCount>& vertices) : vertices_(vertices) {
}

const std::array<Vec3, Polygon::kVertexCount>& Polygon::GetVertices() const {
    return vertices_;
}

std::array<Vec3, Polygon::kVertexCount>& Polygon::GetVertices() {
    return vertices_;
}

void Polygon::SetColor(const Color& color) {
    color_ = color;
}

Color Polygon::GetColor() const {
    return color_;
}

}  // namespace renderer
