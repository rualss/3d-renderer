project(3d-renderer)

include(sources.cmake)

add_subdirectory(app)

include(FetchContent)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

target_link_libraries(3d_pipeline_lib PUBLIC glm::glm assimp::assimp)
