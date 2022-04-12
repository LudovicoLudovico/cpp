#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "chain.hpp"

TEST_CASE("Testing Chain Class") {
  Hooke hooke{100, 5};

  SUBCASE("Testing Hooke Normal") {
    PPState p1{1, 0., 0.};
    PPState p2{1, 10., 0.};
    double force = hooke(p1, p2);
    CHECK(force == -500);
  }

  SUBCASE("Testing Hooke with same point") {
    PPState p1{1, 0., 0.};
    PPState p2{1, 0., 0.};
    double force = hooke(p1, p2);
    CHECK(force == 500);
  }

  SUBCASE("Chain class") {
    Chain c{Hooke{0.1, 2.}};
    c.push_back({1., 0., 0.});
    c.push_back({1., 4., 0.});
    const auto state_i = c.state();
    c.evolve(1.0);
    const auto state_f = c.state();
  }
}