#include "application.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "camera.h"
#include "light.h"
#include "linalg.h"
#include "material.h"
#include "model_loader.h"
#include "object_3d.h"
#include "picture.h"
#include "polygon.h"
#include "world.h"
#include <SFML/Window/VideoMode.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace renderer {

namespace {

World ExampleScene() {
    ModelLoader loader;
    World world;
    loader.Open("../data/cat/12221_Cat_v1_l3.obj");
    Object3D floppa = loader.GetObject();
    PointLight light;
    light.quadratic_attenuation = 0;
    light.linear_attenuation = 0;
    light.position = {0, 0, 100};
    world.AddLight(light);
    world.AddObject(floppa);
    return world;
}

}  // namespace

Application::Application()
    : world_(ExampleScene()),
      window_(sf::VideoMode({static_cast<Index>(picture_.GetWidth()),
                             static_cast<Index>(picture_.GetHeight())}),
              kDefaultName) {
}

void Application::Run() {
    while (window_.isOpen()) {
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
    Time elapsed = timer_.Elapsed();
    CoordType move_distance = movement_speed_ * elapsed.ToSeconds();
    CoordType rotate_angle = rotation_speed_ * elapsed.ToSeconds();
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
        camera_.Rotate(Axis::X, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        camera_.Rotate(Axis::X, -rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        camera_.Rotate(Axis::Y, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        camera_.Rotate(Axis::Y, -rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        camera_.Rotate(Axis::Z, rotate_angle);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        camera_.Rotate(Axis::Z, -rotate_angle);
    }
}

void Application::RenderFrame() {
    renderer_.Render(world_, camera_, &picture_);
    pixels_.clear();
    for (size_t x = 0; x < picture_.GetWidth(); ++x) {
        for (size_t y = 0; y < picture_.GetHeight(); ++y) {
            DiscreteColor color = picture_(x, y);
            if (color != kBlackDiscrete) {
                pixels_.emplace_back(sf::Vector2f(x, y), sf::Color(color.x, color.y, color.z));
            }
        }
    }
    window_.draw(pixels_.data(), pixels_.size(), sf::PrimitiveType::Points);
    window_.display();
}

}  // namespace renderer
