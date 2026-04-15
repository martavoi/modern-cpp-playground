#include <concepts>
#include <iostream>
#include <string>

template <typename T>
concept Squarable = requires(T value) {
  { value * value } -> std::same_as<T>;
};

template <Squarable T>
auto square(T value) -> T {
  return value * value;
}

int main() {
  std::cout << "square(12) = " << square(12) << '\n';
  std::cout << "square(1.5) = " << square(1.5) << '\n';
  // square(std::string{"hello"});
  return 0;
}
