# `print_demo` Design

## Goal

Explore whether the default Apple clang and Xcode toolchain on this machine supports the standard library `<print>` facility well enough to add a new standalone `print_demo` target to the existing modern C++ playground.

The desired outcome is simple:

- if `<print>` builds and runs successfully in this project, keep the new demo
- if `<print>` is not supported by the current default toolchain, remove the temporary target and leave the repository unchanged

## Project Context

The repository already contains three tiny standalone demo executables:

1. `ranges_demo`
2. `concepts_demo`
3. `cpp26_demo`

Each demo is intentionally small, self-contained, and registered as its own subdirectory under `src/`. The new `print_demo` should follow that same pattern if it survives the support check.

## Scope

This design covers only one small experiment:

- add a temporary fourth demo target named `print_demo`
- implement it using `<print>` and `std::println`
- build it with the default Apple toolchain already on the machine
- keep it only if the build succeeds

## Non-Goals

- Do not add compiler switching, Homebrew LLVM, or non-default toolchains.
- Do not add CMake feature-detection logic for `<print>`.
- Do not add fallback code paths inside the demo.
- Do not keep a broken or unsupported target in the repository.
- Do not update documentation unless the demo is confirmed to work and remain in the project.

## Architecture

The project keeps its current structure and may gain one additional standalone target:

- `CMakeLists.txt` registers `src/print_demo`
- `src/print_demo/CMakeLists.txt` defines the `print_demo` executable
- `src/print_demo/main.cpp` contains a minimal `<print>` example

The implementation approach is intentionally direct rather than adaptive. Instead of probing support during CMake configuration, the repository temporarily adds the target and relies on the normal build as the support check.

## Components

### 1. Top-Level CMake Registration

If the experiment succeeds, the root `CMakeLists.txt` will add:

- `add_subdirectory(src/print_demo)`

If the experiment fails, this change will be removed.

### 2. `src/print_demo/CMakeLists.txt`

This file will define one executable named `print_demo` and require a language level appropriate for `<print>`. The preferred standard level is `C++23`, since the goal is to demonstrate the standard printing facility rather than a compiler extension.

### 3. `src/print_demo/main.cpp`

The source file should remain as small and readable as the other demos. It should:

- include `<print>`
- call `std::println(...)`
- print a short visible result
- exit normally

There should be no extra abstraction, helper library, or portability scaffolding in this file.

### 4. `README.md`

The README should only be updated if the new demo builds successfully and is retained in the project. If `<print>` is unsupported and the target is removed, the README remains unchanged.

## Data Flow

The demo execution path should be trivial:

1. `main.cpp` includes `<print>`
2. the program calls `std::println(...)`
3. the program emits one or two formatted lines
4. the process exits

At the project level, the workflow is:

1. add `print_demo`
2. configure and build the target with the default Apple toolchain
3. if the build succeeds, run the executable once and keep the target
4. if the build fails due to missing `<print>` support, remove the target and restore the prior project shape

## Error Handling

The demo itself should not contain compatibility handling. Build failure is the compatibility signal.

That means:

- `main.cpp` assumes `<print>` support
- no `#ifdef` fallback path is added
- no alternate output API is included
- toolchain incompatibility is handled by removing the target rather than complicating the demo

This preserves the teaching value of the example by keeping it focused on the actual feature under exploration.

## Testing Strategy

Verification should stay lightweight and consistent with the rest of the repository:

1. reconfigure the project with `print_demo` present
2. build `print_demo` using the default Apple toolchain
3. if the build succeeds, run the executable once to confirm visible output
4. if the build fails because `<print>` is unavailable, delete the new target and do not keep any related documentation changes

No dedicated unit tests are needed for this experiment because the demo is intentionally tiny and the key question is toolchain support rather than runtime behavior complexity.

## Success Criteria

This work is successful if one of the following outcomes is reached cleanly:

1. `print_demo` builds and runs with the default Apple toolchain and becomes a permanent fourth demo target
2. `print_demo` fails due to lack of `<print>` support and is removed cleanly, leaving the repository effectively unchanged

Both outcomes are acceptable because the purpose of the task is to verify real support on the current machine, not to force the feature into the project.
