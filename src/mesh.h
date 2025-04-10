#pragma once

#include <iterator>
#include <vector>
#include "polygon.h"

namespace renderer {
class Mesh {
public:
    Mesh();
    Mesh(const std::string& path);
    template <std::input_iterator InputIt>
        requires std::convertible_to<std::iter_value_t<InputIt>, Polygon>
    Mesh(InputIt first, InputIt last) : polygons_(first, last) {
    }
    Mesh(const Mesh& other);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other);
    Mesh& operator=(Mesh&& other);
    ~Mesh();
    void Swap(Mesh& other);

    const std::vector<Polygon>& GetPolygons() const;
    void AddPolygon(const Polygon& polygon);
    void AddPolygon(Polygon&& polygon);
    void SetColor(const Color& color);
    void ApplyMatrix(const Mat4& mat);
    const Vec3& GetLocalOrigin() const;
    void SetLocalOrigin(const Vec3& new_origin);
    void SetColorsRandomly();

private:
    static constexpr Vec3 kDefaultLocalOrigin = {0, 0, 0};

    std::vector<Polygon> polygons_;
    Vec3 local_origin_ = kDefaultLocalOrigin;
};
}  // namespace renderer
