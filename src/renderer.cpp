#include "renderer.h"
#include <cassert>
#include <cmath>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <utility>
#include "linalg.h"
#include "picture.h"
#include "polygon.h"
#include "world.h"
#include "camera.h"
#include "geometry.h"

#include "glm/ext.hpp"

namespace renderer {

namespace {

Vec3 TransformVector(const Mat4& transformation_matrix, const Vec3& vector) {
    Vec4 homogeneous(vector, 1.0);
    homogeneous = transformation_matrix * homogeneous;
    assert(std::abs(homogeneous.w) > kEps && "TransformVector: Point went to infinity");
    return Vec3(homogeneous.x, homogeneous.y, homogeneous.z) / homogeneous.w;
}

Polygon TransformPolygon(const Mat4& transformation_matrix,
                         const Polygon& polygon) {  // TODO maybe inplace will be faster
    std::array<Vec3, 3> transformed_vertices = polygon.GetVertices();
    for (size_t i = 0; i < 3; ++i) {
        transformed_vertices[i] = TransformVector(transformation_matrix, transformed_vertices[i]);
    }
    Polygon transformed_polygon(transformed_vertices);
    transformed_polygon.SetColor(polygon.GetColor());
    return transformed_polygon;
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
    Mat4 view_translate = Mat4(1.);  // temporary
    Mat4 projection_matrix = glm::perspective(glm::radians(camera.GetFOV()), aspect_ratio,
                                              camera.GetNearDist(), camera.GetFarDist());
    return TransformPolygons(projection_matrix * view_translate, polygons);
}

CoordType GetAspectRatio(Height height, Width width) {
    return static_cast<float>(width) / static_cast<float>(height);
}

void TransformPolygonToScreenSpace(Polygon& polygon, Height height, Width width) {
    std::array<Vec3, Polygon::kVertexCount>& vertices = polygon.GetVertices();

    for (Index i = 0; i < Polygon::kVertexCount; ++i) {
        vertices[i].x = std::round(static_cast<float>(width) * ((vertices[i].x + 1.f) / 2.f));
        vertices[i].y = std::round(static_cast<float>(height) * ((1.f - vertices[i].y) / 2.f));
        vertices[i].x = std::clamp(vertices[i].x, 0., static_cast<float>(width) - 1.);
        vertices[i].y = std::clamp(vertices[i].y, 0., static_cast<float>(height) - 1.);
    }
}

void SortPolygonVertices(Polygon& polygon) {
    std::array<Vec3, Polygon::kVertexCount>& vertices = polygon.GetVertices();
    if (vertices[0].y > vertices[1].y) {
        std::swap(vertices[0], vertices[1]);
    }
    if (vertices[1].y > vertices[2].y) {
        std::swap(vertices[1], vertices[2]);
    }
    if (vertices[0].y > vertices[1].y) {
        std::swap(vertices[0], vertices[1]);
    }
}

void UpdatePicture(Picture& picture, Index i, Index j, CoordType z, Color color) {
    if (picture.GetZBufferValue(j, i) > z) {
        picture.SetZBufferValue(j, i, z);
        picture.SetPixel(j, i, color);
    }
}

struct HorizontalSlice {
    Index left_x;
    Index right_x;
};

HorizontalSlice GetHorizontalSlice(Index y, const std::array<Vec3, Polygon::kVertexCount>& vertices,
                                   const Line2& line01, const Line2& line02, const Line2& line12) {
    assert(vertices[0].y <= vertices[1].y && vertices[1].y <= vertices[2].y &&
           "GetHorizontalSlice: Polygon vertices must be sorted by y");
    assert(vertices[0].y <= y && y <= vertices[2].y && "Slice should be in polygon");

    HorizontalSlice slice = {0, -1};
    if (y == vertices[0].y && y == vertices[1].y && y == vertices[2].y) {
        slice.left_x = static_cast<Index>(vertices[0].x);
        slice.left_x = std::min(slice.left_x, static_cast<Index>(vertices[1].x));
        slice.left_x = std::min(slice.left_x, static_cast<Index>(vertices[2].x));
        slice.right_x = static_cast<Index>(vertices[0].x);
        slice.right_x = std::max(slice.left_x, static_cast<Index>(vertices[1].x));
        slice.right_x = std::max(slice.left_x, static_cast<Index>(vertices[2].x));
        return slice;
    }
    if (y == vertices[0].y && y == vertices[1].y) {
        slice.left_x = std::min(vertices[0].x, vertices[1].x);
        slice.right_x = std::max(vertices[0].x, vertices[1].x);
        return slice;
    }
    if (y == vertices[1].y && y == vertices[2].y) {
        slice.left_x = std::min(vertices[1].x, vertices[2].x);
        slice.right_x = std::max(vertices[1].x, vertices[2].x);
        return slice;
    }
    // std::cerr << line02.GetCoefficients().x << ' ' << line02.GetCoefficients().y << ' '
    //           << line02.GetCoefficients().z << '\n';
    slice.left_x = static_cast<Index>(line02.GetXByY(y));
    if (y < vertices[1].y) {
        // std::cerr << line01.GetCoefficients().x << ' ' << line01.GetCoefficients().y << ' '
        //           << line01.GetCoefficients().z << '\n';
        slice.right_x = static_cast<Index>(line01.GetXByY(y));
    } else {
        // std::cerr << line12.GetCoefficients().x << ' ' << line12.GetCoefficients().y << ' '
        //           << line12.GetCoefficients().z << '\n';
        slice.right_x = static_cast<Index>(line12.GetXByY(y));
    }
    if (slice.left_x > slice.right_x) {
        std::swap(slice.left_x, slice.right_x);
    }
    return slice;
}

CoordType GetPolygonZProjection(Vec2 xy, const std::array<Vec3, Polygon::kVertexCount>& vertices,
                                const Plane& polygon_plane) {
    Vec4 plane_coefs = polygon_plane.GetCoefficients();
    // assert(std::abs(plane_coefs.z) > kEps && "Polygon shouldn't be perpendicular to view plane");
    return polygon_plane.GetZByXY(xy);
}

void DrawPolygon(Picture& picture, const Polygon& polygon) {
    const std::array<Vec3, Polygon::kVertexCount>& vertices = polygon.GetVertices();
    assert(vertices[0].y <= vertices[1].y && vertices[1].y <= vertices[2].y &&
           "To draw polygon vertices must be sorted by y");
    std::cerr << vertices[0].x << ' ' << vertices[0].y << ' ' << vertices[0].z << '\n';
    std::cerr << vertices[1].x << ' ' << vertices[1].y << ' ' << vertices[1].z << '\n';
    std::cerr << vertices[2].x << ' ' << vertices[2].y << ' ' << vertices[2].z << '\n';
    Plane polygon_plane(polygon);
    if (std::abs(polygon_plane.GetCoefficients().z) < kEps) {
        return;
    }
    Line2 line01(vertices[0], vertices[1]);
    Line2 line02(vertices[0], vertices[2]);
    Line2 line12(vertices[1], vertices[2]);
    std::cerr << '\n';
    for (Index i = vertices[0].y; i <= vertices[2].y; ++i) {
        HorizontalSlice slice = GetHorizontalSlice(i, vertices, line01, line02, line12);
        for (int j = slice.left_x; j <= slice.right_x; ++j) {
            UpdatePicture(picture, i, j, GetPolygonZProjection({j, i}, vertices, polygon_plane),
                          polygon.GetColor());
        }
    }
}

}  // namespace

Picture Renderer::Render(const World& world, const Camera& camera, Height height, Width width) {
    CoordType aspect_ratio = GetAspectRatio(height, width);
    std::vector<Polygon> transformed_polygons = Project(world.GetPolygons(), camera, aspect_ratio);
    for (Index i = 0; i < transformed_polygons.size(); ++i) {
        TransformPolygonToScreenSpace(transformed_polygons[i], height, width);
        SortPolygonVertices(transformed_polygons[i]);
    }
    Picture picture(height, width);
    for (Index i = 0; i < transformed_polygons.size(); ++i) {
        DrawPolygon(picture, transformed_polygons[i]);
    }
    return picture;
}

}  // namespace renderer
