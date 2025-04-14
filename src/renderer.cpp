#include "renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <execution>
#include <glm/ext.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "light.h"
#include "linalg.h"
#include "material.h"
#include "object_3d.h"
#include "picture.h"
#include "polygon.h"
#include "geometry.h"

namespace renderer {

namespace {

CoordType GetAspectRatio(Height height, Width width) {
    return static_cast<CoordType>(width) / static_cast<CoordType>(height);
}

void TransformPolygonToScreenSpace(Polygon& polygon, Height height, Width width) {
    CoordType real_height = static_cast<CoordType>(height);
    CoordType real_width = static_cast<CoordType>(width);

    for (Index i = 0; i < Polygon::kVertexCount; ++i) {
        polygon.vertices[i].x = real_width * ((polygon.vertices[i].x + 1.) / 2.);
        polygon.vertices[i].y = real_height * ((1. - polygon.vertices[i].y) / 2.);
    }
}

bool IsInsidePolygon(const Vec3& barycentric_coordinates) {
    return barycentric_coordinates.x <= 1.0 && barycentric_coordinates.x >= 0.0 &&
           barycentric_coordinates.y <= 1.0 && barycentric_coordinates.y >= 0.0 &&
           barycentric_coordinates.z <= 1.0 && barycentric_coordinates.z >= 0.0;
}

Index RoundDown(CoordType coordinate) {
    return static_cast<size_t>(std::max(0.0, std::floor(coordinate)));
}

Index RoundUp(CoordType coordinate) {
    return static_cast<size_t>(std::ceil(coordinate));
}

CoordType CalculateZ(const Vec3& barycentric, const Polygon& polygon) {
    return barycentric[0] * polygon.vertices[0].z + barycentric[1] * polygon.vertices[1].z +
           barycentric[2] * polygon.vertices[2].z;
}

bool IsVisible(const Polygon& polygon) {
    Vec3 look_dir = polygon.vertices[0];
    return glm::dot(look_dir, GetNonUnitNormal(polygon)) > 0;
}

Vec3 GetLinePlaneIntersection(Vec3 normal, CoordType offset, Vec3 point1, Vec3 point2) {
    Vec3 line_direction = point2 - point1;
    CoordType t = -(glm::dot(normal, point1) + offset) / glm::dot(normal, line_direction);
    return point1 + t * line_direction;
}

bool IsPointOnCorrectSideOfPlane(Vec3 normal, CoordType offset, const Vec3& point) {
    assert(std::abs(glm::length(normal) - 1) <= kEps &&
           "IsPointOnCorrectSideOfPlane: normal vector length must be 1");
    CoordType a = glm::dot(normal, point) + offset;
    return glm::dot(normal, point) + offset > 0;
}

std::vector<Polygon> ClipPolygon(Vec3 plane_normal, CoordType plane_offset,
                                 const Polygon& polygon) {
    std::vector<Index> inside_indices;
    std::vector<Index> outside_indices;
    for (Index i = 0; i < Polygon::kVertexCount; ++i) {
        if (IsPointOnCorrectSideOfPlane(plane_normal, plane_offset, polygon.vertices[i])) {
            inside_indices.push_back(i);
        } else {
            outside_indices.push_back(i);
        }
    }
    if (inside_indices.size() == 0) {
        return {};
    }
    if (inside_indices.size() == 1) {
        if (inside_indices[0] == 1) {
            std::swap(outside_indices[0], outside_indices[1]);
        }
        Vec3 intersection1 = GetLinePlaneIntersection(plane_normal, plane_offset,
                                                      polygon.vertices[inside_indices[0]],
                                                      polygon.vertices[outside_indices[0]]);
        Vec3 intersection2 = GetLinePlaneIntersection(plane_normal, plane_offset,
                                                      polygon.vertices[inside_indices[0]],
                                                      polygon.vertices[outside_indices[1]]);
        return {Polygon{polygon.vertices[inside_indices[0]], intersection1, intersection2}};
    }
    if (inside_indices.size() == 2) {
        if (outside_indices[0] == 1) {
            std::swap(inside_indices[0], inside_indices[1]);
        }
        Vec3 intersection1 = GetLinePlaneIntersection(plane_normal, plane_offset,
                                                      polygon.vertices[outside_indices[0]],
                                                      polygon.vertices[inside_indices[0]]);
        Vec3 intersection2 = GetLinePlaneIntersection(plane_normal, plane_offset,
                                                      polygon.vertices[outside_indices[0]],
                                                      polygon.vertices[inside_indices[1]]);
        return {Polygon{polygon.vertices[inside_indices[0]], intersection2, intersection1},
                Polygon{polygon.vertices[inside_indices[0]], polygon.vertices[inside_indices[1]],
                        intersection2}};
    }
    return {polygon};
}

void NormalizePlane(Vec4& plane) {
    plane /= glm::length(Vec3(plane));
}

std::array<Vec4, 2> GetClippingPlanes(const Mat4& projection_matrix) {
    std::array<Vec4, 2> frustum_planes;
    frustum_planes[0] = glm::row(projection_matrix, 3) + glm::row(projection_matrix, 2);
    frustum_planes[1] = glm::row(projection_matrix, 3) - glm::row(projection_matrix, 2);
    NormalizePlane(frustum_planes[0]);
    NormalizePlane(frustum_planes[1]);
    return frustum_planes;
}

void ClipPolygons(const Mat4& projection_matrix, std::vector<Polygon>& polygons_to_clip) {
    auto frustum_planes = GetClippingPlanes(projection_matrix);
    for (Index i = 0; i < frustum_planes.size(); ++i) {
        std::vector<Polygon> clipped;
        for (Polygon& polygon : polygons_to_clip) {
            for (Polygon& clipped_polygon :
                 ClipPolygon(Vec3(frustum_planes[i]), frustum_planes[i].w, polygon)) {
                clipped.emplace_back(std::move(clipped_polygon));
            }
        }
        polygons_to_clip = std::move(clipped);
    }
}

DiscreteColor CalculateColor(const Vec3& barycentric, const Polygon& polygon,
                             const Polygon& original_polygon, const Material& material,
                             const std::vector<Light>& lights) {
    Vec3 inv_w = {original_polygon.vertices[0].z, original_polygon.vertices[1].z,
                  original_polygon.vertices[2].z};
    inv_w = -1. / inv_w;
    CoordType inv_denominator = glm::dot(barycentric, inv_w);
    Vec3 weights = barycentric * inv_w;
    // Vec3 normal = (weights[0] * polygon.normals[0] + weights[1] * polygon.normals[1] +
    //                weights[2] * polygon.normals[2]) *
    //               inv_denominator;
    DiscreteColor color;
    assert(material.diffuse_texture && polygon.texture_vertices && "Texture must be present");
    const auto& texture_vertices = polygon.texture_vertices.value();
    Vec2 texture_coords = (weights[0] * texture_vertices[0] + weights[1] * texture_vertices[1] +
                           weights[2] * texture_vertices[2]) *
                          inv_denominator;
    return material.diffuse_texture.value().SampleColor(texture_coords);
}

}  // namespace

void Renderer::Render(const World& world, const Camera& camera, Picture&& picture) {
    Height height = Height{picture.GetHeight()};
    Width width = Width{picture.GetWidth()};
    assert(height > 0 && "Height must be positive");
    assert(width > 0 && "Width must be positive");
    picture.Reset();
    ResetZBuffer(picture);
    CoordType aspect_ratio = GetAspectRatio(height, width);
    Mat4 projection_matrix = glm::perspective(camera.GetFOV(), GetAspectRatio(height, width),
                                              camera.GetNearDist(), camera.GetFarDist());

    for (const Object3D& object : world.GetObjects()) {
    }
}

void Renderer::RenderObject(const Object3D& object, const Camera& camera,
                            const std::vector<Light>& lights, const Mat4& projection_matrix,
                            Picture&& picture) {
    Mat4 transform_to_camera =
        camera.GetWorldToCameraMatrix() * glm::translate(Mat4(1.), object.GetLocalOrigin());
    for (const Mesh& mesh : object.GetMeshes()) {
    }
}

void Renderer::RenderMesh(const Mesh& mesh, const Camera& camera, const std::vector<Light>& lights,
                          const Mat4& projection_matrix, const Mat4& transform_to_camera,
                          Picture&& picture) {
    Height height = Height{picture.GetHeight()};
    Width width = Width{picture.GetWidth()};
    std::vector<Polygon> polygons;
    for (const Polygon& polygon : mesh.GetPolygons()) {
        Polygon translated_polygon(polygon);
        TransformPolygon(transform_to_camera, translated_polygon);
        if (mesh.GetMaterial().two_sided || IsVisible(polygon)) {
            polygons.emplace_back(std::move(translated_polygon));
        }
    }
    ClipPolygons(projection_matrix, polygons);
    std::vector<Polygon> transformed_polygons = polygons;
    for (Polygon& transformed_polygon : transformed_polygons) {
        ProjectiveTransformPolygon(projection_matrix, transformed_polygon);
        TransformPolygonToScreenSpace(transformed_polygon, height, width);
    }
    for (Index i = 0; i < transformed_polygons.size(); ++i) {
        DrawPolygon(picture, transformed_polygons[i], polygons[i], mesh.GetMaterial(), lights);
    }
}

void Renderer::DrawPolygon(Picture& picture, const Polygon& polygon,
                           const Polygon& original_polygon, const Material& material,
                           const std::vector<Light>& lights) {
    Index min_x = picture.GetWidth() + picture.GetHeight() + 1;
    Index min_y = picture.GetWidth() + picture.GetHeight() + 1;
    Index max_x = -1;
    Index max_y = -1;
    for (int i = 0; i < Polygon::kVertexCount; ++i) {
        min_x = std::min(RoundDown(polygon.vertices[i].x), min_x);
        min_y = std::min(RoundDown(polygon.vertices[i].y), min_y);
        max_x = std::max(RoundUp(polygon.vertices[i].x), max_x);
        max_y = std::max(RoundUp(polygon.vertices[i].y), max_y);
    }
    min_x = std::max(0, min_x);
    min_y = std::max(0, min_y);
    max_x = std::min((picture.GetWidth() - 1), max_x);
    max_y = std::min((picture.GetHeight() - 1), max_y);
    BarycentricCoordinateSystem barycentric_system(polygon);
    for (Index x = min_x; x <= max_x; ++x) {
        for (Index y = min_y; y <= max_y; ++y) {
            Vec2 point_to_check = {static_cast<CoordType>(x) + 0.5,
                                   static_cast<CoordType>(y) + 0.5};
            Vec3 barycentric = barycentric_system.GetBarycentricCoordinates(point_to_check);
            if (!IsInsidePolygon(barycentric)) {
                continue;
            }
            CoordType current_z = CalculateZ(barycentric, polygon);
            Index pixel_index = y * picture.GetWidth() + x;
            if (z_buffer_[pixel_index] <= current_z) {
                continue;
            }
            z_buffer_[pixel_index] = current_z;
            picture(x, y) =
                CalculateColor(barycentric, polygon, original_polygon, material, lights);
        }
    }
}

void Renderer::ResetZBuffer(const Picture& picture) {
    if (z_buffer_.size() < picture.GetPixels().size()) {
        z_buffer_.resize(picture.GetPixels().size());
    }
    std::fill(std::execution::par, z_buffer_.begin(), z_buffer_.end(), 2.);
}

}  // namespace renderer
