#include "renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <glm/ext.hpp>
#include <glm/matrix.hpp>
#include <variant>
#include <optional>
#include <vector>
#include "camera.h"
#include "color.h"
#include "glm/geometric.hpp"
#include "light.h"
#include "linalg.h"
#include "geometry.h"
#include "polygon.h"

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

struct PolygonVertex {
    Vec3 vertex;
    Vec3 normal;
    Vec2 texture_coordinates;
};

PolygonVertex PolygonSidePlaneIntersection(const Vec3& normal, CoordType offset,
                                           const PolygonVertex& point1,
                                           const PolygonVertex& point2) {
    Vec3 line_direction = point2.vertex - point1.vertex;
    CoordType t = -(glm::dot(normal, point1.vertex) + offset) / glm::dot(normal, line_direction);
    return {.vertex = point1.vertex + line_direction * t,
            .normal = point1.normal + (point2.normal - point1.normal) * t,
            .texture_coordinates = point1.texture_coordinates +
                                   (point2.texture_coordinates - point1.texture_coordinates) * t};
}

bool IsPointOnCorrectSideOfPlane(Vec3 normal, CoordType offset, const Vec3& point) {
    assert(std::abs(glm::length(normal) - 1) <= kEps &&
           "IsPointOnCorrectSideOfPlane: normal vector length must be 1");
    CoordType a = glm::dot(normal, point) + offset;
    return glm::dot(normal, point) + offset > 0;
}

PolygonVertex MakePolygonVertex(const Polygon& polygon, Index i) {
    PolygonVertex polygon_vertex = {.vertex = polygon.vertices[i],
                                    .normal = polygon.normals[i],
                                    .texture_coordinates = {-1, -1}};
    if (polygon.texture_vertices) {
        polygon_vertex.texture_coordinates = polygon.texture_vertices.value()[i];
    }
    return polygon_vertex;
}

