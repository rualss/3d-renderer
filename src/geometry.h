#pragma once

#include "linalg.h"
#include "polygon.h"

namespace renderer {

class Line2 {
public:
    Line2(const Vec2& p1, const Vec2& p2);

    CoordType GetXByY(CoordType y) const;
    CoordType GetYByX(CoordType x) const;
    Vec3 GetCoefficients() const;

private:
    CoordType a_;
    CoordType b_;
    CoordType c_;
};

class Plane {
public:
    Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3);
    Plane(const std::array<Vec3, Polygon::kVertexCount>& vertices);
    Plane(const Polygon& polygon);

    CoordType GetZByXY(Vec2 xy) const;
    Vec4 GetCoefficients() const;

private:
    bool IsDifferent(Vec3 p1, Vec3 p2);
    CoordType a_;
    CoordType b_;
    CoordType c_;
    CoordType d_;
};

}  // namespace renderer
