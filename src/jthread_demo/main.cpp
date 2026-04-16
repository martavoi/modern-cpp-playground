/**
 * @file main.cpp
 * @brief Demo: `std::jthread` cooperative cancellation with `std::stop_token`.
 *
 * `std::jthread` automatically `join`s on destruction (unlike `std::thread`).
 * If the callable accepts a `std::stop_token`, the runtime can call
 * `request_stop()` to ask the thread to finish; `stop_requested()` polls that
 * request. Here a mutex orders console output so lines are deterministic.
 *
 * Build: `cmake --build <build-dir> --target jthread_demo`
 * Run:   `<build-dir>/src/jthread_demo/jthread_demo`
 */

#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>

int main() {
  std::mutex log_mutex;
  std::binary_semaphore started{0};

  std::jthread worker{[&](std::stop_token st) {
    started.release();
    while (!st.stop_requested()) {
      std::this_thread::yield();
    }
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "worker exited after stop\n";
  }};

  started.acquire();
  std::cout << "worker is running\n";
  {
    std::lock_guard<std::mutex> lock(log_mutex);
    worker.request_stop();
    std::cout << "stop requested\n";
  }
  return 0;
}
