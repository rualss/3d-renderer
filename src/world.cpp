#include "world.h"

namespace renderer {

World::World() = default;
World::World(std::vector<Object3D>&& objects) : objects_(std::move(objects)) {
}

World::World(const std::vector<Object3D>& objects) : objects_(objects) {
}

const std::vector<Object3D>& World::GetObjects() const {
    return objects_;
}

void World::AddObject(const Object3D& object) {
    objects_.push_back(object);
}

void World::AddObject(Object3D&& object) {
    objects_.emplace_back(std::move(object));
}

}  // namespace renderer
