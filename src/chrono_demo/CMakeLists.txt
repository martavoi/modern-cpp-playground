add_executable(chrono_demo main.cpp)
target_compile_features(chrono_demo PRIVATE cxx_std_23)
set_target_properties(chrono_demo PROPERTIES CXX_EXTENSIONS OFF)
