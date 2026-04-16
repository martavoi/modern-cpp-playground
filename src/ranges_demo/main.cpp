/**
 * @file main.cpp
 * @brief Demo: C++20 ranges pipeline, plus C++23 `ranges::to` and `fold_left`.
 *
 * A pipeline of `views::` adaptors is lazy — nothing happens until it is
 * iterated. C++23 then closes the loop:
 *   - `std::ranges::to<Container>()` materializes a view into a container,
 *   - `std::ranges::fold_left` reduces a range with a binary op (modern
 *     spelling of `std::accumulate`).
 * Range formatting with `std::println("{}", v)` comes from `<format>` in C++23.
 *
 * Build: `cmake --build <build-dir> --target ranges_demo`
 * Run:   `<build-dir>/src/ranges_demo/ranges_demo`
 */

#include <algorithm>
#include <functional>
#include <print>
#include <ranges>
#include <vector>

int main() {
  constexpr auto is_even = [](int x) { return x % 2 == 0; };
  constexpr auto square = [](int x) { return x * x; };

  const std::vector<int> values{1, 2, 3, 4, 5, 6};

  auto even_squares = values | std::views::filter(is_even)
                             | std::views::transform(square);

  std::println("Even squares (lazy view): {}", even_squares);

  const auto collected = std::views::iota(1, 6)
                       | std::views::transform(square)
                       | std::ranges::to<std::vector>();
  std::println("Squares 1..5 collected:   {}", collected);

  const int sum = std::ranges::fold_left(even_squares, 0, std::plus{});
  std::println("Sum of even squares:      {}", sum);

  return 0;
}
