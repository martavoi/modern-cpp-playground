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
