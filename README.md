# modern-cpp-playground

Small CMake project with standalone demos for modern C++ features.
Each demo is a single `main.cpp` focused on one concept, built as its own
target and runnable by itself.

## Demos

| Target            | Standard | What it shows                                                                                   |
| ----------------- | -------- | ----------------------------------------------------------------------------------------------- |
| `ranges_demo`     | C++20/23 | Lazy `views::filter`/`views::transform`, then `std::ranges::to` and `std::ranges::fold_left`    |
| `concepts_demo`   | C++20    | Named `concept`; explicit template and abbreviated `Squarable auto` forms                       |
| `cpp26_demo`      | C++17    | Structured-binding declaration as an `if` initializer                                           |
| `print_demo`      | C++23    | `std::println` / `std::format`, plus a custom `std::formatter<Color>` with its own spec         |
| `spaceship_demo`  | C++20    | Defaulted `operator<=>` with `static_assert`-checked orderings                                  |
| `constexpr_demo`  | C++20    | Tiny `constexpr` HTTP-style router with `consteval` validation; no dispatch-time allocations    |
| `jthread_demo`    | C++20    | `std::jthread` with `std::stop_token` cooperative cancellation                                  |
| `expected_demo`   | C++23    | `std::expected` with `transform` / `and_then` / `or_else` / `transform_error`                   |
| `optional_demo`   | C++23    | `std::optional` monadic operations                                                              |
| `span_demo`       | C++20    | `std::span` over `std::array`, a C array, and `std::vector`                                     |
| `mdspan_demo`     | C++23    | `std::mdspan` with dynamic / fixed extents and row- / column-major layouts                      |
| `chrono_demo`     | C++20/23 | `<chrono>` calendar types, duration arithmetic, `hh_mm_ss`, and format specifiers               |
| `if_consteval_demo` | C++23  | `if consteval` dispatch: Newton's method at compile time, `std::sqrt` at runtime                |
| `latch_barrier_demo` | C++20 | `std::latch` as a start gun + `std::barrier` for per-phase rendezvous with a completion function|

## Requirements

- Xcode 26.4 or newer
- Apple clang 21
- CMake 3.25 or newer

## Configure and build

```bash
cmake -S . -B build
cmake --build build
```

To build a single demo:

```bash
cmake --build build --target ranges_demo
```

## Run

```bash
./build/src/ranges_demo/ranges_demo
./build/src/concepts_demo/concepts_demo
./build/src/cpp26_demo/cpp26_demo
./build/src/print_demo/print_demo
./build/src/spaceship_demo/spaceship_demo
./build/src/constexpr_demo/constexpr_demo
./build/src/jthread_demo/jthread_demo
./build/src/expected_demo/expected_demo
./build/src/optional_demo/optional_demo
./build/src/span_demo/span_demo
./build/src/mdspan_demo/mdspan_demo
./build/src/chrono_demo/chrono_demo
./build/src/if_consteval_demo/if_consteval_demo
./build/src/latch_barrier_demo/latch_barrier_demo
```

## Expected output

```text
# ranges_demo
Even squares (lazy view): [4, 16, 36]
Squares 1..5 collected:   [1, 4, 9, 16, 25]
Sum of even squares:      56

# concepts_demo
square_explicit(12)    = 144
square_abbreviated(12) = 144
square_explicit(1.5)   = 2.25
square_abbreviated(1.5)= 2.25

# cpp26_demo
parse_digit("7") -> 7
parse_digit("no") failed: input is not a single character
parse_digit("x") failed: character is not a decimal digit

# print_demo
Hello from print_demo
2 + 3 = 5
hex 255 = 0xff, padded = 0007
default spec: rgb(0, 128, 128)
hex spec:     #008080

# spaceship_demo
Version 1.10 vs 2.0: less

# constexpr_demo
GET /health -> health
POST /api/users -> api_users
GET /unknown -> no match

# jthread_demo
worker: running
main:   requesting stop
worker: exiting after stop request

# expected_demo
plain:
  parse_digit("7")                                 -> 7
  parse_digit("x")                                 !! not_a_digit
transform (value side):
  parse_digit("5").transform(square)               -> 25
  parse_digit("x").transform(square)               !! not_a_digit
and_then (chained fallible):
  parse_digit("3").and_then(combine_with_9)        -> 39
  parse_digit("x").and_then(combine_with_9)        !! not_a_digit
or_else (recover):
  parse_digit("x").or_else(fallback_zero)          -> 0
transform_error (error side: ParseError -> std::string):
  parse_digit("x").transform_error(verbose_error)  !! parse failed (not_a_digit)
  parse_digit("7").transform_error(verbose_error)  -> 7

# optional_demo
parse_digit("4").transform(+10) -> 14
parse_digit("7").and_then(double_if_ge_5) -> 14
parse_digit("z") -> nullopt, or_else supplies -1
after or_else -> -1

# span_demo
sum(std::array) = 10
sum(C array)    = 60
sum(vector)     = 15

# mdspan_demo
row-major, dynamic extents (3x4):
   1   2   3   4
   5   6   7   8
   9  10  11  12
row-major, fixed 3x4 (3x4):
   1   2   3   4
   5   6   7   8
   9  10  11  12
column-major, same storage (3x4):
   1   4   7  10
   2   5   8  11
   3   6   9  12
3-D cube corner cube[1, 0, 1] = 101

# chrono_demo  (the 'utc now' line varies, everything else is fixed)
today:           2026-04-16
today (iso):     2026-04-16
weekday:         Thursday
last of Feb:     2026-02-28
days until 2026-06-01: 46
flight hms:      08:45:17
flight rounded:  525min
utc now:         2026-04-16 20:47:32

# if_consteval_demo
compile-time my_sqrt(2) = 1.414213562373095
run-time     my_sqrt(2) = 1.4142135623730951
absolute delta          = 2.220e-16

# latch_barrier_demo
main: 4 workers waiting at the start line
phase 0 done: partials=[1, 2, 3, 4] sum=10
phase 1 done: partials=[2, 4, 6, 8] sum=20
phase 2 done: partials=[3, 6, 9, 12] sum=30
```

## Clean verification

```bash
rm -rf build && cmake -S . -B build
cmake --build build
for t in ranges_demo concepts_demo cpp26_demo print_demo spaceship_demo \
         constexpr_demo jthread_demo expected_demo optional_demo \
         span_demo mdspan_demo chrono_demo if_consteval_demo \
         latch_barrier_demo; do
  echo "== $t ==" && ./build/src/$t/$t
done
```
