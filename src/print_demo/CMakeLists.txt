add_executable(print_demo main.cpp)
target_compile_features(print_demo PRIVATE cxx_std_23)
set_target_properties(print_demo PROPERTIES CXX_EXTENSIONS OFF)
