project(3d-renderer)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
    renderer
    main.cpp
    application.cpp
    timer.cpp
)

target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(renderer PRIVATE 3d_pipeline_lib SFML::Graphics)
