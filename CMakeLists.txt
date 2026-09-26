cmake_minimum_required(VERSION 3.23)

project(StormEngine
    VERSION 0.1.0
    DESCRIPTION "A small modular game engine for Android and Termux"
    LANGUAGES CXX
)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STORM_BUILD_DEMO "Build the Storm Engine demos" ON)
option(STORM_BUILD_TESTS "Build Storm Engine tests" ${BUILD_TESTING})
option(STORM_ENABLE_WARNINGS "Enable compiler warnings for Storm targets" ON)

add_library(storm_core
    src/storm/core.cpp
    src/storm/time.cpp
    src/storm/software_renderer.cpp
    src/storm/headless_platform.cpp
)

target_include_directories(storm_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(storm_core PUBLIC cxx_std_17)

if(STORM_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(storm_core PRIVATE /W4)
    else()
        target_compile_options(storm_core PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

if(STORM_BUILD_DEMO)
    add_executable(storm_demo examples/hello_world.cpp)
    target_link_libraries(storm_demo PRIVATE storm_core)

    add_executable(storm_renderer_demo examples/renderer_demo.cpp)
    target_link_libraries(storm_renderer_demo PRIVATE storm_core)
endif()

if(BUILD_TESTING AND STORM_BUILD_TESTS)
    add_executable(storm_core_tests tests/core_tests.cpp)
    target_link_libraries(storm_core_tests PRIVATE storm_core)
    add_test(NAME storm_core_tests COMMAND storm_core_tests)

    add_executable(storm_platform_tests tests/platform_tests.cpp)
    target_link_libraries(storm_platform_tests PRIVATE storm_core)
    add_test(NAME storm_platform_tests COMMAND storm_platform_tests)

    add_executable(storm_renderer_tests tests/renderer_tests.cpp)
    target_link_libraries(storm_renderer_tests PRIVATE storm_core)
    add_test(NAME storm_renderer_tests COMMAND storm_renderer_tests)

    add_executable(storm_time_tests tests/time_tests.cpp)
    target_link_libraries(storm_time_tests PRIVATE storm_core)
    add_test(NAME storm_time_tests COMMAND storm_time_tests)
endif()
