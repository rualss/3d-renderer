#pragma once

#include "glm/glm.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdint>

namespace renderer {
using Vec2 = glm::dvec2;
using Vec3 = glm::dvec3;
using Vec4 = glm::dvec4;
using Mat4 = glm::dmat4;
using CoordType = double;
using Color = glm::vec<3, int32_t>;
using Index = int32_t;

const static CoordType kEps = 1e-6;

enum class Height : int32_t {};
enum class Width : int32_t {};

constexpr static Color kBlack = {0, 0, 0};

}  // namespace renderer
