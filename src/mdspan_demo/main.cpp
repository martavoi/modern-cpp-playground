/**
 * @file main.cpp
 * @brief Demo: `std::mdspan` — non-owning multi-dimensional view (C++23).
 *
 * Where `std::span` views a contiguous 1-D range, `std::mdspan` adds shape
 * and (optionally) a layout policy on top of the same flat storage. The
 * extents can be fully dynamic (`std::dextents`) or fixed at compile time,
 * and the multi-dim subscript `m[i, j]` comes straight from C++23.
 *
 * Build: `cmake --build <build-dir> --target mdspan_demo`
 * Run:   `<build-dir>/src/mdspan_demo/mdspan_demo`
 */

#include <array>
#include <mdspan>
#include <print>
#include <vector>

template <class Mdspan>
void print_matrix(std::string_view label, Mdspan m) {
  std::println("{} ({}x{}):", label, m.extent(0), m.extent(1));
  for (std::size_t r = 0; r < m.extent(0); ++r) {
    for (std::size_t c = 0; c < m.extent(1); ++c) {
      std::print(" {:3}", m[r, c]);
    }
    std::println();
  }
}

int main() {
  std::vector<int> storage{1, 2, 3,
                           4, 5, 6,
                           7, 8, 9,
                           10, 11, 12};

  std::mdspan dyn{storage.data(), 3, 4};
  print_matrix("row-major, dynamic extents", dyn);

  using Fixed = std::mdspan<int, std::extents<std::size_t, 3, 4>>;
  print_matrix("row-major, fixed 3x4", Fixed{storage.data()});

  using ColMajor = std::mdspan<int, std::dextents<std::size_t, 2>,
                               std::layout_left>;
  print_matrix("column-major, same storage", ColMajor{storage.data(), 3, 4});

  std::array<double, 8> cube_storage{};
  std::mdspan cube{cube_storage.data(), 2, 2, 2};
  for (std::size_t i = 0; i < 2; ++i)
    for (std::size_t j = 0; j < 2; ++j)
      for (std::size_t k = 0; k < 2; ++k)
        cube[i, j, k] = i * 100.0 + j * 10.0 + k;

  std::println("3-D cube corner cube[1, 0, 1] = {}", cube[1, 0, 1]);
  return 0;
}
