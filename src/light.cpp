#include "light.h"
#include <cassert>
#include <glm/geometric.hpp>

namespace renderer {

Light::Light() = default;

Light::Light(const Vec3& direction, CoordType intensity)
    : direction_(direction), intensity_(intensity) {
    assert((direction_ != Vec3{0, 0, 0}) && "Direction must not be zero vector");
    direction_ = glm::normalize(direction_);
}

}  // namespace renderer
