/**
 * @file main.cpp
 * @brief Demo: C++23 `<print>`, `std::format`, and a user-defined formatter.
 *
 * `std::println` writes a line with `{}` placeholders just like `std::format`.
 * To make a custom type first-class inside those placeholders you specialize
 * `std::formatter<T>`: `parse` reads an optional spec (here, `:x` for hex),
 * and `format` writes the output to the context iterator.
 *
 * Build: `cmake --build <build-dir> --target print_demo`
 * Run:   `<build-dir>/src/print_demo/print_demo`
 */

#include <cstdint>
#include <format>
#include <print>
#include <string>

struct Color {
  std::uint8_t r{};
  std::uint8_t g{};
  std::uint8_t b{};
};

template <>
struct std::formatter<Color> {
  bool as_hex = false;

  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();
    if (it != ctx.end() && *it == 'x') {
      as_hex = true;
      ++it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error{"Color: only ':x' (hex) is supported"};
    }
    return it;
  }

  auto format(const Color& c, std::format_context& ctx) const {
    return as_hex
        ? std::format_to(ctx.out(), "#{:02x}{:02x}{:02x}", c.r, c.g, c.b)
        : std::format_to(ctx.out(), "rgb({}, {}, {})", c.r, c.g, c.b);
  }
};

int main() {
  std::println("Hello from print_demo");
  std::println("2 + 3 = {}", 2 + 3);

  const std::string detail =
      std::format("hex 255 = {:#x}, padded = {:04}", 255, 7);
  std::println("{}", detail);

  constexpr Color teal{0, 128, 128};
  std::println("default spec: {}", teal);
  std::println("hex spec:     {:x}", teal);

  return 0;
}
