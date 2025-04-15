#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "camera.h"
#include "linalg.h"
#include "picture.h"
#include "renderer.h"
#include "world.h"
#include "timer.h"

namespace renderer {

class Application {
    using Window = sf::RenderWindow;
    using Index = uint32_t;  // SFML default size type, there is no sf::size_type

public:
    Application();
    void Run();

private:
    void HandleEvents();
    void HandleKeyboard();
    void RenderFrame();

    static constexpr CoordType kDefaultMovementSpeed = 50.;
    static constexpr CoordType kDefaultRotationSpeed = 80.;
    static constexpr std::string kDefaultName = "3D renderer";

    Renderer renderer_;
    Camera camera_;
    World world_;
    Picture picture_;
    Window window_;
    CoordType movement_speed_ = kDefaultMovementSpeed;
    CoordType rotation_speed_ = kDefaultRotationSpeed;
    std::vector<sf::Vertex> pixels_;
    Timer timer_;
};

}  // namespace renderer
