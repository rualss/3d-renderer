#pragma once

#include "texture.h"
#include <filesystem>
#include <memory>

namespace renderer {

namespace impl {

struct TextureLoaderImpl;

}

class TextureLoader {
public:
    Texture LoadTexture(std::filesystem::path path);
    bool HasTexture(std::filesystem::path path) const;

private:
    std::unique_ptr<impl::TextureLoaderImpl> impl_;
};

}  // namespace renderer
