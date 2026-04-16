/**
 * @file main.cpp
 * @brief Demo: `constexpr` / `consteval` compile-time routing table.
 *
 * Models a tiny slice of a zero-allocation HTTP-style router: routes live in
 * a `constexpr` table of string literals, `consteval` checks run at compile time,
 * and `match_route` can run at compile time or runtime. Extending the table
 * with an invalid pattern fails the `static_assert` on `routes_well_formed`.
 *
 * Build: `cmake --build <build-dir> --target constexpr_demo`
 * Run:   `<build-dir>/src/constexpr_demo/constexpr_demo`
 */

#include <array>
#include <iostream>
#include <optional>
#include <string_view>

/** HTTP-like verb; only what the demo table needs. */
enum class Method : std::uint8_t { Get, Post };

/** Stable id for dispatch after a match (handler table index, enum, etc.). */
enum class RouteId : std::uint8_t { Health, Metrics, ApiUsers };

/** One row in the fixed route table (no owning strings). */
struct RouteEntry {
  Method method;
  std::string_view pattern;
  RouteId id;
};

/** Route paths must be absolute (`/...`) for this toy router. */
consteval bool valid_route_pattern(std::string_view pattern) {
  return !pattern.empty() && pattern[0] == '/';
}

/** Validates every row in a fixed-size route array at compile time. */
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
    {Method::Get, "/health", RouteId::Health},
    {Method::Get, "/metrics", RouteId::Metrics},
    {Method::Post, "/api/users", RouteId::ApiUsers},
}};

static_assert(routes_well_formed(k_routes));

/**
 * Parses a method name known at compile time.
 * Unknown literals make the program ill-formed when this is evaluated in a
 * constant context (e.g. `static_assert`).
 */
consteval Method parse_method(std::string_view token) {
  if (token == "GET") {
    return Method::Get;
  }
  if (token == "POST") {
    return Method::Post;
  }
  throw "unsupported method literal";
}

static_assert(parse_method("GET") == Method::Get);
static_assert(parse_method("POST") == Method::Post);

/** Linear scan match; constexpr so callers can `static_assert` on results. */
constexpr std::optional<RouteId> match_route(Method method,
                                             std::string_view path) noexcept {
  for (const auto& route : k_routes) {
    if (route.method == method && route.pattern == path) {
      return route.id;
    }
  }
  return std::nullopt;
}

static_assert(match_route(Method::Get, "/health") == RouteId::Health);
static_assert(match_route(Method::Post, "/api/users") == RouteId::ApiUsers);
static_assert(!match_route(Method::Get, "/api/users").has_value());
static_assert(!match_route(Method::Get, "/nope").has_value());

constexpr std::string_view method_string(Method method) noexcept {
  switch (method) {
    case Method::Get:
      return "GET";
    case Method::Post:
      return "POST";
  }
  return "?";
}

constexpr std::string_view route_name(RouteId id) noexcept {
  switch (id) {
    case RouteId::Health:
      return "health";
    case RouteId::Metrics:
      return "metrics";
    case RouteId::ApiUsers:
      return "api_users";
  }
  return "unknown";
}

/** Prints `METHOD path -> route` or `no match` using only constexpr data. */
void print_match(Method method, std::string_view path) {
  std::cout << method_string(method) << ' ' << path << " -> ";
  if (const auto hit = match_route(method, path)) {
    std::cout << route_name(*hit) << '\n';
  } else {
    std::cout << "no match\n";
  }
}

int main() {
  print_match(Method::Get, "/health");
  print_match(Method::Post, "/api/users");
  print_match(Method::Get, "/unknown");
  return 0;
}
