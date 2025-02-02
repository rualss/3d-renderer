#include "renderer.h"
#include "picture.h"
#include "polygon.h"
#include "world.h"
#include "camera.h"

#include "glm/ext.hpp"

// I plan to rewrite all of this using Eigen
// Because glm::perspective and glm::translate feels like cheating to me

namespace renderer {

Vec3 TransformVector(const Mat4& transformation_matrix, const Vec3& vector) {
    Vec4 homogeneous(vector, 1.0);
    homogeneous = transformation_matrix * homogeneous;
    return Vec3(homogeneous.x, homogeneous.y, homogeneous.z) / homogeneous.w;
}

Polygon TransformPolygon(const Mat4& transformation_matrix,
                         const Polygon& polygon) {  // TODO maybe inplace will be faster
    std::array<Vec3, 3> transformed_vertices = polygon.GetVertices();
    for (size_t i = 0; i < 3; ++i) {
        transformed_vertices[i] = TransformVector(transformation_matrix, transformed_vertices[i]);
    }
    return Polygon(transformed_vertices);
}

std::vector<Polygon> TransformPolygons(const Mat4& transformation_matrix,
                                       const std::vector<Polygon>& polygons) {
    std::vector<Polygon> transformed_polygons = polygons;
    for (Polygon& polygon : transformed_polygons) {
        polygon = TransformPolygon(transformation_matrix, polygon);
    }
    return transformed_polygons;
}

std::vector<Polygon> Project(const std::vector<Polygon>& polygons, const Camera& camera,
                             CoordType aspect_ratio) {
    Mat4 view_translate = glm::translate(Mat4(1.), camera.GetFocalPoint());
    Mat4 projection_matrix =
        glm::perspective(camera.GetFOV(), aspect_ratio, camera.GetNearDist(), camera.GetFarDist());
    std::vector<Polygon> projected;
    return TransformPolygons(projection_matrix * view_translate, polygons);
}

// TODO

// Picture Render(const World& world, const Camera& camera, size_t width, size_t height) {
// }

}  // namespace renderer