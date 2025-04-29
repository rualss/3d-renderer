#include "texture.h"
#include <cassert>
#include <memory>
#include <vector>
#include "alias.h"
#include "color.h"
#include "linalg.h"
#include "picture.h"

namespace renderer {

Texture::Texture() {
    std::vector<DiscreteColor> pixel(1, kWhite);
    impl_ = std::make_shared<const Picture>(Width{1}, std::move(pixel));
}

Texture::Texture(const Picture& picture) : impl_(std::make_shared<const Picture>(picture)) {
}

Texture::Texture(Picture&& picture) : impl_(std::make_shared<const Picture>(std::move(picture))) {
}

Texture Texture::From(const Picture& picture) {
    return Texture(picture);
}

Texture Texture::From(Picture&& picture) {
    return Texture(std::move(picture));
}

const Picture& Texture::operator*() const {
    return *impl_;
}

const Picture* Texture::operator->() const {
    return impl_.get();
}

const DiscreteColor& Texture::SampleColor(Vec2 coords) const {
    assert(0 <= coords.x <= 1 && 0 <= coords.y <= 1 && "Texture coordinates must be in [0, 1]");
    return impl_->SampleColor(coords);
}

}  // namespace renderer
