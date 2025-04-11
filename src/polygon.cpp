#include "polygon.h"
#include <glm/ext/quaternion_geometric.hpp>

namespace renderer {

Polygon::Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3, Color color)
    : vertices_({v1, v2, v3}), color_(color) {
}

Polygon::Polygon(const std::array<Vec3, kVertexCount>& vertices, Color color)
    : vertices_(vertices), color_(color) {
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

Vec3& Polygon::operator[](Index i) {
    return vertices_[i];
}

const Vec3& Polygon::operator[](Index i) const {
    return vertices_[i];
}

Vec3 Polygon::GetUnitNormal() const {
    return glm::normalize(GetNonUnitNormal());
}

Vec3 Polygon::GetNonUnitNormal() const {
    return glm::cross(vertices_[2] - vertices_[0], vertices_[1] - vertices_[0]);
}

}  // namespace renderer
