/**
 * @file main.cpp
 * @brief Demo: `std::optional` with C++23 monadic operations.
 *
 * When all we need is "maybe a value" — no error channel — `optional` is
 * lighter than `expected`. `transform`, `and_then`, and `or_else` let us
 * build pipelines without nested `if (opt.has_value())` branches.
 *
 * Build: `cmake --build <build-dir> --target optional_demo`
 * Run:   `<build-dir>/src/optional_demo/optional_demo`
 */

#include <optional>
#include <print>
#include <string_view>

std::optional<int> parse_digit(std::string_view s) {
  if (s.size() != 1) {
    return std::nullopt;
  }
  const char c = s[0];
  if (c < '0' || c > '9') {
    return std::nullopt;
  }
  return c - '0';
}

int main() {
  const auto inc = parse_digit("4").transform([](int d) { return d + 10; });
  std::println("parse_digit(\"4\").transform(+10) -> {}", *inc);

  const auto gated = parse_digit("7").and_then([](int d) -> std::optional<int> {
    return d >= 5 ? std::optional{d * 2} : std::nullopt;
  });
  std::println("parse_digit(\"7\").and_then(double_if_ge_5) -> {}", *gated);

  const auto recovered = parse_digit("z").or_else([] {
    std::println("parse_digit(\"z\") -> nullopt, or_else supplies -1");
    return std::optional{-1};
  });
  std::println("after or_else -> {}", *recovered);

  return 0;
}
