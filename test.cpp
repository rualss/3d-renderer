#include "src/picture.h"
#include "src/polygon.h"
#include "src/renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

using namespace renderer;

Picture GetPic(const Mesh& cat) {
    Camera camera;
    World world;
    world.AddMesh(cat);
    Renderer renderer;
    Index height = 600;
    Index width = 800;
    return renderer.Render(world, camera, Height{600}, Width{800});
}

int main() {
    Mesh cat("../data/cat.obj");
    cat.SetColorsRandomly();
    cat.SetLocalOrigin({0, -12, -20});
    cat.ApplyMatrix(glm::scale(Mat4(1.), {0.05, 0.05, 0.05}));
    // Mesh cat;
    // cat.AddPolygon(Polygon({0, 0, -0.05}, {0.25, 1, -10}, {-0.25, 1, -10}, {255, 0, 0}));
    Index height = 600;
    Index width = 800;

    sf::RenderWindow window(sf::VideoMode({width, height}), "3D Renderer");
    while (window.isOpen()) {
        window.clear();
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        Picture picture = GetPic(cat);

        std::vector<sf::Vertex> pixels;
        pixels.reserve(height * width);
        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                Color color = picture.GetPixel(x, y);
                if (color != kBlack) {
                    pixels.push_back(
                        sf::Vertex{sf::Vector2f(x, y), sf::Color(color.x, color.y, color.z)});
                }
            }
        }
        window.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);

        window.display();

        cat.ApplyMatrix(glm::rotate(Mat4(1.), glm::radians(2.), {0, 1, 0}));
    }
}
