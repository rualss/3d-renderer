#include "mesh.h"
#include "linalg.h"
#include "material.h"
#include "polygon.h"
#include <utility>

namespace renderer {
Mesh::Mesh() = default;

Mesh::Mesh(const Mesh& other) = default;

Mesh::Mesh(Mesh&& other) noexcept = default;

Mesh& Mesh::operator=(const Mesh& other) {
    Mesh tmp(other);
    Swap(tmp);
    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) {
    Swap(other);
    return *this;
}

Mesh::~Mesh() = default;

void Mesh::Swap(Mesh& other) {
    polygons_.swap(other.polygons_);
}

const std::vector<Polygon>& Mesh::GetPolygons() const {
    return polygons_;
}

void Mesh::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

void Mesh::AddPolygon(Polygon&& polygon) {
    polygons_.emplace_back(std::move(polygon));
}

void Mesh::ApplyMatrix(const Mat4& mat) {
    for (Polygon& polygon : polygons_) {
        TransformPolygon(mat, polygon);
    }
}

void Mesh::SetMaterial(const Material& material) {
    material_ = material;
}

void Mesh::SetMaterial(Material&& material) {
    material_ = std::move(material);
}

const Material& Mesh::GetMaterial() const {
    return material_;
}

}  // namespace renderer
