#pragma once

#include <variant>
#include "linalg.h"
#include "color.h"

namespace renderer {

struct AmbientLight {
    Color color = {0.2, 0.2, 0.2};
};

struct DirectionalLight {
    Color color = kWhite;
    Vec3 direction = {0, 0, -1};
};

struct PointLight {
    Color color = kWhite;
    CoordType constant_attenuation = 1;
    CoordType linear_attenuation = 0.022;
    CoordType quadratic_attenuation = 0.0019;
    Vec3 position = {0, 0, 0};
};

using Light = std::variant<AmbientLight, DirectionalLight, PointLight>;

}  // namespace renderer
