#include <exception>
#include <iostream>

#include "application.h"

using namespace renderer;

int main() {
    try {
        Application app;
        app.Run();
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
    }
}
