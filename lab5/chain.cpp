#include <cmath>
#include <iostream>

#include "chain.hpp"

Hooke::Hooke(double k, double l) : k_{k}, l_{l} {}
double Hooke::operator()(PPState const& p1, PPState const& p2) const {
  return k_ * (std::abs(p1.x - p2.x) - l_);
}
Chain::Chain(Hooke const& hooke) : inter_{hooke} {}
void Chain::push_back(PPState const& pps) { ppses_.push_back(pps); }
int Chain::size() const { return ppses_.size(); }
void Chain::evolve(double delta_t) {
  auto copy = ppses_;
  for (int i = 0; i != size(); i++) {
    double a = 0;
    if (i == 0) {
      double f_next = inter_(copy[i], copy[i + 1]);
      a = f_next / copy[i].m;
    } else if (i == size() - 1) {
      double f_previous = -inter_(copy[i - 1], copy[i]);
      a = f_previous / copy[i].m;
    } else {
      double f_previous = -inter_(copy[i - 1], copy[i]);
      double f_next = inter_(copy[i], copy[i + 1]);
      double force = f_next + f_previous;
      a = force / copy[i].m;
    }

    copy[i].x += copy[i].v * delta_t + 0.5 * a * delta_t * delta_t;
    copy[i].v += a * delta_t;
  }
  ppses_ = copy;
}
std::vector<PPState> const& Chain::state() const { return ppses_; }