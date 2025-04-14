#pragma once

#include "linalg.h"
#include "picture.h"
#include "world.h"
#include "camera.h"
#include "light.h"

namespace renderer {

class Renderer {
public:
    void Render(const World& world, const Camera& camera, Picture&& picture);

private:
    void DrawPolygon(Picture& picture, const Polygon& polygon, const Polygon& original_polygon,
                     const Material& material, const std::vector<Light>& lights);
    void RenderObject(const Object3D& object, const Camera& camera,
                      const std::vector<Light>& lights, const Mat4& projection_matrix,
                      Picture&& picture);
    void RenderMesh(const Mesh& mesh, const Camera& camera, const std::vector<Light>& lights,
                    const Mat4& projection_matrix, const Mat4& transform_to_camera,
                    Picture&& picture);
    void ResetZBuffer(const Picture& picture);

    std::vector<CoordType> z_buffer_;
};

}  // namespace renderer
