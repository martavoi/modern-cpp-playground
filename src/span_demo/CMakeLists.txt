add_executable(span_demo main.cpp)
target_compile_features(span_demo PRIVATE cxx_std_23)
set_target_properties(span_demo PROPERTIES CXX_EXTENSIONS OFF)
