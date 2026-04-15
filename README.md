# modern-cpp-playground

Small CMake project with three standalone demos for modern C++ features.

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

## Build per target

```bash
cmake --build build --target ranges_demo
cmake --build build --target concepts_demo
cmake --build build --target cpp26_demo
```

## Run

The executable paths below assume a single-config generator such as the default Makefiles or Ninja flow on this machine.

```bash
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
```

## Clean verification

```bash
rm -rf build && cmake -S . -B build
cmake --build build
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
```

Expected output:

- `Even squares: 4 16 36`
- `square(12) = 144`
- `square(1.5) = 2.25`
- `Parsed digit: 7`
- `Could not parse input`
