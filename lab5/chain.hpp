#ifndef CHAIN_HPP
#define CHAIN_HPP
#include <iostream>
#include <vector>
struct PPState {
  double m{};
  double x{};
  double v{};
  void print() { std::cout << m << '-' << x << '-' << v << '\n'; }
};

class Hooke {
 private:
  double k_{};
  double l_{};

 public:
  Hooke(double k, double l);
  double operator()(PPState const& p1, PPState const& p2) const;
};

class Chain {
 private:
  Hooke inter_;
  std::vector<PPState> ppses_;

 public:
  Chain(Hooke const& hooke);
  void push_back(PPState const& pps);
  int size() const;
  void evolve(double delta_t);
  std::vector<PPState> const& state() const;
};

#endif