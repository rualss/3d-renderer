#include "camera.h"

namespace renderer {

Camera::Camera(const Vec3& focal_point, CoordType near_dist, CoordType far_dist, CoordType fov_y)
    : focal_point_(focal_point), near_dist_(near_dist), far_dist_(far_dist), fov_y_(fov_y) {
}

const Vec3& Camera::GetFocalPoint() const {
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
