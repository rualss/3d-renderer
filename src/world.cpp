#include "world.h"

namespace renderer {

void World::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

}  // namespace renderer