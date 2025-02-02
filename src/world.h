#pragma once

#include "polygon.h"

#include <vector>

namespace renderer {

class World {
public:
    World(const std::vector<Polygon>& polygons) : polygons_(polygons) {
    }

    void AddPolygon(const Polygon& polygon);

private:
    std::vector<Polygon> polygons_;
};

}  // namespace renderer
