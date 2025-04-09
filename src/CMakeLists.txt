project(3d-renderer)

include(sources.cmake)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(glm CONFIG REQUIRED)
target_link_libraries(3d_renderer PUBLIC glm::glm SFML::Graphics)
