#pragma once

#include <memory>
#include "linalg.h"
#include "picture.h"
#include "color.h"

namespace renderer {

class Texture {
public:
    Texture();
    Texture(const Picture& picture);
    Texture(Picture&& picture);

    static Texture From(const Picture& picture);
    Texture From(Picture&& picture);
    const Picture& operator*() const;
    const Picture* operator->() const;

    const DiscreteColor& SampleColor(Vec2 coords) const;

private:
    std::shared_ptr<const Picture> impl_;
};

}  // namespace renderer
