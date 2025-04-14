#pragma once

#include <optional>
#include "linalg.h"
#include "texture.h"
namespace renderer {

struct Material {
    Color ambient = color::kWhite;
    Color diffuse = color::kWhite;
    Color specular = color::kBlack;
    CoordType shininess = 2;
    bool two_sided = false;
    std::optional<Texture> ambient_texture = std::nullopt;
    std::optional<Texture> diffuse_texture = std::nullopt;
    std::optional<Texture> specular_texture = std::nullopt;
};

}  // namespace renderer
