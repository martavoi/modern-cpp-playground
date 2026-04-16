# modern-cpp-playground

Small CMake project with standalone demos for modern C++ features.

## Demos

- `ranges_demo`: a small ranges pipeline over integers
- `concepts_demo`: a small custom concept constraining a function template
- `cpp26_demo`: structured binding declaration as a condition
- `print_demo`: `std::println` from `<print>` and `std::format` from `<format>`
- `spaceship_demo`: defaulted `operator<=>` with `static_assert` checks
- `constexpr_demo`: tiny compile-time HTTP-style router (fixed route table, `consteval` checks, `constexpr` match, no allocations in dispatch)
- `jthread_demo`: `std::jthread` with `std::stop_token` and cooperative stop

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
cmake --build build --target print_demo
cmake --build build --target spaceship_demo
cmake --build build --target constexpr_demo
cmake --build build --target jthread_demo
```

## Run

The executable paths below assume a single-config generator such as the default Makefiles or Ninja flow on this machine.

```bash
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
./build/src/print_demo/print_demo
./build/src/spaceship_demo/spaceship_demo
./build/src/constexpr_demo/constexpr_demo
./build/src/jthread_demo/jthread_demo
```

## Clean verification

```bash
rm -rf build && cmake -S . -B build
cmake --build build
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
./build/src/print_demo/print_demo
./build/src/spaceship_demo/spaceship_demo
./build/src/constexpr_demo/constexpr_demo
./build/src/jthread_demo/jthread_demo
```

Expected output:

- `Even squares: 4 16 36`
- `square(12) = 144`
- `square(1.5) = 2.25`
- `Parsed digit: 7`
- `Could not parse input`
- `Hello from print_demo`
- `2 + 3 = 5`
- `hex 255 = 0xff, padded = 0007`
- `Version 1.10 vs 2.0: less`
- `GET /health -> health`
- `POST /api/users -> api_users`
- `GET /unknown -> no match`
- `worker is running`
- `stop requested`
- `worker exited after stop`
