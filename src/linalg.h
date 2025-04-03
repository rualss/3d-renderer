#pragma once

#include "glm/glm.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdint>

namespace renderer {
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using CoordType = float;
using Color = glm::vec<3, int32_t>;
using Index = int32_t;

const static CoordType kEps = 1e-4;  // stolen

enum class Height : int32_t {};
enum class Width : int32_t {};

constexpr static Color kBlack = {0, 0, 0};

}  // namespace renderer