Polygon MakePolygonFromVertices(const PolygonVertex& a, const PolygonVertex& b,
                                const PolygonVertex& c) {
    Polygon polygon;
    polygon.vertices = {a.vertex, b.vertex, c.vertex};
    polygon.normals = {a.normal, b.normal, c.normal};
    polygon.texture_vertices = std::nullopt;

    polygon.texture_vertices = std::nullopt;
    if (a.texture_coordinates.x >= 0) {
        polygon.texture_vertices = {a.texture_coordinates, b.texture_coordinates,
                                    c.texture_coordinates};
    }
    return polygon;
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
        PolygonVertex inside = MakePolygonVertex(polygon, inside_indices[0]);
        PolygonVertex outside0 = MakePolygonVertex(polygon, outside_indices[0]);
        PolygonVertex outside1 = MakePolygonVertex(polygon, outside_indices[1]);

        PolygonVertex intersection1 =
            PolygonSidePlaneIntersection(plane_normal, plane_offset, inside, outside0);

        PolygonVertex intersection2 =
            PolygonSidePlaneIntersection(plane_normal, plane_offset, inside, outside1);
        return {MakePolygonFromVertices(inside, intersection1, intersection2)};
    }
    if (inside_indices.size() == 2) {
        if (outside_indices[0] == 1) {
            std::swap(inside_indices[0], inside_indices[1]);
        }
        PolygonVertex outside = MakePolygonVertex(polygon, outside_indices[0]);
        PolygonVertex inside0 = MakePolygonVertex(polygon, inside_indices[0]);
        PolygonVertex inside1 = MakePolygonVertex(polygon, inside_indices[1]);

        PolygonVertex intersection1 =
            PolygonSidePlaneIntersection(plane_normal, plane_offset, outside, inside0);
        PolygonVertex intersection2 =
            PolygonSidePlaneIntersection(plane_normal, plane_offset, outside, inside1);
        return {MakePolygonFromVertices(inside0, intersection2, intersection1),
                MakePolygonFromVertices(inside0, inside1, intersection2)};
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

Color CalculateLightColor(const Light& light, const Vec3& position, const Vec3& normal,
                          const Material& material, Vec2 texture_coordinates) {
    Color ambient_color = material.ambient;
    if (material.ambient_texture) {
        ambient_color = DiscreteColorToColor(
            material.ambient_texture.value()->SampleColor(texture_coordinates));
    }
    Color diffuse_color = material.diffuse;
    if (material.diffuse_texture) {
        diffuse_color = DiscreteColorToColor(
            material.diffuse_texture.value()->SampleColor(texture_coordinates));
    }
    Color specular_color = material.specular;
    if (material.specular_texture) {
        specular_color = DiscreteColorToColor(
            material.specular_texture.value()->SampleColor(texture_coordinates));
    }
    if (std::holds_alternative<AmbientLight>(light)) {
        const AmbientLight& current_light = std::get<AmbientLight>(light);
        return current_light.color * ambient_color;
    }
    if (std::holds_alternative<DirectionalLight>(light)) {
        const DirectionalLight& current_light = std::get<DirectionalLight>(light);
        Vec3 light_direction = glm::normalize(-current_light.direction);
        Vec3 view_direction = glm::normalize(-position);
        Vec3 halfway = glm::normalize(view_direction + light_direction);

        ColorValue diff = std::max(glm::dot(light_direction, normal), 0.);
        ColorValue spec = std::pow(std::max(glm::dot(halfway, normal), 0.), material.shininess);
        Color diffuse = diffuse_color * diff;
        Color specular = specular_color * spec;
        return (diffuse + specular) * current_light.color;
    }
    if (std::holds_alternative<PointLight>(light)) {
        const PointLight& current_light = std::get<PointLight>(light);
        Vec3 light_direction = (current_light.position - position);
        ColorValue distance_to_surface = glm::length(light_direction);
        light_direction = glm::normalize(light_direction);
        Vec3 view_direction = glm::normalize(-position);
        Vec3 halfway = glm::normalize(view_direction + light_direction);
        ColorValue attenuation = 1.0f / (current_light.constant_attenuation +
                                         current_light.linear_attenuation * distance_to_surface +
                                         current_light.quadratic_attenuation * distance_to_surface *
                                             distance_to_surface);

        ColorValue diff = std::max(glm::dot(light_direction, normal), 0.);
        ColorValue spec = std::pow(std::max(glm::dot(halfway, normal), 0.), material.shininess);
        Color diffuse = diffuse_color * diff;
        Color specular = specular_color * spec;
        return (diffuse + specular) * current_light.color * attenuation;
    }
    return kBlack;
}

DiscreteColor CalculateColor(const Vec3& barycentric, const Polygon& polygon,
                             const Polygon& original_polygon, const Material& material,
                             const std::vector<Light>& lights) {
    Vec3 inv_w = {-original_polygon.vertices[0].z, -original_polygon.vertices[1].z,
                  -original_polygon.vertices[2].z};
    inv_w = 1. / inv_w;
    CoordType inv_denominator = 1. / glm::dot(barycentric, inv_w);
    Vec3 weights = barycentric * inv_w;
    Vec3 normal = (weights[0] * polygon.normals[0] + weights[1] * polygon.normals[1] +
                   weights[2] * polygon.normals[2]) *
                  inv_denominator;
    normal = glm::normalize(normal);
    Vec3 position =
        (weights[0] * original_polygon.vertices[0] + weights[1] * original_polygon.vertices[1] +
         weights[2] * original_polygon.vertices[2]) *
        inv_denominator;
    Vec2 texture_coords = {-1, -1};
    if (polygon.texture_vertices) {
        const auto& texture_vertices = polygon.texture_vertices.value();
        texture_coords = (weights[0] * texture_vertices[0] + weights[1] * texture_vertices[1] +
                          weights[2] * texture_vertices[2]) *
                         inv_denominator;
    }
    Color light_color = kBlack;
    for (const Light& light : lights) {
        light_color += CalculateLightColor(light, position, normal, material, texture_coords);
    }
    return ColorToDiscrete(light_color);
}

Light GetTransformedLight(const Camera& camera, const Light& light) {
    if (std::holds_alternative<AmbientLight>(light)) {
        return light;
    }
    if (std::holds_alternative<DirectionalLight>(light)) {
        DirectionalLight current_light = std::get<DirectionalLight>(light);
        Vec4 dir(current_light.direction, 0);
        dir = glm::transpose(camera.GetRotationMatrix()) * dir;
        current_light.direction = Vec3(dir);
        return current_light;
    }
    if (std::holds_alternative<PointLight>(light)) {
        PointLight current_light = std::get<PointLight>(light);
        Vec4 pos(current_light.position, 1.);
        pos = camera.MakeWorldToCameraMatrix() * pos;
        current_light.position = Vec3(pos);
        return current_light;
    }
    assert(false);
    return AmbientLight{};
}

}  // namespace

