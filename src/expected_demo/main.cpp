/**
 * @file main.cpp
 * @brief Demo: `std::expected` (C++23) — monadic operations on both channels.
 *
 * `std::expected<T, E>` carries either a `T` or an `E`, no exceptions.
 * Monadic combinators let us write straight-line pipelines:
 *   - `transform`        maps the value on success
 *   - `and_then`         chains a fallible step (may yield a new error)
 *   - `or_else`          recovers from an error back into the value channel
 *   - `transform_error`  maps the error type itself (E1 -> E2)
 * The error short-circuits through every step that touches the value side.
 *
 * Build: `cmake --build <build-dir> --target expected_demo`
 * Run:   `<build-dir>/src/expected_demo/expected_demo`
 */

#include <expected>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <utility>

enum class ParseError { Empty, NotDigit };

constexpr std::string_view describe(ParseError e) {
  using enum ParseError;
  switch (e) {
    case Empty:    return "empty_or_multi_char";
    case NotDigit: return "not_a_digit";
  }
  std::unreachable();
}

template <>
struct std::formatter<ParseError> : std::formatter<std::string_view> {
  auto format(ParseError e, auto& ctx) const {
    return std::formatter<std::string_view>::format(describe(e), ctx);
  }
};

std::expected<int, ParseError> parse_digit(std::string_view s) {
  if (s.size() != 1) {
    return std::unexpected{ParseError::Empty};
  }
  const char c = s[0];
  if (c < '0' || c > '9') {
    return std::unexpected{ParseError::NotDigit};
  }
  return c - '0';
}

template <typename T, typename E>
void show(std::string_view label, const std::expected<T, E>& r) {
  if (r) {
    std::println("  {:48} -> {}", label, *r);
  } else {
    std::println("  {:48} !! {}", label, r.error());
  }
}

int main() {
  std::println("plain:");
  show("parse_digit(\"7\")", parse_digit("7"));
  show("parse_digit(\"x\")", parse_digit("x"));

  std::println("transform (value side):");
  const auto square = [](int d) { return d * d; };
  show("parse_digit(\"5\").transform(square)", parse_digit("5").transform(square));
  show("parse_digit(\"x\").transform(square)", parse_digit("x").transform(square));

  std::println("and_then (chained fallible):");
  const auto combine_with_9 = [](int d) {
    return parse_digit("9").transform([d](int e) { return d * 10 + e; });
  };
  show("parse_digit(\"3\").and_then(combine_with_9)", parse_digit("3").and_then(combine_with_9));
  show("parse_digit(\"x\").and_then(combine_with_9)", parse_digit("x").and_then(combine_with_9));

  std::println("or_else (recover):");
  const auto fallback_zero = [](ParseError) {
    return std::expected<int, ParseError>{0};
  };
  show("parse_digit(\"x\").or_else(fallback_zero)", parse_digit("x").or_else(fallback_zero));

  std::println("transform_error (error side: ParseError -> std::string):");
  const auto verbose_error = [](ParseError e) {
    return std::format("parse failed ({})", describe(e));
  };
  show("parse_digit(\"x\").transform_error(verbose_error)",
       parse_digit("x").transform_error(verbose_error));
  show("parse_digit(\"7\").transform_error(verbose_error)",
       parse_digit("7").transform_error(verbose_error));

  return 0;
}
