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

Vec3 TransformVector(const Mat4& transformation_matrix, const Vec3& vector) {
    Vec4 homogeneous(vector, 1.0);
    homogeneous = transformation_matrix * homogeneous;
    assert(std::abs(homogeneous.w) > kEps && "TransformVector: Point went to infinity");
    return Vec3(homogeneous.x, homogeneous.y, homogeneous.z) / homogeneous.w;
}

Polygon TransformPolygon(const Mat4& transformation_matrix,
                         const Polygon& polygon) {  // TODO maybe inplace will be faster
    std::array<Vec3, 3> transformed_vertices = polygon.GetVertices();
    for (size_t i = 0; i < 3; ++i) {
        transformed_vertices[i] = TransformVector(transformation_matrix, transformed_vertices[i]);
    }
    Polygon transformed_polygon(transformed_vertices);
    transformed_polygon.SetColor(polygon.GetColor());
    return transformed_polygon;
}

Picture GetPic(int iters) {
    Camera camera;
    World world;

    // Vec3 a(10, 0, -60);
    // Vec3 b(5, 10, -40);
    // Vec3 c(-5, -5, -30);

    // Vec3 e(-15, 10, -40);
    // Vec3 f(15, -5, -50);
    // Vec3 g(10, -10, -35);

    // Vec3 e(-15, 10, -40);
    // Vec3 f(5, 10, -40);
    // Vec3 g(-5, -5, -30);

    Vec3 a1(0, 0, 0);
    Vec3 a2(10, 0, 0);
    Vec3 a3(0, 10, 0);
    Vec3 a4(10, 10, 0);
    Vec3 a5(0, 0, -10);
    Vec3 a6(10, 0, -10);
    Vec3 a7(0, 10, -10);
    Vec3 a8(10, 10, -10);

    Polygon p1(a1, a2, a4);  // front
    Polygon p2(a1, a3, a4);

    Polygon p3(a1, a2, a6);  // down
    Polygon p4(a1, a5, a6);

    Polygon p5(a1, a3, a7);  // left
    Polygon p6(a1, a5, a7);

    Polygon p7(a4, a2, a6);  // right
    Polygon p8(a4, a8, a6);

    Polygon p9(a6, a8, a7);  // back
    Polygon p10(a6, a5, a7);

    Polygon p11(a4, a3, a7);  // up
    Polygon p12(a4, a8, a7);

    Mat4 translate = glm::translate(Mat4(1.), {0, -10, -50});
    Mat4 rotate1 = glm::rotate(translate, glm::radians(1. * iters), Vec3(0, 1, 0));
    Mat4 rotate = glm::rotate(rotate1, glm::radians(1. * iters), Vec3(1, 0, 0));

    p1 = TransformPolygon(rotate, p1);
    p2 = TransformPolygon(rotate, p2);
    p3 = TransformPolygon(rotate, p3);
    p4 = TransformPolygon(rotate, p4);
    p5 = TransformPolygon(rotate, p5);
    p6 = TransformPolygon(rotate, p6);
    p7 = TransformPolygon(rotate, p7);
    p8 = TransformPolygon(rotate, p8);
    p9 = TransformPolygon(rotate, p9);
    p10 = TransformPolygon(rotate, p10);
    p11 = TransformPolygon(rotate, p11);
    p12 = TransformPolygon(rotate, p12);

    p1.SetColor({255, 0, 0});
    p2.SetColor({0, 255, 0});
    p3.SetColor({0, 0, 255});
    p4.SetColor({255, 255, 0});
    p5.SetColor({255, 0, 255});
    p6.SetColor({0, 255, 255});
    p7.SetColor({255, 255, 255});
    p8.SetColor({255, 0, 0});
    p9.SetColor({0, 255, 0});
    p10.SetColor({0, 0, 255});
    p11.SetColor({255, 255, 0});
    p12.SetColor({255, 0, 255});

    // world.AddPolygon(p);
    world.AddPolygon(p1);
    world.AddPolygon(p2);
    world.AddPolygon(p3);
    world.AddPolygon(p4);
    world.AddPolygon(p5);
    world.AddPolygon(p6);
    world.AddPolygon(p7);
    world.AddPolygon(p8);
    world.AddPolygon(p9);
    world.AddPolygon(p10);
    world.AddPolygon(p11);
    world.AddPolygon(p12);
    Renderer renderer;
    Picture picture = renderer.Render(world, camera, Height{600}, Width{800});
    return picture;
}

