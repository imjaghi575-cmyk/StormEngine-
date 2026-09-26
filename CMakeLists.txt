cmake_minimum_required(VERSION 3.16)

project(StormEngine
    VERSION 0.1.0
    DESCRIPTION "A small modular game engine for Termux and Android"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STORM_BUILD_DEMO "Build the Storm Engine demos" ON)
option(STORM_BUILD_TESTS "Build Storm Engine tests" ON)

add_library(storm_core
    src/storm/core.cpp
    src/storm/software_renderer.cpp
    src/storm/headless_platform.cpp
)

target_include_directories(storm_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(storm_core PUBLIC cxx_std_17)

if(STORM_BUILD_DEMO)
    add_executable(storm_demo examples/hello_world.cpp)
    target_link_libraries(storm_demo PRIVATE storm_core)

    add_executable(storm_renderer_demo examples/renderer_demo.cpp)
    target_link_libraries(storm_renderer_demo PRIVATE storm_core)
endif()

if(STORM_BUILD_TESTS)
    enable_testing()

    add_executable(storm_core_tests tests/core_tests.cpp)
    target_link_libraries(storm_core_tests PRIVATE storm_core)
    add_test(NAME storm_core_tests COMMAND storm_core_tests)

    add_executable(storm_platform_tests tests/platform_tests.cpp)
    target_link_libraries(storm_platform_tests PRIVATE storm_core)
    add_test(NAME storm_platform_tests COMMAND storm_platform_tests)
endif()
