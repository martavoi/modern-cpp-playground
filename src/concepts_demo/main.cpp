/**
 * @file main.cpp
 * @brief Demo: C++20 concepts constraining a function template.
 *
 * A @e concept is a named boolean constraint on types. Here `Squarable`
 * requires that `value * value` exists and has the same type as `T`.
 * `square` only accepts types satisfying that concept, so misuse is a
 * clear compile error instead of a long template instantiation diagnostic.
 *
 * Build: `cmake --build <build-dir> --target concepts_demo`
 * Run:   `<build-dir>/src/concepts_demo/concepts_demo`
 */

#include <concepts>
#include <iostream>
#include <string>

/** Requires `T` to support multiplication with itself yielding `T`. */
template <typename T>
concept Squarable = requires(T value) {
  { value * value } -> std::same_as<T>;
};

/** Returns `value * value` for any @ref Squarable type. */
template <Squarable T>
auto square(T value) -> T {
  return value * value;
}

int main() {
  std::cout << "square(12) = " << square(12) << '\n';
  std::cout << "square(1.5) = " << square(1.5) << '\n';
  // `std::string` is not Squarable: `square(std::string{"hello"});` will not compile.
  return 0;
}