int main() {
    // Camera camera;
    // World world;

    // // Vec3 a(10, 0, -60);
    // // Vec3 b(5, 10, -40);
    // // Vec3 c(-5, -5, -30);

    // // Vec3 e(-15, 10, -40);
    // // Vec3 f(15, -5, -50);
    // // Vec3 g(10, -10, -35);

    // // Vec3 e(-15, 10, -40);
    // // Vec3 f(5, 10, -40);
    // // Vec3 g(-5, -5, -30);

    // Vec3 a1(0, 0, 0);
    // Vec3 a2(10, 0, 0);
    // Vec3 a3(0, 10, 0);
    // Vec3 a4(10, 10, 0);
    // Vec3 a5(0, 0, -10);
    // Vec3 a6(10, 0, -10);
    // Vec3 a7(0, 10, -10);
    // Vec3 a8(10, 10, -10);

    // Polygon p1(a1, a2, a4);  // front
    // Polygon p2(a1, a3, a4);

    // Polygon p3(a1, a2, a6);  // down
    // Polygon p4(a1, a5, a6);

    // Polygon p5(a1, a3, a7);  // left
    // Polygon p6(a1, a5, a7);

    // Polygon p7(a4, a2, a6);  // right
    // Polygon p8(a4, a8, a6);

    // Polygon p9(a6, a8, a7);  // back
    // Polygon p10(a6, a5, a7);

    // Polygon p11(a4, a3, a7);  // up
    // Polygon p12(a4, a8, a7);

    // Mat4 translate = glm::translate(Mat4(1.), {0, -13, -50});
    // Mat4 rotate = glm::rotate(translate, glm::radians(30.), Vec3(0, 1, 0));

    // p1 = TransformPolygon(rotate, p1);
    // p2 = TransformPolygon(rotate, p2);
    // p3 = TransformPolygon(rotate, p3);
    // p4 = TransformPolygon(rotate, p4);
    // p5 = TransformPolygon(rotate, p5);
    // p6 = TransformPolygon(rotate, p6);
    // p7 = TransformPolygon(rotate, p7);
    // p8 = TransformPolygon(rotate, p8);
    // p9 = TransformPolygon(rotate, p9);
    // p10 = TransformPolygon(rotate, p10);
    // p11 = TransformPolygon(rotate, p11);
    // p12 = TransformPolygon(rotate, p12);

    // p1.SetColor({255, 0, 0});
    // p2.SetColor({0, 255, 0});
    // p3.SetColor({0, 0, 255});
    // p4.SetColor({255, 255, 0});
    // p5.SetColor({255, 0, 255});
    // p6.SetColor({0, 255, 255});
    // p7.SetColor({255, 255, 255});
    // p8.SetColor({255, 0, 0});
    // p9.SetColor({0, 255, 0});
    // p10.SetColor({0, 0, 255});
    // p11.SetColor({255, 255, 0});
    // p12.SetColor({255, 0, 255});

    // // world.AddPolygon(p);
    // world.AddPolygon(p1);
    // world.AddPolygon(p2);
    // world.AddPolygon(p3);
    // world.AddPolygon(p4);
    // world.AddPolygon(p5);
    // world.AddPolygon(p6);
    // world.AddPolygon(p7);
    // world.AddPolygon(p8);
    // world.AddPolygon(p9);
    // world.AddPolygon(p10);
    // world.AddPolygon(p11);
    // world.AddPolygon(p12);
    // Renderer renderer;
    // Picture picture = renderer.Render(world, camera, Height{600}, Width{800});
    int i = 1;
    sf::RenderWindow window(sf::VideoMode({800, 600}), "3D Renderer");
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        size_t height = 600;
        size_t width = 800;
        Picture picture = GetPic(i);
        std::vector<sf::Vertex> pixels;
        pixels.reserve(height * width);
        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                Color color = picture.GetPixel(x, y);
                if (color != kBlack) {
                    sf::Color sf_color(picture.GetPixel(x, y).x, picture.GetPixel(x, y).y,
                                       picture.GetPixel(x, y).z);
                    pixels.push_back(sf::Vertex{sf::Vector2f(x, y), sf_color});
                }
            }
        }
        window.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);

        // Update the window
        window.display();
        ++i;
    }
}
