#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <cstdint>

namespace renderer {

using Vec2 = glm::dvec2;
using Vec3 = glm::dvec3;
using Vec4 = glm::dvec4;
using Mat3 = glm::dmat3;
using Mat4 = glm::dmat4;
using CoordType = double;
using Index = int32_t;

const static CoordType kEps = 1e-6;

}  // namespace renderer
