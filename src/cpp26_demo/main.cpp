/**
 * @file main.cpp
 * @brief Demo: structured bindings in an `if` initializer (C++17).
 *
 * Combines initialization and a boolean condition: the `if` tests the
 * object returned from the initializer after binding its members. Here
 * `ParseResult` is contextually convertible to `bool` via `explicit operator bool`,
 * so the success flag drives the branch while `value` stays in scope
 * inside the `if` body only.
 *
 * Build: `cmake --build <build-dir> --target cpp26_demo`
 * Run:   `<build-dir>/src/cpp26_demo/cpp26_demo`
 */

#include <iostream>
#include <string_view>

/** Simple parse outcome: numeric @p value when @p success is true. */
struct ParseResult {
  int value;
  bool success;
  /** Used by `if (ParseResult(...))` — only @p success is consulted. */
  explicit operator bool() const { return success; }
};

/** Parses a single ASCII digit; empty or non-digit input yields `success == false`. */
ParseResult parse_digit(std::string_view s) {
  if (s.size() != 1) {
    return {0, false};
  }
  const char c = s[0];
  if (c < '0' || c > '9') {
    return {0, false};
  }
  return {c - '0', true};
}

int main() {
  if (auto [value, success] = parse_digit("7")) {
    static_cast<void>(success);
    std::cout << "Parsed digit: " << value << '\n';
  } else {
  }
  if (auto [value, success] = parse_digit("no")) {
  } else {
    static_cast<void>(value);
    static_cast<void>(success);
    std::cout << "Could not parse input\n";
  }
}
