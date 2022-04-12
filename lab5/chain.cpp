#include <cmath>
#include <iostream>

#include "chain.hpp"

Hooke::Hooke(double k, double l) : k_{k}, l_{l} {}
double Hooke::operator()(PPState const& p1, PPState const& p2) const {
  return k_ * (l_ - std::abs(p1.x - p2.x));
}
Chain::Chain(Hooke const& hooke) : inter_{hooke} {}
void Chain::push_back(PPState const& pps) { ppses_.push_back(pps); }
int Chain::size() const { return ppses_.size(); }
void Chain::evolve(double delta_t) {
  auto copy = ppses_;
  for (int i = 0; i != size(); i++) {
    double f = 0;
    if (i == 0) f = inter_(copy[i], copy[i + 1]);
    if (i == size() - 1) f = inter_(copy[i - 1], copy[i]);

    if (f == 0) f = inter_(copy[i - 1], copy[i]) + inter_(copy[i], copy[i + 1]);

    double a = f;
    ppses_[i].x += copy[i].v * delta_t + 0.5 * a * delta_t * delta_t;
    ppses_[i].v += a * delta_t;
  }
}
std::vector<PPState> const& Chain::state() const { return ppses_; }