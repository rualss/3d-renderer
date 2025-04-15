#include "world.h"

namespace renderer {

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
    objects_.push_back(std::move(object));
}

void World::AddLight(const Light& light) {
    lights_.push_back(light);
}

void World::AddLight(Light&& light) {
    lights_.push_back(std::move(light));
}

const std::vector<Light>& World::GetLights() const {
    return lights_;
}

}  // namespace renderer
