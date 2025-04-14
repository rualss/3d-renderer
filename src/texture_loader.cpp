#include "texture_loader.h"
#include <filesystem>
#include <unordered_map>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace renderer {

Texture TextureLoader::LoadTexture(std::filesystem::path path) {
    if (HasTexture(path)) {
        return loaded_textures_[path];
    }
    Index width;
    Index height;
    Index channel_count;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channel_count, 3);
    if (data == nullptr) {
        return Texture{};
    }
    Texture texture{Height{height}, Width{width}, data};
    stbi_image_free(data);
    loaded_textures_[path] = texture;
    return texture;
}

bool TextureLoader::HasTexture(std::filesystem::path path) const {
    return loaded_textures_.contains(path);
}

}  // namespace renderer
