# C++ Feature Exploration Project Design

## Goal

Create a very small CMake-based C++ project for macOS that explores a few worthwhile features from modern C++, with the initial demos focused on C++20 and C++26 using the default Apple toolchain already installed on the machine.

The project should stay intentionally minimal: each demo should be easy to read in under a minute, build independently, and highlight one feature without extra architecture or framework code around it.

## Toolchain Baseline

- Host OS: macOS 26.4.1
- Xcode: 26.4
- Compiler: Apple clang 21.0.0
- Build system: CMake 4.3.1

This project targets the default Apple toolchain rather than a separate Homebrew LLVM installation. The purpose is to keep setup simple and ensure the demos reflect what is readily available on the machine.

## Scope

The initial project will contain three standalone executables:

1. `ranges_demo`
2. `concepts_demo`
3. `cpp26_demo`

Each executable exists to teach one idea clearly. The project is not intended to be a library, framework, benchmark suite, or comprehensive compatibility matrix.

## Non-Goals

- Do not attempt to demonstrate unsupported features such as standard C++26 reflection.
- Do not introduce external dependencies.
- Do not build a large example app around the language features.
- Do not add complexity just to make the project look more realistic.

## Project Layout

The repository should use a small top-level CMake project with one subdirectory per demo:

- `CMakeLists.txt`
- `README.md`
- `src/ranges_demo/CMakeLists.txt`
- `src/ranges_demo/main.cpp`
- `src/concepts_demo/CMakeLists.txt`
- `src/concepts_demo/main.cpp`
- `src/cpp26_demo/CMakeLists.txt`
- `src/cpp26_demo/main.cpp`

This structure keeps each feature isolated while still allowing one shared configure step with CMake.

## Build Model

The build should use a single shared build directory such as `build/`, with independently buildable targets:

- `cmake -S . -B build`
- `cmake --build build --target ranges_demo`
- `cmake --build build --target concepts_demo`
- `cmake --build build --target cpp26_demo`

Language levels should be set per target rather than globally:

- `ranges_demo` uses `C++20`
- `concepts_demo` uses `C++20`
- `cpp26_demo` uses `C++2c`

This keeps each demo honest about the standard that introduced the feature being explored.

## Demo Design

### 1. `ranges_demo`

Purpose:
Show a small but useful pipeline written with C++20 ranges.

Behavior:
- Start from a small `std::vector<int>`.
- Filter the even values.
- Transform the remaining values by squaring them.
- Print the resulting sequence.

Why this demo exists:
- Ranges are widely useful in everyday C++ code.
- The feature is well supported on the default Apple toolchain and makes a good minimal standalone demo.

### 2. `concepts_demo`

Purpose:
Make concepts feel concrete and understandable rather than theoretical.

Behavior:
- Define one small named concept, likely based on a type supporting a simple operation such as addition.
- Use the concept to constrain one function template that would be awkward or less clear without the constraint.
- Run a valid example in `main.cpp`.
- Include one commented-out invalid usage to make it easy to inspect compiler diagnostics later without breaking the default build.

Why this demo exists:
- Concepts are highly relevant for readable generic code.
- They improve intent and diagnostics compared with unconstrained templates.

### 3. `cpp26_demo`

Purpose:
Explore one C++26 feature that is supported by the current Apple clang toolchain.

Chosen feature:
- Structured binding declaration as a condition.

Behavior:
- Return a small custom result type from a parsing helper.
- Use the structured binding directly in an `if` condition.
- Print whether parsing succeeded or failed.

Why this demo exists:
- It keeps the project aligned with the original goal of exploring newer C++ features.
- It is small, readable, and supported today on the selected toolchain.

## Data Flow

Each executable should be entirely self-contained:

- `main.cpp` constructs a tiny example input
- feature-specific code runs
- the program prints one or two short lines showing the result

There is no shared runtime state across executables, no reusable support library, and no configuration file format in the initial version.

## Error Handling

Error handling should stay minimal and feature-focused:

- Prefer direct program flow over abstraction layers.
- If a demo has an invalid case worth showing, keep it commented out so the default build remains successful.
- Avoid adding exception-heavy or framework-style handling that distracts from the feature being demonstrated.

## Testing Strategy

The initial implementation should rely primarily on build-and-run verification rather than a formal test suite.

Verification for each demo should include:

- target builds successfully with CMake
- executable runs successfully
- output confirms the intended feature is being exercised

Automated unit tests are not required in the first iteration because the project is primarily educational and the demos are intentionally tiny.

## Documentation

The `README.md` should explain:

- what each executable demonstrates
- which language standard each target uses
- how to configure the project
- how to build each target independently
- how to run each executable

The README should stay short and practical rather than tutorial-heavy.

## Future Extensions

Possible future demos may include additional features from C++20, C++23, or later C++26 support as the Apple toolchain evolves. These additions should follow the same rule as the initial demos: one small idea per target, with minimal surrounding code.
