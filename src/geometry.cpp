#include "geometry.h"
#include <cassert>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include "linalg.h"

namespace renderer {

Line2::Line2(Vec2 p1, Vec2 p2) {
    assert(glm::length(p1 - p2) > kEps && "To construct a line points should be different");
    a_ = (p1.y - p2.y);
    b_ = (p2.x - p1.x);
    c_ = (p2.y - p1.y) * p1.x + (p1.x - p2.x) * p1.y;
}

CoordType Line2::GetXByY(CoordType y) const {
    assert(std::abs(a_) > kEps && "Division by zero");
    return -(c_ + b_ * y) / a_;
}

CoordType Line2::GetYByX(CoordType x) const {
    assert(std::abs(b_) > kEps && "Division by zero");
    return -(c_ + a_ * x) / b_;
}

Plane::Plane(Vec3 p1, Vec3 p2, Vec3 p3) {
    // assert(IsDifferent(p1, p2) && "Points should be different to construct a plane");
    // assert(IsDifferent(p1, p3) && "Points should be different to construct a plane");
    // assert(IsDifferent(p3, p2) && "Points should be different to construct a plane");
    a_ = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
    b_ = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
    c_ = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    assert((std::abs(a_) > kEps || std::abs(b_) > kEps || std::abs(c_) > kEps) &&
           "Points shouldn't be collinear to construct a plane");
    d_ = -(a_ * p1.x + b_ * p1.y + c_ * p1.z);
}

Plane::Plane(const std::array<Vec3, Polygon::kVertexCount>& vertices)
    : Plane(vertices[0], vertices[1], vertices[2]) {
}

Plane::Plane(const Polygon& polygon) : Plane(polygon.GetVertices()) {
}

CoordType Plane::GetZByXY(Vec2 xy) const {
    assert(std::abs(c_) > kEps && "Division by zero");
    return -(a_ * xy[0] + b_ * xy[1] + d_) / c_;
}

bool IsDifferent(Vec3 p1, Vec3 p2) {
    return glm::length(p1 - p2) > kEps;
}

Vec4 Plane::GetCoefficients() const {
    return Vec4(a_, b_, c_, d_);
}

}  // namespace renderer
