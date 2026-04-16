/**
 * @file main.cpp
 * @brief Demo: `std::jthread` cooperative cancellation with `std::stop_token`.
 *
 * `std::jthread` auto-`join`s on destruction (unlike `std::thread`) and, when
 * the callable takes a `std::stop_token` first parameter, the runtime can
 * ask it to stop via `request_stop()`. A binary semaphore is used only to
 * pin down the print order so the output is deterministic.
 *
 * Build: `cmake --build <build-dir> --target jthread_demo`
 * Run:   `<build-dir>/src/jthread_demo/jthread_demo`
 */

#include <print>
#include <semaphore>
#include <thread>

int main() {
  std::binary_semaphore worker_started{0};

  std::jthread worker{[&](std::stop_token st) {
    std::println("worker: running");
    worker_started.release();

    while (!st.stop_requested()) {
      std::this_thread::yield();
    }
    std::println("worker: exiting after stop request");
  }};

  worker_started.acquire();
  std::println("main:   requesting stop");
  worker.request_stop();
  return 0;
}
