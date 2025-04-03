#pragma once

#include "glm/glm.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "linalg.h"

#include <vector>
#include <array>

namespace renderer {

class Polygon {
public:
    static constexpr size_t kVertexCount = 3;

    Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    explicit Polygon(const std::array<Vec3, kVertexCount>& vertices);

    const std::array<Vec3, kVertexCount>& GetVertices() const;
    std::array<Vec3, kVertexCount>& GetVertices();
    void SetColor(const Color& color);
    Color GetColor() const;

private:
    Color color_;
    std::array<Vec3, kVertexCount> vertices_;
};

}  // namespace renderer
