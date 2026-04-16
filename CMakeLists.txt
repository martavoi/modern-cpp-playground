cmake_minimum_required(VERSION 3.25)
project(modern_cpp_playground LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/ranges_demo)
add_subdirectory(src/concepts_demo)
add_subdirectory(src/cpp26_demo)
add_subdirectory(src/print_demo)
add_subdirectory(src/spaceship_demo)
add_subdirectory(src/constexpr_demo)
add_subdirectory(src/jthread_demo)
