#pragma once

#include "linalg.h"
#include <array>
#include <optional>

namespace renderer {

struct Polygon {
public:
    static constexpr Index kVertexCount = 3;

    std::array<Vec3, kVertexCount> vertices;
    std::array<Vec3, kVertexCount> normals;
    std::optional<std::array<Vec2, kVertexCount>> texture_vertices = std::nullopt;
};

void TransformPolygon(const Mat4& mat, Polygon& polygon);
void TransformNormals(const Mat4& mat, Polygon& polygon);
Vec3 GetNonUnitNormal(const Polygon& polygon);
void ProjectiveTransformVector(const Mat4& transformation_matrix, Vec3& vector);
void ProjectiveTransformPolygon(const Mat4& transformation_matrix, Polygon& polygon);

}  // namespace renderer
