project(3d-renderer)

include(sources.cmake)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

find_package(glm CONFIG REQUIRED)
target_link_libraries(3d_renderer PRIVATE glm::glm sfml-graphics)
