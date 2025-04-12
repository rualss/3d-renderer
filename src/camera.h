#pragma once

#include "linalg.h"

namespace renderer {

class Camera {
public:
    Camera() = default;
    Camera(const Vec3& focal_point, CoordType near_dist, CoordType far_dist, CoordType fov_y);

    const Vec3& GetFocalPoint() const;
    CoordType GetFOV() const;
    CoordType GetNearDist() const;
    CoordType GetFarDist() const;
    void Move(const Vec3& shift);
    void Rotate(Axis axis, CoordType angle);
    Mat4 GetWorldToCameraMatrix() const;
    Vec3 GetRightDirecton() const;
    Vec3 GetUpDirection() const;
    Vec3 GetForwardDirection() const;
    const Mat4& GetRotationMatrix() const;

private:
    static constexpr Vec3 kDefaultFocalPoint = {0, 0, 0};
    static constexpr CoordType kDefaultNearDist = 0.1;
    static constexpr CoordType kDefaultFarDist = 1000.0;
    static constexpr CoordType kDefaultFOV = 45.0;
    static constexpr Vec3 kDefaultPosition = Vec3{0, 0, 0};
    static constexpr Mat3 kDefaultRotation = Mat4{1.};

    Vec3 focal_point_ = kDefaultFocalPoint;
    CoordType near_dist_ = kDefaultNearDist;
    CoordType far_dist_ = kDefaultFarDist;
    CoordType fov_y_ = kDefaultFOV;
    Vec3 position_ = kDefaultPosition;
    Mat4 rotation_ = kDefaultRotation;
};

}  // namespace renderer
