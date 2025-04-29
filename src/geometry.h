#pragma once

#include "linalg.h"
#include "polygon.h"

namespace renderer {

class BarycentricCoordinateSystem {
public:
    BarycentricCoordinateSystem(const Polygon& polygon);
    Vec3 GetBarycentricCoordinates(const Vec2& p);

private:
    Vec2 a_;
    Vec2 v0_;
    Vec2 v1_;
    CoordType inv_denominator_;
};

}  // namespace renderer
