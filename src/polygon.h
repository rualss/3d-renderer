#pragma once

#include "linalg.h"
#include <array>

namespace renderer {

class Polygon {
public:
    static constexpr Index kVertexCount = 3;
    static constexpr Color kDefaultColor = kBlack;

    Polygon(const Vec3& v1, const Vec3& v2, const Vec3& v3, Color color);
    explicit Polygon(const std::array<Vec3, kVertexCount>& vertices, Color color);
    Polygon(const Polygon& other);
    Polygon(Polygon&& other);
    Polygon& operator=(const Polygon& other);
    Polygon& operator=(Polygon&& other);
    ~Polygon();
    void Swap(Polygon& other);

    void SetColor(const Color& color);
    Color GetColor() const;
    void ApplyMatrix(const Mat4& mat);
    Vec3& operator[](Index i);
    const Vec3& operator[](Index i) const;
    Vec3 GetUnitNormal() const;
    Vec3 GetNonUnitNormal() const;

private:
    Color color_ = kDefaultColor;
    std::array<Vec3, kVertexCount> vertices_;
};

}  // namespace renderer
