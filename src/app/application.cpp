#include "application.h"
#include <SFML/Window/Keyboard.hpp>
#include "camera.h"
#include "light.h"
#include "linalg.h"
#include "picture.h"
#include "world.h"
#include <glm/ext/matrix_transform.hpp>

namespace application {

Application::Application()
    : renderer_(),
      camera_(),
      world_(),
      light_(),
      window_(sf::VideoMode({width_, height_}), "3D Renderer") {
    renderer::Mesh cat("../data/cat.obj");
    cat.SetColorsRandomly();
    cat.SetLocalOrigin({0, -12, -20});
    cat.ApplyMatrix(glm::scale(renderer::Mat4(1.), {0.1, 0.1, 0.1}));
    world_.AddMesh(cat);
}
void Application::Run() {
    while (window_.isOpen()) {
        timer_.Tick();
        window_.clear();
        HandleEvents();
        HandleKeyboard();
        RenderFrame();
    }
}

void Application::HandleEvents() {
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
    }
}

void Application::HandleKeyboard() {
    renderer::CoordType move_distance = movement_speed_ * timer_.GetDelta();
    renderer::CoordType rotate_angle = rotation_speed_ * timer_.GetDelta();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        camera_.Move(camera_.GetForwardDirection() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        camera_.Move(-camera_.GetRightDirecton() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        camera_.Move(-camera_.GetForwardDirection() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        camera_.Move(camera_.GetRightDirecton() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        camera_.Move(camera_.GetUpDirection() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        camera_.Move(-camera_.GetUpDirection() * move_distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        camera_.Rotate(renderer::Axis::X, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        camera_.Rotate(renderer::Axis::X, -rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        camera_.Rotate(renderer::Axis::Y, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        camera_.Rotate(renderer::Axis::Y, -rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        camera_.Rotate(renderer::Axis::Z, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        camera_.Rotate(renderer::Axis::Z, -rotate_angle);
    }
}

void Application::RenderFrame() {
    renderer::Picture picture = renderer_.Render(world_, camera_, light_, renderer::Height{height_},
                                                 renderer::Width{width_});
    std::vector<sf::Vertex> pixels;
    pixels.reserve(height_ * width_);
    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            renderer::Color color = picture.GetPixel(x, y);
            if (color != renderer::kBlack) {
                pixels.push_back(
                    sf::Vertex{sf::Vector2f(x, y), sf::Color(color.x, color.y, color.z)});
            }
        }
    }
    window_.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);
    window_.display();
}

}  // namespace application
