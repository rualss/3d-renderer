#include "texture_loader.h"
#include <filesystem>
#include <unordered_map>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace renderer {

namespace impl {

struct TextureLoaderImpl {
    Texture LoadTexture(std::filesystem::path path) {
        if (HasTexture(path)) {
            return loaded_textures[path];
        }
        Index width;
        Index height;
        Index channel_count;
        unsigned char *data = stbi_load("test.bmp", &width, &height, &channel_count, 3);
        if (data == nullptr) {
            return Texture{};
        }
        Texture texture{Height{height}, Width{width}, data};
        stbi_image_free(data);
        return texture;
    }

    bool HasTexture(std::filesystem::path path) const {
        return loaded_textures.contains(path);
    }

    std::unordered_map<std::filesystem::path, Texture> loaded_textures;
};

}  // namespace impl

Texture TextureLoader::LoadTexture(std::filesystem::path path) {
    return impl_->LoadTexture(path);
}

bool TextureLoader::HasTexture(std::filesystem::path path) const {
    return impl_->HasTexture(path);
}

}  // namespace renderer
