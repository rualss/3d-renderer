#include "world.h"

namespace renderer {

World::World(const std::vector<Polygon>& polygons) : polygons_(polygons) {
}

void World::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

const std::vector<Polygon>& World::GetPolygons() const {
    return polygons_;
}

std::vector<Polygon>& World::GetPolygons() {
    return polygons_;
}

}  // namespace renderer
