#pragma once
#include "glm/glm.hpp"
#include "linalg.h"

namespace renderer {

class Camera {
public:
    Camera() = default;
    Camera(Vec3 focal_point, CoordType near_dist, CoordType far_dist, CoordType fov_y);

    Vec3 GetFocalPoint() const;

    CoordType GetFOV() const;

    CoordType GetNearDist() const;

    CoordType GetFarDist() const;

private:
    static constexpr Vec3 kDefaultFocalPoint = {0, 0, 0};
    static constexpr CoordType kDefaultNearDist = 0.1;
    static constexpr CoordType kDefaultFarDist = 100.0;
    static constexpr CoordType kDefaultFOV = 45.0;

    Vec3 focal_point_ = kDefaultFocalPoint;
    CoordType near_dist_ = kDefaultNearDist;
    CoordType far_dist_ = kDefaultFarDist;
    CoordType fov_y_ = kDefaultFOV;
};

}  // namespace renderer
