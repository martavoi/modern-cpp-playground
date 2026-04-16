/**
 * @file main.cpp
 * @brief Demo: C++23 `<print>` and `std::format`.
 *
 * `std::println` writes a line including the newline, using `{}` placeholders
 * like `std::format`. `std::format` returns a `std::string` (or compatible)
 * with full format-specifier support (e.g. radix and width), which you can
 * then print or store.
 *
 * Build: `cmake --build <build-dir> --target print_demo`
 * Run:   `<build-dir>/src/print_demo/print_demo`
 */

#include <format>
#include <print>
#include <string>

int main() {
  std::println("Hello from print_demo");
  std::println("2 + 3 = {}", 2 + 3);

  const std::string detail =
      std::format("hex 255 = {:#x}, padded = {:04}", 255, 7);
  std::println("{}", detail);

  return 0;
}
