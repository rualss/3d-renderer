project(3d-renderer)

include(sources.cmake)

find_package(glm CONFIG REQUIRED)
target_link_libraries(3d_renderer PRIVATE glm::glm)
