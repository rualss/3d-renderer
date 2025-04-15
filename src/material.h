#pragma once

#include <optional>
#include "linalg.h"
#include "texture.h"
#include "color.h"

namespace renderer {

struct Material {
    Color ambient = kWhite;
    Color diffuse = kWhite;
    Color specular = kBlack;
    CoordType shininess = 32;

    bool two_sided = false;
    std::optional<Texture> ambient_texture = std::nullopt;
    std::optional<Texture> diffuse_texture = std::nullopt;
    std::optional<Texture> specular_texture = std::nullopt;
};

}  // namespace renderer
