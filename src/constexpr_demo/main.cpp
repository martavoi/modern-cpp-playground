/**
 * @file main.cpp
 * @brief Demo: `constexpr` / `consteval` compile-time routing table.
 *
 * Models a tiny slice of a zero-allocation HTTP-style router. The route table
 * lives in a `constexpr` array of string-view rows, `consteval` checks run at
 * compile time, and `match_route` can run at compile time or at runtime.
 * Extending the table with an invalid pattern breaks the `static_assert` on
 * `routes_well_formed`.
 *
 * Build: `cmake --build <build-dir> --target constexpr_demo`
 * Run:   `<build-dir>/src/constexpr_demo/constexpr_demo`
 */

#include <array>
#include <cstdint>
#include <optional>
#include <print>
#include <string_view>
#include <utility>

enum class Method : std::uint8_t { Get, Post };

enum class RouteId : std::uint8_t { Health, Metrics, ApiUsers };

struct RouteEntry {
  Method method;
  std::string_view pattern;
  RouteId id;
};

consteval bool valid_route_pattern(std::string_view pattern) {
  return !pattern.empty() && pattern.front() == '/';
}

template <std::size_t N>
consteval bool routes_well_formed(const std::array<RouteEntry, N>& routes) {
  for (const auto& route : routes) {
    if (!valid_route_pattern(route.pattern)) {
      return false;
    }
  }
  return true;
}

inline constexpr std::array<RouteEntry, 3> k_routes{{
    {Method::Get,  "/health",    RouteId::Health},
    {Method::Get,  "/metrics",   RouteId::Metrics},
    {Method::Post, "/api/users", RouteId::ApiUsers},
}};

static_assert(routes_well_formed(k_routes));

constexpr std::optional<RouteId> match_route(Method method,
                                             std::string_view path) noexcept {
  for (const auto& route : k_routes) {
    if (route.method == method && route.pattern == path) {
      return route.id;
    }
  }
  return std::nullopt;
}

static_assert(match_route(Method::Get,  "/health")    == RouteId::Health);
static_assert(match_route(Method::Post, "/api/users") == RouteId::ApiUsers);
static_assert(!match_route(Method::Get, "/api/users").has_value());
static_assert(!match_route(Method::Get, "/nope").has_value());

constexpr std::string_view method_string(Method method) noexcept {
  using enum Method;
  switch (method) {
    case Get:  return "GET";
    case Post: return "POST";
  }
  std::unreachable();
}

constexpr std::string_view route_name(RouteId id) noexcept {
  using enum RouteId;
  switch (id) {
    case Health:   return "health";
    case Metrics:  return "metrics";
    case ApiUsers: return "api_users";
  }
  std::unreachable();
}

void print_match(Method method, std::string_view path) {
  const auto hit = match_route(method, path);
  std::println("{} {} -> {}", method_string(method), path,
               hit ? route_name(*hit) : "no match");
}

int main() {
  print_match(Method::Get,  "/health");
  print_match(Method::Post, "/api/users");
  print_match(Method::Get,  "/unknown");
  return 0;
}
