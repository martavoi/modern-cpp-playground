/**
 * @file main.cpp
 * @brief Demo: defaulted three-way comparison (`operator<=>`, C++20).
 *
 * `auto operator<=>(...) const = default` asks the compiler to synthesize
 * the three-way comparison and the relational operators (`==`, `<`, etc.)
 * from member-wise comparison. Lexicographic ordering on `{major, minor}`
 * matches intuitive version ordering for free.
 *
 * Build: `cmake --build <build-dir> --target spaceship_demo`
 * Run:   `<build-dir>/src/spaceship_demo/spaceship_demo`
 */

#include <compare>
#include <print>

struct Version {
  int major{};
  int minor{};

  auto operator<=>(const Version&) const = default;
};

int main() {
  static_assert((Version{1, 2} <=> Version{1, 3}) == std::strong_ordering::less);
  static_assert((Version{2, 0} <=> Version{1, 9}) == std::strong_ordering::greater);
  static_assert(Version{1, 0} == Version{1, 0});

  constexpr Version a{1, 10};
  constexpr Version b{2, 0};
  std::println("Version 1.10 vs 2.0: {}", a < b ? "less" : "not less");
  return 0;
}
