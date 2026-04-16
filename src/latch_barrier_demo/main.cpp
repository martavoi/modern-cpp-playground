/**
 * @file main.cpp
 * @brief Demo: `std::latch` (one-shot) and `std::barrier` (reusable) — C++20.
 *
 * `std::latch` is a single-use downward counter: once it hits zero every
 * `wait()` unblocks, and it stays at zero forever. `std::barrier` is the
 * reusable cousin: each round, all N participants `arrive_and_wait()`, an
 * optional completion function runs exactly once on the last-arriving
 * thread, and then all N are released together.
 *
 * Pattern:
 *   1. Spawn N workers that block on a `start_line` latch (count = 1).
 *   2. Main flips the latch — every worker starts simultaneously.
 *   3. Each worker writes its per-phase partial into its own array slot,
 *      then rendezvous on `sync`. The completion function prints that
 *      phase's summary — the only thing any thread ever prints besides
 *      main's pre-start line, which makes the output deterministic.
 *   4. `std::jthread` auto-joins at scope exit.
 *
 * Build: `cmake --build <build-dir> --target latch_barrier_demo`
 * Run:   `<build-dir>/src/latch_barrier_demo/latch_barrier_demo`
 */

#include <array>
#include <barrier>
#include <latch>
#include <numeric>
#include <print>
#include <thread>
#include <vector>

int main() {
  constexpr int worker_count = 4;
  constexpr int phase_count = 3;

  std::array<int, worker_count> partials{};
  int phase = 0;

  std::latch start_line{1};

  std::barrier sync{worker_count, [&]() noexcept {
    const int total = std::accumulate(partials.begin(), partials.end(), 0);
    std::println("phase {} done: partials={} sum={}", phase, partials, total);
    ++phase;
  }};

  std::vector<std::jthread> workers;
  workers.reserve(worker_count);
  for (int i = 0; i < worker_count; ++i) {
    workers.emplace_back([&, i] {
      start_line.wait();
      for (int p = 0; p < phase_count; ++p) {
        partials[i] = (i + 1) * (p + 1);
        sync.arrive_and_wait();
      }
    });
  }

  std::println("main: {} workers waiting at the start line", worker_count);
  start_line.count_down();
  return 0;
}
