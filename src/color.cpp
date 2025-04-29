#include "color.h"
#include <algorithm>
#include "glm/common.hpp"

namespace renderer {

DiscreteColor ColorToDiscrete(const Color& color) {
    DiscreteColor discrete_color;
    discrete_color = glm::round(color * static_cast<ColorValue>(kDiscreteColorMax));
    discrete_color = glm::clamp(discrete_color, 0, kDiscreteColorMax);
    return discrete_color;
}

Color DiscreteColorToColor(const DiscreteColor& discrete_color) {
    Color color{discrete_color};
    color /= 255.;
    color = glm::clamp(color, 0.0f, 1.0f);
    return color;
}

}  // namespace renderer
