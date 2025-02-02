#pragma once

#include "glm/glm.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>
#include <array>

namespace renderer {

using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using CoordType = float;

class Polygon {
public:
    static constexpr size_t kVertexCount = 3;

    Polygon() = delete;
    explicit Polygon(const std::array<Vec3, kVertexCount>& vertices) : vertices_(vertices) {
    }

    std::array<Vec3, kVertexCount> GetVertices() const;

private:
    std::array<Vec3, kVertexCount> vertices_;
};

}  // namespace renderer