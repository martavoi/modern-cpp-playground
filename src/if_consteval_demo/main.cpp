/**
 * @file main.cpp
 * @brief Demo: `if consteval` (C++23) — one `constexpr` function, two bodies.
 *
 * `if consteval { A } else { B }` lets the same `constexpr` function take a
 * different path depending on whether the call is being evaluated as a
 * constant expression. The compile-time branch must itself be `constexpr`;
 * the runtime branch is free to call anything, so it can use hardware-backed
 * library routines that are not `constexpr`.
 *
 * Here `std::sqrt` is not `constexpr` in C++23, so a `static_assert` cannot
 * call it directly. Routing through `my_sqrt` lets the same spelling work in
 * both a `constexpr` context (Newton's method) and at runtime (`std::sqrt`).
 *
 * Build: `cmake --build <build-dir> --target if_consteval_demo`
 * Run:   `<build-dir>/src/if_consteval_demo/if_consteval_demo`
 */

#include <cmath>
#include <print>

/** Pure constexpr Newton's method; converges quadratically for x > 0. */
constexpr double sqrt_newton(double x) {
  double guess = x;
  for (int i = 0; i < 20; ++i) {
    guess = 0.5 * (guess + x / guess);
  }
  return guess;
}

constexpr double my_sqrt(double x) {
  if consteval {
    return sqrt_newton(x);
  } else {
    return std::sqrt(x);
  }
}

int main() {
  constexpr double ct = my_sqrt(2.0);
  static_assert(ct > 1.41421356 && ct < 1.41421357);

  const double x = 2.0;
  const double rt = my_sqrt(x);

  std::println("compile-time my_sqrt(2) = {}", ct);
  std::println("run-time     my_sqrt(2) = {}", rt);
  std::println("absolute delta          = {:.3e}", std::abs(rt - ct));
  return 0;
}
