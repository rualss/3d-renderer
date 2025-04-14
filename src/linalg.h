#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <cstdint>

namespace renderer {
using Vec2 = glm::dvec2;
using Vec3 = glm::dvec3;
using Vec4 = glm::dvec4;
using Mat3 = glm::dmat3;
using Mat4 = glm::dmat4;
using Quaternion = glm::dquat;
using CoordType = double;
using Color = glm::vec3;
using DiscreteColor = glm::vec<3, int32_t>;
using Index = int32_t;

const static CoordType kEps = 1e-6;

template <class Tag>
struct TypedIntAlias {
    template <typename T>
        requires(std::is_convertible_v<T, Index>)
    explicit TypedIntAlias(T value) : value_(static_cast<Index>(value)) {
    }

    operator Index() const {
        return value_;
    }

private:
    Index value_;
};

enum class Axis { X, Y, Z };

using Width = TypedIntAlias<struct width_tag>;
using Height = TypedIntAlias<struct height_tag>;

namespace color {

constexpr static Index kDiscreteColorMax = 255;

constexpr static DiscreteColor kBlackDiscrete = {0, 0, 0};
constexpr static DiscreteColor kWhiteDiscrete = {255, 255, 255};
constexpr static DiscreteColor kRedDiscrete = {255, 0, 0};
constexpr static DiscreteColor kGreenDiscrete = {0, 255, 0};
constexpr static DiscreteColor kBlueDiscrete = {0, 0, 255};

constexpr static Color kBlack = {0, 0, 0};
constexpr static Color kWhite = {1, 1, 1};
constexpr static Color kRed = {1, 0, 0};
constexpr static Color kGreen = {0, 1, 0};
constexpr static Color kBlue = {0, 0, 1};

}  // namespace color
}  // namespace renderer
