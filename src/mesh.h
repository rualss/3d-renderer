#pragma once

#include <iterator>
#include <vector>
#include "polygon.h"
#include "material.h"

namespace renderer {
class Mesh {
public:
    Mesh() = default;

    template <std::input_iterator InputIt>
        requires std::convertible_to<std::iter_value_t<InputIt>, Polygon>
    Mesh(InputIt first, InputIt last) : polygons_(first, last) {
    }

    const std::vector<Polygon>& GetPolygons() const;
    void AddPolygon(const Polygon& polygon);
    void AddPolygon(Polygon&& polygon);
    void ApplyMatrix(const Mat4& mat);
    void SetMaterial(const Material& material);
    void SetMaterial(Material&& material);
    const Material& GetMaterial() const;

private:
    Material material_;
    std::vector<Polygon> polygons_;
};
}  // namespace renderer
