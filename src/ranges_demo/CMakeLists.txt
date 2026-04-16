add_executable(ranges_demo main.cpp)
target_compile_features(ranges_demo PRIVATE cxx_std_23)
set_target_properties(ranges_demo PROPERTIES CXX_EXTENSIONS OFF)
