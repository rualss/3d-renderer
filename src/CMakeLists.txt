project(3d-renderer)

include(sources.cmake)

add_subdirectory(app)

find_package(glm CONFIG REQUIRED)
target_link_libraries(3d_pipeline_lib PUBLIC glm::glm)
