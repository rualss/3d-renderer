#include "texture_loader.h"
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "linalg.h"
#include "picture.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace renderer {

namespace {

class STBLoader {
public:
    explicit STBLoader(std::filesystem::path path) {
        data_ = stbi_load(path.c_str(), &width_, &height_, &channel_count_, 3);
    }

    ~STBLoader() {
        stbi_image_free(data_);
    }

    Index GetSize() {
        return height_ * width_;
    }

    const unsigned char *GetData() {
        return data_;
    }

    Index GetWidth() {
        return width_;
    }

private:
    Index width_;
    Index height_;
    Index channel_count_;
    unsigned char *data_;
};

}  // namespace

Texture TextureLoader::LoadTexture(std::filesystem::path path) {
    if (HasTexture(path)) {
        return loaded_textures_[path];
    }
    STBLoader loader(path);
    if (loader.GetData() == nullptr) {
        return Texture{};
    }
    std::vector<DiscreteColor> pixels;
    pixels.reserve(loader.GetSize());
    for (Index i = 0; i < loader.GetSize(); ++i) {
        Index data_index = i * 3;
        pixels.emplace_back(loader.GetData()[data_index], loader.GetData()[data_index + 1],
                            loader.GetData()[data_index + 2]);
    }
    Picture loaded_picture{Width{loader.GetWidth()}, std::move(pixels)};
    Texture texture{std::move(loaded_picture)};
    loaded_textures_[path] = texture;
    return texture;
}

bool TextureLoader::HasTexture(std::filesystem::path path) const {
    return loaded_textures_.contains(path);
}

}  // namespace renderer
