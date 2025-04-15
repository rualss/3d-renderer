#include "mesh.h"
#include "linalg.h"
#include "material.h"
#include "polygon.h"
#include <utility>

namespace renderer {

const std::vector<Polygon>& Mesh::GetPolygons() const {
    return polygons_;
}

void Mesh::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

void Mesh::AddPolygon(Polygon&& polygon) {
    polygons_.push_back(std::move(polygon));
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
