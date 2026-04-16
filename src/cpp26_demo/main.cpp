/**
 * @file main.cpp
 * @brief Demo: structured bindings in an `if` initializer (C++17).
 *
 * `if (init; cond)` from C++17 lets us declare and test in one statement.
 * Here the init is a structured-binding declaration, and the condition is
 * the underlying `ParseResult` object itself — `explicit operator bool` on
 * it selects the branch, while `value` and `error` stay in scope for both
 * branches of the `if`.
 *
 * Build: `cmake --build <build-dir> --target cpp26_demo`
 * Run:   `<build-dir>/src/cpp26_demo/cpp26_demo`
 */

#include <print>
#include <string_view>

struct ParseResult {
  int value;
  std::string_view error;

  explicit operator bool() const { return error.empty(); }
};

ParseResult parse_digit(std::string_view s) {
  if (s.size() != 1) {
    return {0, "input is not a single character"};
  }
  const char c = s[0];
  if (c < '0' || c > '9') {
    return {0, "character is not a decimal digit"};
  }
  return {c - '0', {}};
}

void report(std::string_view input) {
  if (auto [value, error] = parse_digit(input)) {
    std::println("parse_digit({:?}) -> {}", input, value);
  } else {
    std::println("parse_digit({:?}) failed: {}", input, error);
  }
}

int main() {
  report("7");
  report("no");
  report("x");
}
