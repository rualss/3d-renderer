#include "camera.h"

namespace renderer {

Vec3 Camera::GetFocalPoint() const {
    return focal_point_;
}

CoordType Camera::GetFOV() const {
    return fov_y_;
}

CoordType Camera::GetNearDist() const {
    return near_dist_;
}

CoordType Camera::GetFarDist() const {
    return far_dist_;
}

}  // namespace renderer
