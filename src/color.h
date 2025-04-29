#pragma once

#include "linalg.h"

namespace renderer {

using ColorValue = float;
using Color = glm::vec3;
using DiscreteColor = glm::vec<3, int32_t>;

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

DiscreteColor ColorToDiscrete(const Color& color);
Color DiscreteColorToColor(const DiscreteColor& discrete_color);

}  // namespace renderer
