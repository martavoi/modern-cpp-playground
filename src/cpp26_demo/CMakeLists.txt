add_executable(cpp26_demo main.cpp)
set_target_properties(
  cpp26_demo
  PROPERTIES CXX_STANDARD 26
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS OFF)
