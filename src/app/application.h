#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "camera.h"
#include "linalg.h"
#include "picture.h"
#include "renderer.h"
#include "world.h"
#include "timer.h"

namespace application {

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

    static constexpr renderer::CoordType kDefaultMovementSpeed = 50.;
    static constexpr renderer::CoordType kDefaultRotationSpeed = 80.;
    static constexpr Index kDefaultHeight = 720;
    static constexpr Index kDefaultWidth = 1280;
    static constexpr std::string kDefaultName = "3D renderer";

    Index height_ = kDefaultHeight;
    Index width_ = kDefaultWidth;
    Window window_;
    renderer::Renderer renderer_;
    renderer::Camera camera_;
    renderer::World world_;
    renderer::Picture picture_;
    renderer::CoordType movement_speed_ = kDefaultMovementSpeed;
    renderer::CoordType rotation_speed_ = kDefaultRotationSpeed;
    std::vector<sf::Vertex> pixels_;
    Timer timer_;
};

}  // namespace application
