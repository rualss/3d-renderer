#pragma once

#include "linalg.h"
#include <array>

namespace renderer {

class Polygon {
public:
    static constexpr size_t kVertexCount = 3;

    Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    explicit Polygon(const std::array<Vec3, kVertexCount>& vertices);
    Polygon(const Polygon& other);
    Polygon(Polygon&& other);
    Polygon& operator=(const Polygon& other);
    Polygon& operator=(Polygon&& other);
    ~Polygon();
    void Swap(Polygon& other);

    const std::array<Vec3, kVertexCount>& GetVertices() const;
    std::array<Vec3, kVertexCount>& GetVertices();
    void SetColor(const Color& color);
    Color GetColor() const;
    void ApplyMatrix(const Mat4& mat);

private:
    Color color_;
    std::array<Vec3, kVertexCount> vertices_;
};

}  // namespace renderer
