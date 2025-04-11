#include "light.h"
#include <cassert>
#include <glm/geometric.hpp>

namespace renderer {

Light::Light() = default;

Light::Light(const Vec3& direction, CoordType intensity)
    : direction_(direction), intensity_(intensity) {
    assert((direction_ != Vec3{0, 0, 0}) && "Light: Direction must not be zero vector");
    assert(0. <= intensity_ && intensity_ <= 1. && "Light: Intensity must be in [0, 1]");
    direction_ = glm::normalize(direction_);
}

const Vec3& Light::GetDirection() const {
    return direction_;
}

CoordType Light::GetIntensity() const {
    return intensity_;
}

Light Light::GetTransformed(const Mat4& mat) const {
    Vec4 homogeneous{direction_, 1.};
    homogeneous = mat * homogeneous;
    homogeneous /= homogeneous.w;
    return {Vec3(homogeneous), intensity_};
}

}  // namespace renderer
