# C++ Feature Exploration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a tiny CMake-based project with three independently buildable executables that demonstrate C++20 ranges, C++20 concepts, and one supported C++26 language feature on Apple clang 21.

**Architecture:** Use one top-level CMake project and one subdirectory per demo. Keep each executable fully self-contained, with no shared support library and no external dependencies, so each target teaches one idea clearly.

**Tech Stack:** Apple clang 21.0.0, Xcode 26.4, CMake 4.3.1, C++20, C++26 (`CXX_STANDARD 26`)

---

## File Structure

- Create: `.gitignore`
- Create: `CMakeLists.txt`
- Create: `README.md`
- Create: `src/ranges_demo/CMakeLists.txt`
- Create: `src/ranges_demo/main.cpp`
- Create: `src/concepts_demo/CMakeLists.txt`
- Create: `src/concepts_demo/main.cpp`
- Create: `src/cpp26_demo/CMakeLists.txt`
- Create: `src/cpp26_demo/main.cpp`

File responsibilities:

- `CMakeLists.txt`: top-level project definition and subdirectory registration.
- `README.md`: short explanation of the project plus configure, build, and run commands.
- `src/ranges_demo/CMakeLists.txt`: defines the `ranges_demo` executable with a C++20 standard requirement.
- `src/ranges_demo/main.cpp`: demonstrates a small ranges pipeline over integers.
- `src/concepts_demo/CMakeLists.txt`: defines the `concepts_demo` executable with a C++20 standard requirement.
- `src/concepts_demo/main.cpp`: defines a small custom concept and uses it to constrain a templated function.
- `src/cpp26_demo/CMakeLists.txt`: defines the `cpp26_demo` executable with `CXX_STANDARD 26`.
- `src/cpp26_demo/main.cpp`: demonstrates structured binding declaration as a condition with a custom result type.

### Task 1: Bootstrap Root Build And `ranges_demo`

**Files:**
- Create: `.gitignore`
- Create: `CMakeLists.txt`
- Create: `src/ranges_demo/CMakeLists.txt`
- Create: `src/ranges_demo/main.cpp`

- [ ] **Step 1: Create the initial ignore file**

```gitignore
.worktrees/
build/
.DS_Store
```

- [ ] **Step 2: Create the top-level CMake project with only the ranges demo enabled**

```cmake
cmake_minimum_required(VERSION 3.25)

project(modern_cpp_playground LANGUAGES CXX)

add_subdirectory(src/ranges_demo)
```

- [ ] **Step 3: Create the ranges demo CMake file**

```cmake
add_executable(ranges_demo main.cpp)
target_compile_features(ranges_demo PRIVATE cxx_std_20)
set_target_properties(ranges_demo PROPERTIES CXX_EXTENSIONS OFF)
```

- [ ] **Step 4: Create the ranges demo source**

```cpp
#include <iostream>
#include <ranges>
#include <vector>

int main() {
  std::vector<int> values{1, 2, 3, 4, 5, 6};
  auto even_squares = values
    | std::views::filter([](int value) { return value % 2 == 0; })
    | std::views::transform([](int value) { return value * value; });

  std::cout << "Even squares:";
  for (int value : even_squares) {
    std::cout << ' ' << value;
  }
  std::cout << '\n';
}
```

- [ ] **Step 5: Configure the project**

Run: `cmake -S . -B build`

Expected: CMake configures successfully and writes build files to `build/` without errors.

- [ ] **Step 6: Build only the ranges demo**

Run: `cmake --build build --target ranges_demo`

Expected: The build completes successfully and reports that `ranges_demo` was built.

- [ ] **Step 7: Run the ranges demo**

Run: `./build/src/ranges_demo/ranges_demo`

Expected output:

```text
Even squares: 4 16 36
```

### Task 2: Add `concepts_demo`

**Files:**
- Modify: `CMakeLists.txt`
- Create: `src/concepts_demo/CMakeLists.txt`
- Create: `src/concepts_demo/main.cpp`

- [ ] **Step 1: Register the concepts demo in the top-level CMake file**

```cmake
cmake_minimum_required(VERSION 3.25)

project(modern_cpp_playground LANGUAGES CXX)

add_subdirectory(src/ranges_demo)
add_subdirectory(src/concepts_demo)
```

- [ ] **Step 2: Create the concepts demo CMake file**

```cmake
add_executable(concepts_demo main.cpp)
target_compile_features(concepts_demo PRIVATE cxx_std_20)
set_target_properties(concepts_demo PROPERTIES CXX_EXTENSIONS OFF)
```

- [ ] **Step 3: Create the concepts demo source**

