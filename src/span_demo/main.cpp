/**
 * @file main.cpp
 * @brief Demo: `std::span` — non-owning view over contiguous elements.
 *
 * `std::span<const int>` carries a pointer and length and accepts any
 * contiguous source: `std::array`, a C array, or a `std::vector`. Callers
 * do not template on the container type and do not copy the sequence.
 *
 * Build: `cmake --build <build-dir> --target span_demo`
 * Run:   `<build-dir>/src/span_demo/span_demo`
 */

#include <array>
#include <numeric>
#include <print>
#include <span>
#include <vector>

int sum(std::span<const int> values) {
  return std::accumulate(values.begin(), values.end(), 0);
}

int main() {
  constexpr std::array<int, 4> table{1, 2, 3, 4};
  constexpr int stack_vals[] = {10, 20, 30};
  const std::vector<int> heap{7, 8};

  std::println("sum(std::array) = {}", sum(table));
  std::println("sum(C array)    = {}", sum(stack_vals));
  std::println("sum(vector)     = {}", sum(heap));

  return 0;
}
