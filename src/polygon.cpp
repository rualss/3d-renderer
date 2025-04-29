#include "polygon.h"
#include "glm/geometric.hpp"

namespace renderer {

void TransformPolygon(const Mat4& mat, Polygon& polygon) {
    for (int i = 0; i < Polygon::kVertexCount; ++i) {
        Vec4 tmp_vertex(polygon.vertices[i], 1.);
        polygon.vertices[i] = Vec3(mat * tmp_vertex);
    }
}

void TransformNormals(const Mat4& mat, Polygon& polygon) {
    for (int i = 0; i < Polygon::kVertexCount; ++i) {
        Vec4 tmp_normal(polygon.normals[i], 1.);
        polygon.normals[i] = glm::normalize(Vec3(mat * tmp_normal));
    }
}

Vec3 GetNonUnitNormal(const Polygon& polygon) {
    return glm::cross(polygon.vertices[2] - polygon.vertices[0],
                      polygon.vertices[1] - polygon.vertices[0]);
}

void ProjectiveTransformVector(const Mat4& transformation_matrix, Vec3& vector) {
    Vec4 homogeneous(vector, 1.0);
    homogeneous = transformation_matrix * homogeneous;
    assert(std::abs(homogeneous.w) > kEps && "TransformVector: Point went to infinity");
    vector = Vec3(homogeneous / homogeneous.w);
}

void ProjectiveTransformPolygon(const Mat4& transformation_matrix, Polygon& polygon) {
    for (size_t i = 0; i < Polygon::kVertexCount; ++i) {
        ProjectiveTransformVector(transformation_matrix, polygon.vertices[i]);
    }
}

}  // namespace renderer
