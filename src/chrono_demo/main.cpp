/**
 * @file main.cpp
 * @brief Demo: C++20/23 `<chrono>` — calendar types, durations, formatting.
 *
 * Modern `<chrono>` distinguishes calendar points (`year_month_day`, `weekday`)
 * from durations (`hours`, `minutes`, `hh_mm_ss`) and time points
 * (`system_clock::now()`), and lets us format all of them with the same
 * `std::format` specifiers (`%F`, `%T`, ...). Arithmetic between them is
 * type-checked — subtracting two `sys_days` yields `days`, not a bare integer.
 *
 * Note: Apple libc++ does not yet ship the IANA time-zone database, so
 * `std::chrono::zoned_time` / `current_zone()` are intentionally not used
 * here. Everything below is the untimed, UTC-only subset.
 *
 * Build: `cmake --build <build-dir> --target chrono_demo`
 * Run:   `<build-dir>/src/chrono_demo/chrono_demo`
 */

#include <chrono>
#include <print>

int main() {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  constexpr year_month_day today{2026y, April, 16d};
  std::println("today:           {}", today);
  std::println("today (iso):     {:%F}", today);
  std::println("weekday:         {:%A}", weekday{sys_days{today}});

  constexpr year_month_day last_feb{
      year_month_day_last{2026y, month_day_last{February}}};
  std::println("last of Feb:     {}", last_feb);

  constexpr auto release = sys_days{2026y / June / 1};
  std::println("days until {}:{:>3}", year_month_day{release},
               (release - sys_days{today}).count());

  constexpr auto flight = 8h + 45min + 17s;
  std::println("flight hms:      {}", hh_mm_ss{flight});
  std::println("flight rounded:  {}", round<minutes>(flight));

  const auto now = system_clock::now();
  std::println("utc now:         {:%F %T}", floor<seconds>(now));

  return 0;
}
