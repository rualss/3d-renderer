#include "renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <execution>
#include <glm/ext.hpp>
#include <glm/matrix.hpp>
#include "light.h"
#include "linalg.h"
#include "picture.h"
#include "polygon.h"
#include "geometry.h"

namespace renderer {

namespace {

void ProjectiveTransformVector(const Mat4& transformation_matrix, Vec3& vector) {
    Vec4 homogeneous(vector, 1.0);
    homogeneous = transformation_matrix * homogeneous;
    assert(std::abs(homogeneous.w) > kEps && "TransformVector: Point went to infinity");
    vector = Vec3(homogeneous / homogeneous.w);
}

void ProjectiveTransformPolygon(const Mat4& transformation_matrix, Polygon& polygon) {
    for (size_t i = 0; i < Polygon::kVertexCount; ++i) {
        ProjectiveTransformVector(transformation_matrix, polygon[i]);
    }
}

CoordType GetAspectRatio(Height height, Width width) {
    return static_cast<CoordType>(width) / static_cast<CoordType>(height);
}

void TransformPolygonToScreenSpace(Polygon& polygon, Height height, Width width) {
    CoordType real_height = static_cast<CoordType>(height);
    CoordType real_width = static_cast<CoordType>(width);

    for (Index i = 0; i < Polygon::kVertexCount; ++i) {
        polygon[i].x = real_width * ((polygon[i].x + 1.) / 2.);
        polygon[i].y = real_height * ((1. - polygon[i].y) / 2.);
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
    return barycentric[0] * polygon[0].z + barycentric[1] * polygon[1].z +
           barycentric[2] * polygon[2].z;
}

bool IsVisible(const Polygon& polygon) {
    Vec3 look_dir = polygon[0];
    return glm::dot(look_dir, polygon.GetNonUnitNormal()) > 0;
}

std::vector<Polygon> GetPolygons(const World& world, const Camera& camera) {
    std::vector<Polygon> polygons;
    for (const Mesh& mesh : world.GetMeshes()) {
        Mat4 transform_to_camera =
            camera.GetWorldToCameraMatrix() * glm::translate(Mat4(1.), mesh.GetLocalOrigin());
        for (const Polygon& polygon : mesh.GetPolygons()) {
            Polygon translated_polygon(polygon);
            translated_polygon.ApplyMatrix(transform_to_camera);
            if (IsVisible(translated_polygon)) {
                polygons.emplace_back(std::move(translated_polygon));
            }
        }
    }
    return polygons;
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
        if (IsPointOnCorrectSideOfPlane(plane_normal, plane_offset, polygon[i])) {
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
        Vec3 intersection1 = GetLinePlaneIntersection(
            plane_normal, plane_offset, polygon[inside_indices[0]], polygon[outside_indices[0]]);
        Vec3 intersection2 = GetLinePlaneIntersection(
            plane_normal, plane_offset, polygon[inside_indices[0]], polygon[outside_indices[1]]);
        return {
            Polygon{polygon[inside_indices[0]], intersection1, intersection2, polygon.GetColor()}};
    }
    if (inside_indices.size() == 2) {
        if (outside_indices[0] == 1) {
            std::swap(inside_indices[0], inside_indices[1]);
        }
        Vec3 intersection1 = GetLinePlaneIntersection(
            plane_normal, plane_offset, polygon[outside_indices[0]], polygon[inside_indices[0]]);
        Vec3 intersection2 = GetLinePlaneIntersection(
            plane_normal, plane_offset, polygon[outside_indices[0]], polygon[inside_indices[1]]);
        return {
            Polygon{polygon[inside_indices[0]], intersection2, intersection1, polygon.GetColor()},
            Polygon(polygon[inside_indices[0]], polygon[inside_indices[1]], intersection2,
                    polygon.GetColor())};
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

Color MultiplyColor(Color color, CoordType multiplier) {
    assert(0 <= multiplier && multiplier <= 1 && "MultiplyColor: multiplier must be in [0, 1]");
    Vec3 tmp(color);
    tmp *= multiplier;
    for (Index i = 0; i < 3; ++i) {
        tmp[i] = std::round(tmp[i]);
    }
    color = Color(tmp);
    for (Index i = 0; i < 3; ++i) {
        color[i] = std::clamp(color[i], 0, kColorMax);
    }
    return color;
}

void CalculateLightColor(const Light& light, Polygon& polygon) {
    CoordType intensity_on_polygon =
        glm::dot(light.GetDirection(), polygon.GetUnitNormal()) * light.GetIntensity();
    intensity_on_polygon = std::clamp(intensity_on_polygon, 0., 1.);
    intensity_on_polygon += 0.1;
    intensity_on_polygon = std::clamp(intensity_on_polygon, 0., 1.);
    polygon.SetColor(MultiplyColor(polygon.GetColor(), intensity_on_polygon));
}

Light GetRotatedLight(const Light& light, const Camera& camera) {
    return light.GetTransformed(glm::transpose(camera.GetRotationMatrix()));
}

}  // namespace

void Renderer::Render(const World& world, const Camera& camera, const Light& light,
                      Picture&& picture) {
    Height height = Height{picture.GetHeight()};
    Width width = Width{picture.GetWidth()};
    assert(height > 0 && "Height must be positive");
    assert(width > 0 && "Width must be positive");
    picture.Reset();
    ResetZBuffer(picture);
    CoordType aspect_ratio = GetAspectRatio(height, width);
    std::vector<Polygon> polygons = GetPolygons(world, camera);
    Mat4 projection_matrix = glm::perspective(camera.GetFOV(), GetAspectRatio(height, width),
                                              camera.GetNearDist(), camera.GetFarDist());
    ClipPolygons(projection_matrix, polygons);
    std::vector<Polygon> transformed_polygons = polygons;
    Light rotated_light = GetRotatedLight(light, camera);
    for (Polygon& transformed_polygon : transformed_polygons) {
        CalculateLightColor(rotated_light, transformed_polygon);
        ProjectiveTransformPolygon(projection_matrix, transformed_polygon);
        TransformPolygonToScreenSpace(transformed_polygon, height, width);
    }
    for (Index i = 0; i < transformed_polygons.size(); ++i) {
        DrawPolygon(picture, transformed_polygons[i]);
    }
}

void Renderer::DrawPolygon(Picture& picture, const Polygon& polygon) {
    Index min_x = picture.GetWidth() + picture.GetHeight() + 1;
    Index min_y = picture.GetWidth() + picture.GetHeight() + 1;
    Index max_x = -1;
    Index max_y = -1;
    for (int i = 0; i < Polygon::kVertexCount; ++i) {
        min_x = std::min(RoundDown(polygon[i].x), min_x);
        min_y = std::min(RoundDown(polygon[i].y), min_y);
        max_x = std::max(RoundUp(polygon[i].x), max_x);
        max_y = std::max(RoundUp(polygon[i].y), max_y);
    }
    // assert("bounded dimensions are not OK" && max_x < 2 * screen->GetWidth() &&
    //        max_y < 2 * screen->GetHeight());
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
            picture(x, y) = polygon.GetColor();
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
