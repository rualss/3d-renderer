#pragma once

#include "linalg.h"

namespace renderer {
class Light {
public:
    Light();
    Light(const Vec3& direction, CoordType intensity);

private:
    static constexpr CoordType kDefaultIntensity = 1.;
    static constexpr Vec3 kDefaultDirection = {0, 0, -1};

    CoordType intensity_ = kDefaultIntensity;
    Vec3 direction_ = kDefaultDirection;
};
}  // namespace renderer
