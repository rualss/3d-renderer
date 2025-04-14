#pragma once

#include <variant>
#include "linalg.h"

namespace renderer {

struct AmbientLight {
    Color color = color::kWhite;
};

struct DirectionalLight {
    Color color = color::kWhite;
    Vec3 direction = {0, 0, -1};
};

struct PointLight {
    Color color = color::kWhite;
    CoordType constant = 1;
    CoordType linear = 0.22;
    CoordType quadratic = 0.20;
    Vec3 position = {0, 0, 0};
};

using Light = std::variant<AmbientLight, DirectionalLight, PointLight>;

}  // namespace renderer
