#include <iostream>
#include <string_view>

struct ParseResult {
  int value;
  bool success;
  explicit operator bool() const { return success; }
};

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
