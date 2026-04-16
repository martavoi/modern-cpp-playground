/**
 * @file main.cpp
 * @brief Demo: C++20 concepts constraining a function template.
 *
 * A @e concept is a named boolean constraint on types. `Squarable` asks that
 * `value * value` is well-formed and its type is `T`. The explicit template
 * form and the abbreviated `Squarable auto` parameter are equivalent — the
 * latter keeps the constraint right next to the parameter. Misuse becomes a
 * short, readable error rather than a wall of template instantiations.
 *
 * Build: `cmake --build <build-dir> --target concepts_demo`
 * Run:   `<build-dir>/src/concepts_demo/concepts_demo`
 */

#include <concepts>
#include <print>

template <typename T>
concept Squarable = requires(T value) {
  { value * value } -> std::same_as<T>;
};

template <Squarable T>
T square_explicit(T value) {
  return value * value;
}

auto square_abbreviated(Squarable auto value) {
  return value * value;
}

int main() {
  std::println("square_explicit(12)    = {}", square_explicit(12));
  std::println("square_abbreviated(12) = {}", square_abbreviated(12));
  std::println("square_explicit(1.5)   = {}", square_explicit(1.5));
  std::println("square_abbreviated(1.5)= {}", square_abbreviated(1.5));
  // Not Squarable: square_*(std::string{"hello"}) would fail to compile.
  return 0;
}
