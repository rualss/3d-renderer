#pragma once

#include "texture.h"
#include <filesystem>

namespace renderer {

// я не справился написать pimpl :(

class TextureLoader {
public:
    Texture LoadTexture(std::filesystem::path path);
    bool HasTexture(std::filesystem::path path) const;

private:
    std::unordered_map<std::filesystem::path, Texture> loaded_textures_;
};

}  // namespace renderer
