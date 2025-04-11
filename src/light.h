#pragma once

#include "linalg.h"

namespace renderer {
class Light {
public:
    Light();
    Light(const Vec3& direction, CoordType intensity);
    const Vec3& GetDirection() const;
    CoordType GetIntensity() const;
    Light GetTransformed(const Mat4& mat) const;

private:
    static constexpr CoordType kDefaultIntensity = 1.;
    static constexpr Vec3 kDefaultDirection = {0, 0, -1};

    CoordType intensity_ = kDefaultIntensity;
    Vec3 direction_ = kDefaultDirection;
};
}  // namespace renderer
