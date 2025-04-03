// #include "src/renderer.h"
#include "src/linalg.h"
#include <SFML/Graphics/Color.hpp>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "src/linalg.h"
#include "src/picture.h"
#include "src/polygon.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext.hpp"
#include "src/camera.h"
#include "src/renderer.h"
#include "src/world.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <glm/trigonometric.hpp>
#include <iostream>
#include <optional>
#include <ostream>

using namespace renderer;

// fix default color
// fix perpendicular

int main() {
    std::freopen("/home/rualss/3d-renderer/debug_build/res.txt", "w", stdout);
    Camera camera;
    World world;

    Mat4 perspective = glm::perspective(glm::radians(45.), 4. / 3., 0.1, 100.);

    Vec3 a(10, 0, -60);
    Vec3 b(5, 10, -40);
    Vec3 c(-5, -5, -30);

    Vec3 e(-15, 10, -40);
    Vec3 f(15, -5, -50);
    Vec3 g(10, -10, -35);

    Polygon p1({e, f, g});

    Polygon p({a, b, c});
    p.SetColor({255, 15, 0});
    p1.SetColor({0, 0, 255});
    world.AddPolygon(p);
    world.AddPolygon(p1);
    Renderer renderer;
    Picture picture = renderer.Render(world, camera, Height{600}, Width{800});

    sf::RenderWindow window(sf::VideoMode({800, 600}), "3D Renderer");
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        size_t height = 600;
        size_t width = 800;
        std::vector<sf::Vertex> pixels;
        pixels.reserve(height * width);
        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                Color color = picture.GetPixel(x, y);
                if (color != kBlack) {
                    sf::Color sf_color(picture.GetPixel(x, y).x, picture.GetPixel(x, y).y,
                                       picture.GetPixel(x, y).z, 1);
                    pixels.push_back(sf::Vertex{sf::Vector2f(x, y), sf_color});
                }
            }
        }
        window.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);

        // Update the window
        window.display();
    }
}
