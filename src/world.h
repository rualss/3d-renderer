#pragma once

#include "light.h"
#include "object_3d.h"

#include <vector>

namespace renderer {

class World {
public:
    World() = default;
    World(std::vector<Object3D>&& objects);
    World(const std::vector<Object3D>& objects);

    const std::vector<Object3D>& GetObjects() const;
    void AddObject(const Object3D& object);
    void AddObject(Object3D&& object);
    void AddLight(const Light& light);
    void AddLight(Light&& light);
    const std::vector<Light>& GetLights() const;

private:
    std::vector<Object3D> objects_;
    std::vector<Light> lights_;
};

}  // namespace renderer
