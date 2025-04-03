#pragma once

#include "linalg.h"
#include "polygon.h"

#include <vector>

namespace renderer {

class World {
public:
    World() = default;
    World(const std::vector<Polygon>& polygons);
    void AddPolygon(const Polygon& polygon);
    const std::vector<Polygon>& GetPolygons() const;
    std::vector<Polygon>& GetPolygons();

private:
    std::vector<Polygon> polygons_;
};

}  // namespace renderer