```cpp
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

  // Uncomment to see a concepts error with a type that is not Squarable.
  // std::cout << square(std::string{"hello"}) << '\n';
}
```

- [ ] **Step 4: Reconfigure the project**

Run: `cmake -S . -B build`

Expected: CMake reconfigures successfully and discovers both demo targets.

- [ ] **Step 5: Build only the concepts demo**

Run: `cmake --build build --target concepts_demo`

Expected: The build completes successfully and reports that `concepts_demo` was built.

- [ ] **Step 6: Run the concepts demo**

Run: `./build/src/concepts_demo/concepts_demo`

Expected output:

```text
square(12) = 144
square(1.5) = 2.25
```

### Task 3: Add `cpp26_demo`

**Files:**
- Modify: `CMakeLists.txt`
- Create: `src/cpp26_demo/CMakeLists.txt`
- Create: `src/cpp26_demo/main.cpp`

- [ ] **Step 1: Register the C++26 demo in the top-level CMake file**

```cmake
cmake_minimum_required(VERSION 3.25)

project(modern_cpp_playground LANGUAGES CXX)

add_subdirectory(src/ranges_demo)
add_subdirectory(src/concepts_demo)
add_subdirectory(src/cpp26_demo)
```

- [ ] **Step 2: Create the C++26 demo CMake file**

```cmake
add_executable(cpp26_demo main.cpp)
set_target_properties(cpp26_demo PROPERTIES
  CXX_STANDARD 26
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
)
```

- [ ] **Step 3: Create the C++26 demo source**

```cpp
#include <iostream>
#include <string_view>

struct ParseResult {
  int value;
  bool success;

  explicit operator bool() const {
    return success;
  }
};

auto parse_digit(std::string_view text) -> ParseResult {
  if (text.size() == 1 && text.front() >= '0' && text.front() <= '9') {
    return {text.front() - '0', true};
  }

  return {0, false};
}

int main() {
  if (auto [value, success] = parse_digit("7")) {
    std::cout << "Parsed digit: " << value << '\n';
  } else {
    std::cout << "Could not parse input\n";
  }

  if (auto [value, success] = parse_digit("no")) {
    std::cout << "Parsed digit: " << value << '\n';
  } else {
    std::cout << "Could not parse input\n";
  }
}
```

- [ ] **Step 4: Reconfigure the project**

Run: `cmake -S . -B build`

Expected: CMake reconfigures successfully and discovers the new `cpp26_demo` target.

- [ ] **Step 5: Build only the C++26 demo**

Run: `cmake --build build --target cpp26_demo`

Expected: The build completes successfully and reports that `cpp26_demo` was built.

- [ ] **Step 6: Run the C++26 demo**

Run: `./build/src/cpp26_demo/cpp26_demo`

Expected output:

```text
Parsed digit: 7
Could not parse input
```

### Task 4: Write `README.md` And Run Full Verification

**Files:**
- Create: `README.md`

- [ ] **Step 1: Create the project README**

````markdown
# C++ Feature Exploration

Tiny CMake-based demos for learning a few useful features from C++20 and C++26 on Apple clang.

## Demos

- `ranges_demo`: a small ranges pipeline over integers
- `concepts_demo`: a small custom concept constraining a function template
- `cpp26_demo`: structured binding declaration as a condition

## Requirements

- Xcode 26.4 or newer
- Apple clang 21
- CMake 3.25 or newer

## Configure

```bash
cmake -S . -B build
```

## Build One Target

```bash
cmake --build build --target ranges_demo
cmake --build build --target concepts_demo
cmake --build build --target cpp26_demo
```

## Run

```bash
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
```
````

- [ ] **Step 2: Reconfigure from a clean build directory**

Run: `rm -rf build && cmake -S . -B build`

Expected: CMake configures all three targets successfully with a fresh build tree.

- [ ] **Step 3: Build the full project**

Run: `cmake --build build`

Expected: The full build completes successfully and all three executables are produced.

- [ ] **Step 4: Run the ranges demo again as part of the final verification**

Run: `./build/src/ranges_demo/ranges_demo`

Expected output:

```text
Even squares: 4 16 36
```

- [ ] **Step 5: Run the concepts demo again as part of the final verification**

Run: `./build/src/concepts_demo/concepts_demo`

Expected output:

```text
square(12) = 144
square(1.5) = 2.25
```

- [ ] **Step 6: Run the C++26 demo again as part of the final verification**

Run: `./build/src/cpp26_demo/cpp26_demo`

Expected output:

```text
Parsed digit: 7
Could not parse input
```
