#pragma once

#include "linalg.h"
#include "picture.h"
#include "world.h"
#include "camera.h"

namespace renderer {

class Renderer {
public:
    Picture Render(const World& world, const Camera& camera, Height height, Width width);
};

}  // namespace renderer
