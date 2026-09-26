cmake_minimum_required(VERSION 3.23)
project(StormEnginePackageConsumer LANGUAGES CXX)

find_package(StormEngine 0.1 CONFIG REQUIRED)

add_executable(storm_package_consumer main.cpp)
target_link_libraries(storm_package_consumer PRIVATE StormEngine::storm_core)
target_compile_features(storm_package_consumer PRIVATE cxx_std_17)
