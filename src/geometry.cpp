#include "geometry.h"
#include <iostream>
#include "linalg.h"

namespace renderer {

BarycentricCoordinateSystem::BarycentricCoordinateSystem(const Polygon& polygon)
    : a_(polygon[0]),
      v0_(polygon[1] - polygon[0]),
      v1_(polygon[2] - polygon[0]),
      inv_denominator_(1. / (v0_.x * v1_.y - v1_.x * v0_.y)) {
}

Vec3 BarycentricCoordinateSystem::GetBarycentricCoordinates(const Vec2& p) {
    Vec3 barycentric;
    CoordType v2x = p.x - a_.x;
    CoordType v2y = p.y - a_.y;
    barycentric.y = (v2x * v1_.y - v1_.x * v2y) * inv_denominator_;
    barycentric.z = (v0_.x * v2y - v2x * v0_.y) * inv_denominator_;
    barycentric.x = 1.0 - barycentric.y - barycentric.z;
    return barycentric;
}

}  // namespace renderer
