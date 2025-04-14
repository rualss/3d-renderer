#include "texture.h"
#include <cassert>
#include <memory>
#include "linalg.h"
#include "picture.h"

namespace renderer {
Texture::Texture() {
    unsigned char data[3] = {255, 255, 255};
    impl_ = std::make_shared<const Picture>(Height{1}, Width{1}, data);
}

Texture::Texture(Height height, Width width, const unsigned char* data) {
    impl_ = std::make_shared<const Picture>(height, width, data);
}

Texture::Texture(const Texture& texture) = default;
Texture::Texture(Texture&& texture) = default;
Texture::~Texture() = default;
Texture& Texture::operator=(const Texture& other) = default;
Texture& Texture::operator=(Texture&& other) = default;

const Picture& Texture::operator*() const {
    return *impl_;
}

const Picture* Texture::operator->() const {
    return impl_.get();
}

const DiscreteColor& Texture::SampleColor(Vec2 coords) const {
    assert(0 <= coords.x <= 1 && 0 <= coords.y <= 1 && "Texture coordinates must be in [0, 1]");

    return (*impl_)(static_cast<Index>(std::round(coords.x * (impl_->GetWidth() - 1))),
                    static_cast<Index>(std::round(coords.y * (impl_->GetHeight() - 1))));
}

}  // namespace renderer
