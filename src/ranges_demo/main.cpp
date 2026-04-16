/**
 * @file main.cpp
 * @brief Demo: C++20 ranges and views over a container.
 *
 * Shows a lazy pipeline: filter elements, then transform them, without
 * allocating a new container for the intermediate step. The range adaptor
 * chain composes with `operator|`; iteration pulls values on demand.
 *
 * Build: `cmake --build <build-dir> --target ranges_demo`
 * Run:   `<build-dir>/src/ranges_demo/ranges_demo`
 */

#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3, 4, 5, 6};

    // Keep even numbers, then square each; `views` are lazy until iterated.
    std::cout << "Even squares:";
    for (int n : values | std::views::filter([](int x) { return x % 2 == 0; })
                        | std::views::transform([](int x) { return x * x; })) {
        std::cout << ' ' << n;
    }
    std::cout << '\n';
    return 0;
}
