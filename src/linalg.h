#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace renderer {
using Vec2 = glm::dvec2;
using Vec3 = glm::dvec3;
using Vec4 = glm::dvec4;
using Mat3 = glm::dmat3;
using Mat4 = glm::dmat4;
using CoordType = double;
using Color = glm::vec<3, int32_t>;
using Index = int32_t;

const static CoordType kEps = 1e-6;

enum class Height : int32_t {};
enum class Width : int32_t {};

constexpr static Index kColorMax = 255;
constexpr static Color kBlack = {0, 0, 0};

}  // namespace renderer
