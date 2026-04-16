/**
 * @file main.cpp
 * @brief Demo: defaulted three-way comparison (`operator<=>`, C++20).
 *
 * `auto operator<=>(...) const = default` asks the compiler to synthesize
 * `<=>` and the relational operators (`==`, `<`, etc.) from member-wise
 * comparison. Lexicographic order on `{major, minor}` matches intuitive
 * version ordering.
 *
 * Build: `cmake --build <build-dir> --target spaceship_demo`
 * Run:   `<build-dir>/src/spaceship_demo/spaceship_demo`
 */

#include <compare>
#include <iostream>

/** Version tuple compared lexicographically by `major` then `minor`. */
struct Version {
  int major{};
  int minor{};
  /** Member-wise three-way comparison; generates `==`, `<`, etc. */
  auto operator<=>(const Version&) const = default;
};

int main() {
  static_assert((Version{1, 2} <=> Version{1, 3}) == std::strong_ordering::less);
  static_assert((Version{2, 0} <=> Version{1, 9}) == std::strong_ordering::greater);
  static_assert((Version{1, 0} <=> Version{1, 0}) == std::strong_ordering::equivalent);

  const Version a{1, 10};
  const Version b{2, 0};
  std::cout << "Version 1.10 vs 2.0: ";
  if (a < b) {
    std::cout << "less\n";
  } else {
    std::cout << "not less\n";
  }
  return 0;
}
