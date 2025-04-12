#pragma once

#include "linalg.h"
#include "picture.h"
#include "world.h"
#include "camera.h"
#include "light.h"

namespace renderer {

class Renderer {
public:
    void Render(const World& world, const Camera& camera, const Light& light, Picture&& picture);
};

}  // namespace renderer
