#include "mesh.h"
#include "linalg.h"
#include "obj_parser.h"
#include "polygon.h"
#include <utility>
#include <random>

namespace renderer {
Mesh::Mesh() = default;

Mesh::Mesh(const std::string& path) {
    OBJParser parser(path);
    for (const auto& face : parser.GetFaces()) {
        for (Index i = 1; i + 1 < face.size(); ++i) {
            AddPolygon(Polygon(face[0], face[i], face[i + 1]));
        }
    }
}

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
    std::swap(local_origin_, other.local_origin_);
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

void Mesh::SetColor(const Color& color) {
    for (Polygon& polygon : polygons_) {
        polygon.SetColor(color);
    }
}

void Mesh::ApplyMatrix(const Mat4& mat) {
    for (Polygon& polygon : polygons_) {
        polygon.ApplyMatrix(mat);
    }
}

const Vec3& Mesh::GetLocalOrigin() const {
    return local_origin_;
}

void Mesh::SetLocalOrigin(const Vec3& new_origin) {
    local_origin_ = new_origin;
}

void Mesh::SetColorsRandomly() {
    std::random_device rd;
    std::mt19937 rng(rd());
    for (Polygon& polygon : polygons_) {
        polygon.SetColor(
            {rng() % (kColorMax + 1), rng() % (kColorMax + 1), rng() % (kColorMax + 1)});
    }
}

}  // namespace renderer
