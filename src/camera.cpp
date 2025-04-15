#include "camera.h"
#include <cassert>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "linalg.h"

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

void Camera::Move(const Vec3& shift) {
    position_ += shift;
}

void Camera::Rotate(Axis axis, CoordType angle) {
    switch (axis) {
        case Axis::X:
            rotation_ = glm::rotate(Mat4(rotation_), glm::radians(angle), Vec3{1, 0, 0});
            break;
        case Axis::Y:
            rotation_ = glm::rotate(Mat4(rotation_), glm::radians(angle), Vec3{0, 1, 0});
            break;
        case Axis::Z:
            rotation_ = glm::rotate(Mat4(rotation_), glm::radians(angle), Vec3{0, 0, -1});
            break;
        default:
            assert(false);
            break;
    }
}

Mat4 Camera::MakeWorldToCameraMatrix() const {
    return glm::transpose(rotation_) * glm::translate(Mat4(1.), -position_);
}

Vec3 Camera::GetRightDirecton() const {
    return glm::normalize(rotation_[0]);
}

Vec3 Camera::GetUpDirection() const {
    return glm::normalize(rotation_[1]);
}

Vec3 Camera::GetForwardDirection() const {
    return -glm::normalize(rotation_[2]);
}

const Mat4& Camera::GetRotationMatrix() const {
    return rotation_;
}

}  // namespace renderer
