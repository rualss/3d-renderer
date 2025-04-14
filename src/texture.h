#pragma once

#include <memory>
#include "linalg.h"
#include "picture.h"

namespace renderer {

class Texture {
public:
    Texture();
    Texture(Height height, Width width, unsigned char* data);
    Texture(const Texture& texture);
    Texture(Texture&& texture);
    ~Texture();

    Texture& operator=(const Texture& other);
    Texture& operator=(Texture&& other);
    const Picture& operator*() const;
    const Picture* operator->() const;

    Color SampleColor(Vec2 coords);

private:
    std::shared_ptr<const Picture> impl_;
};

}  // namespace renderer
