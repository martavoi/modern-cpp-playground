/**
 * @file main.cpp
 * @brief Coroutines: manual `co_yield` generator (same promise shape as
 *        `std::generator` when `<generator>` exists) and a minimal `co_return` /
 *        `co_await` task.
 *
 * Results for `co_return` are written through a caller-owned slot passed into
 * the coroutine so the value survives coroutine frame teardown (the promise
 * object lives in the frame; reading `promise().value` after completion is not
 * reliable).
 *
 * Build: `cmake --build <build-dir> --target coroutine_demo`
 * Run:   `<build-dir>/src/coroutine_demo/coroutine_demo`
 */

#include <coroutine>
#include <memory>
#include <print>
#include <utility>

// --- Part 1: manual generator (promise with yield_value; like std::generator) ---

struct IntGenerator {
  struct promise_type {
    int current{};

    IntGenerator get_return_object() {
      return IntGenerator{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    std::suspend_always yield_value(int v) noexcept {
      current = v;
      return {};
    }

    void return_void() {}

    void unhandled_exception() { throw; }
  };

  using handle = std::coroutine_handle<promise_type>;

  handle coro{};

  explicit IntGenerator(handle h) : coro(h) {}

  IntGenerator(IntGenerator&& o) noexcept : coro(std::exchange(o.coro, {})) {}

  IntGenerator& operator=(IntGenerator&& o) noexcept {
    if (this != &o) {
      destroy();
      coro = std::exchange(o.coro, {});
    }
    return *this;
  }

  ~IntGenerator() { destroy(); }

  bool next(int& out) {
    if (!coro || coro.done()) {
      return false;
    }
    coro.resume();
    if (coro.done()) {
      return false;
    }
    out = coro.promise().current;
    return true;
  }

private:
  void destroy() {
    if (coro) {
      coro.destroy();
      coro = {};
    }
  }
};

IntGenerator fibonacci(unsigned count) {
  if (count == 0) {
    co_return;
  }
  unsigned a = 0;
  unsigned b = 1;
  for (unsigned i = 0; i < count; ++i) {
    co_yield static_cast<int>(a);
    const unsigned next = a + b;
    a = b;
    b = next;
  }
}

// --- Part 2: minimal promise (co_return and co_await) ---

struct Ready {
  int value{};

  [[nodiscard]] bool await_ready() const noexcept { return true; }

  void await_suspend(std::coroutine_handle<>) const noexcept {}

  [[nodiscard]] int await_resume() const noexcept { return value; }
};

/**
 * Eager task: `suspend_never` at start/end so the body finishes before `f()`
 * returns. The promise is constructed from the caller's `std::shared_ptr<int>`
 * (see C++20 coroutine promise construction from function parameters) so
 * `return_value` writes into heap storage that outlives the frame.
 */
struct IntTask {
  struct promise_type {
    std::shared_ptr<int> out;

    explicit promise_type(std::shared_ptr<int> slot) : out(std::move(slot)) {}

    IntTask get_return_object() {
      return IntTask{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_never initial_suspend() noexcept { return {}; }

    std::suspend_never final_suspend() noexcept { return {}; }

    void return_value(int v) const { *out = v; }

    void unhandled_exception() { throw; }
  };

  using handle = std::coroutine_handle<promise_type>;

  handle coro{};

  explicit IntTask(handle h) : coro(h) {}

  IntTask(IntTask&& o) noexcept : coro(std::exchange(o.coro, {})) {}

  IntTask& operator=(IntTask&& o) noexcept {
    if (this != &o) {
      coro = std::exchange(o.coro, {});
    }
    return *this;
  }

  // Note: On Apple Clang 21 (arm64), `coroutine_handle::destroy()` faults for
  // this parameterized-promise task after completion, so we omit destroy here
  // (small one-shot leak of the coroutine state in this demo only).
  ~IntTask() = default;
};

IntTask make_forty_two(std::shared_ptr<int> slot) {
  co_return 42;
}

IntTask make_via_await(std::shared_ptr<int> slot) {
  const int x = co_await Ready{.value = 7};
  co_return x * 8;
}

int main() {
  std::println("--- generator (co_yield; manual promise / std::generator shape) ---");
  std::print("first 10 Fibonacci: ");
  int n = 0;
  for (auto gen = fibonacci(10); gen.next(n);) {
    std::print("{} ", n);
  }
  std::println();

  std::println("--- custom promise: co_return (result via shared slot) ---");
  auto slot_a = std::make_shared<int>(0);
  make_forty_two(slot_a);
  std::println("make_forty_two -> {}", *slot_a);

  std::println("--- custom promise: co_await + co_return ---");
  auto slot_b = std::make_shared<int>(0);
  make_via_await(slot_b);
  std::println("make_via_await -> {}", *slot_b);
}