void Renderer::Render(const World& world, const Camera& camera, Picture* picture) {
    Index height = picture->GetHeight();
    Index width = picture->GetWidth();
    assert(height > 0 && "Height must be positive");
    assert(width > 0 && "Width must be positive");
    picture->SetDefaultColor();
    z_buffer_.SetDefaultValue(*picture);
    CoordType aspect_ratio = GetAspectRatio(Height{height}, Width{width});
    Mat4 projection_matrix =
        glm::perspective(camera.GetFOV(), GetAspectRatio(Height{height}, Width{width}),
                         camera.GetNearDist(), camera.GetFarDist());

    for (const Object3D& object : world.GetObjects()) {
        RenderObject(object, camera, world.GetLights(), projection_matrix, picture);
    }
}

void Renderer::RenderObject(const Object3D& object, const Camera& camera,
                            const std::vector<Light>& lights, const Mat4& projection_matrix,
                            Picture* picture) {
    Mat4 transform_to_camera =
        camera.MakeWorldToCameraMatrix() * glm::translate(Mat4(1.), object.GetLocalOrigin());
    for (const Mesh& mesh : object.GetMeshes()) {
        RenderMesh(mesh, camera, lights, projection_matrix, transform_to_camera, picture);
    }
}

void Renderer::RenderMesh(const Mesh& mesh, const Camera& camera, const std::vector<Light>& lights,
                          const Mat4& projection_matrix, const Mat4& transform_to_camera,
                          Picture* picture) {
    Height height = Height{picture->GetHeight()};
    Width width = Width{picture->GetWidth()};
    std::vector<Polygon> polygons;
    for (const Polygon& polygon : mesh.GetPolygons()) {
        Polygon translated_polygon(polygon);
        TransformPolygon(transform_to_camera, translated_polygon);
        if (mesh.GetMaterial().two_sided || IsVisible(translated_polygon)) {
            TransformNormals(glm::transpose(camera.GetRotationMatrix()), translated_polygon);
            polygons.emplace_back(std::move(translated_polygon));
        }
    }
    ClipPolygons(projection_matrix, polygons);
    std::vector<Light> transformed_lights;
    transformed_lights.reserve(lights.size());
    for (const Light& light : lights) {
        transformed_lights.push_back(GetTransformedLight(camera, light));
    }
    std::vector<Polygon> transformed_polygons = polygons;
    for (Polygon& transformed_polygon : transformed_polygons) {
        ProjectiveTransformPolygon(projection_matrix, transformed_polygon);
        TransformPolygonToScreenSpace(transformed_polygon, height, width);
    }
    for (Index i = 0; i < transformed_polygons.size(); ++i) {
        DrawPolygon(picture, transformed_polygons[i], polygons[i], mesh.GetMaterial(),
                    transformed_lights);
    }
}

void Renderer::DrawPolygon(Picture* picture, const Polygon& polygon,
                           const Polygon& original_polygon, const Material& material,
                           const std::vector<Light>& lights) {
    Index min_x = picture->GetWidth() + picture->GetHeight() + 1;
    Index min_y = picture->GetWidth() + picture->GetHeight() + 1;
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
    max_x = std::min((picture->GetWidth() - 1), max_x);
    max_y = std::min((picture->GetHeight() - 1), max_y);
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
            if (z_buffer_(x, y) <= current_z) {
                continue;
            }
            z_buffer_(x, y) = current_z;
            (*picture)(x, y) =
                CalculateColor(barycentric, polygon, original_polygon, material, lights);
        }
    }
}

}  // namespace renderer
