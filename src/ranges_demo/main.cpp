#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3, 4, 5, 6};

    std::cout << "Even squares:";
    for (int n : values | std::views::filter([](int x) { return x % 2 == 0; })
                        | std::views::transform([](int x) { return x * x; })) {
        std::cout << ' ' << n;
    }
    std::cout << '\n';
    return 0;
}
