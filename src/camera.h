#pragma once
#include "glm/glm.hpp"
#include "polygon.h"  // I don't like to include header that has 0 connection with camera just for types
                      // But I didn't come up with a beautiful solution

namespace renderer {

class Camera {

    static constexpr CoordType kDefaultNearDist = 0.1;
    static constexpr CoordType kDefaultFarDist = 10.0;
    static constexpr Vec3 kDefaultFocalPoint = {0, 0, 0};
    static constexpr CoordType kDefaultFOV = 45.0;

public:
    Camera()
        : focal_point_(kDefaultFocalPoint),
          near_dist_(kDefaultNearDist),
          far_dist_(kDefaultFarDist),
          fov_y_(kDefaultFOV) {
    }
    Camera(Vec3 focal_point, CoordType near_dist, CoordType far_dist, CoordType fov_y)
        : focal_point_(focal_point), near_dist_(near_dist), far_dist_(far_dist), fov_y_(fov_y) {
    }

    Vec3 GetFocalPoint() const;

    CoordType GetFOV() const;

    CoordType GetNearDist() const;

    CoordType GetFarDist() const;

private:
    Vec3 focal_point_;
    CoordType near_dist_;
    CoordType far_dist_;
    CoordType fov_y_;
};

}  // namespace renderer
