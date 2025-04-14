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

private:
    void DrawPolygon(Picture& picture, const Polygon& polygon);
    void ResetZBuffer(const Picture& picture);

    std::vector<CoordType> z_buffer_;
};

}  // namespace renderer
