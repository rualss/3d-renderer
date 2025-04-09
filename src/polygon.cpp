#include "polygon.h"

namespace renderer {

Polygon::Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3) : vertices_({v1, v2, v3}) {
}

Polygon::Polygon(const std::array<Vec3, kVertexCount>& vertices) : vertices_(vertices) {
}

Polygon::Polygon(const Polygon& other) : color_(other.color_), vertices_(other.vertices_) {
}

Polygon::Polygon(Polygon&& other)
    : color_(std::move(other.color_)), vertices_(std::move(other.vertices_)) {
}

Polygon& Polygon::operator=(const Polygon& other) {
    Polygon tmp(other);
    Swap(tmp);
    return *this;
}

Polygon& Polygon::operator=(Polygon&& other) {
    Swap(other);
    return *this;
}

Polygon::~Polygon() = default;

void Polygon::Swap(Polygon& other) {
    std::swap(color_, other.color_);
    vertices_.swap(other.vertices_);
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

void Polygon::ApplyMatrix(const Mat4& mat) {
    for (int i = 0; i < kVertexCount; ++i) {
        Vec4 tmp(vertices_[i], 1.);
        vertices_[i] = Vec3(mat * tmp);
    }
}

}  // namespace renderer
