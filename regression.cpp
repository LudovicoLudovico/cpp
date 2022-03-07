#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>
#include <stdexcept>
#include <vector>

#include "doctest.h"

template <typename T>
struct Point {
  T X;
  T Y;
  bool operator==(Point const& b) { return b.X == X && b.Y == Y; }
};
template <typename T>
struct RegressionResultY {
  T A;
  T sigma_A;
  T B;
  T sigma_B;
  T sigma_Y;
};

template <typename T>
class Regression {
 private:
  std::vector<Point<T>> data_{};

 public:
  int size() const noexcept { return data_.size(); }
  bool empty() const noexcept { return data_.size() == 0; }
  void add(T X, T Y) { data_.push_back(Point<T>{X, Y}); }
  void add(Point<T> point) { data_.push_back(point); }
  bool remove(Point<T> point) {
    for (auto current = data_.begin(), last = data_.end(); current != last;
         ++current) {
      if (*current == point) {
        data_.erase(current);
        return true;
      }
    }

    return false;
  }
  bool remove(T X, T Y) {
    for (auto current = data_.begin(), last = data_.end(); current != last;
         ++current) {
      if (current->X == X && current->Y == Y) {
        data_.erase(current);
        return true;
      }
    }

    return false;
  }
  RegressionResultY<T> fit(T sigma_y = 0) {
    int const N = data_.size();
    T sum_x{};
    T sum_x2{};
    T sum_y{};
    T sum_xy{};
    if (N < 2) throw std::runtime_error{"Not enough points to run a fit"};

    for (auto& point : data_) {
      sum_x += point.X;
      sum_x2 += point.X * point.X;
      sum_y += point.Y;
      sum_xy += point.X * point.Y;
    }
    T const d = N * sum_x2 - sum_x * sum_x;
    if (d == 0.) throw std::runtime_error{"Trying to fit a vertical line"};

    T const a = (sum_y * sum_x2 - sum_x * sum_xy) / d;
    T const b = (N * sum_xy - sum_x * sum_y) / d;

    T sum_a_b{};
    for (auto& point : data_) {
      sum_a_b += (point.Y - a - (b * point.X)) * (point.Y - a - (b * point.X));
    }

    if (sigma_y == 0) sigma_y = sqrt(sum_a_b / (N - 2));
    T const sigma_a = sigma_y * sqrt(sum_x2 / (N * sum_x2 - (sum_x * sum_x)));
    T const sigma_b = sigma_y * sqrt(N / (N * sum_x2 - (sum_x * sum_x)));
    return {a, sigma_a, b, sigma_b, sigma_y};
  }
};

TEST_CASE("Testing Regression") {
  Regression<double> reg;

  SUBCASE("Remove a point from empty Regression") {
    CHECK(reg.remove(0., 0.) == false);
    CHECK(reg.empty());
  }
  SUBCASE("Remove a point from empty Regression with removePoint") {
    auto to_remove = Point<double>{0., 0.};
    CHECK(reg.remove(to_remove) == false);
    CHECK(reg.empty());
  }
  SUBCASE("Remove a point previously inserted") {
    reg.add(0., 0.);
    reg.add(1., 1.);
    CHECK(reg.remove(0., 0.) == true);
    CHECK(reg.size() == 1);
  }
  SUBCASE("Remove a point previously inserted with removePoint") {
    reg.add(Point<double>{0., 0.});
    reg.add(Point<double>{1., 1.});
    CHECK(reg.remove(Point<double>{0., 0.}) == true);
    CHECK(reg.size() == 1);
  }
  SUBCASE("Remove a point not inserted") {
    reg.add(0., 0.);
    reg.add(1., 1.);
    CHECK(reg.remove(1., 0.) == false);
    CHECK(reg.size() == 2);
  }
  SUBCASE("Remove a point not inserted with removePoint") {
    reg.add(Point<double>{0., 0.});
    reg.add(Point<double>{1., 1.});
    CHECK(reg.remove(Point<double>{1., 0.}) == false);
    CHECK(reg.size() == 2);
  }
  SUBCASE("Remove twice a point inserted once") {
    reg.add(0., 0.);
    reg.add(1., 1.);
    CHECK(reg.remove(0., 0.) == true);
    CHECK(reg.remove(0., 0.) == false);
    CHECK(reg.size() == 1);
  }
  SUBCASE("Adding 3 points, removing one and fitting") {
    reg.add(0., 0.);
    reg.add(1., 1.);
    reg.add(.5, 3.);
    CHECK(reg.remove(0.5, 3.) == true);
    CHECK(reg.size() == 2);
    auto [A, simga_A, B, sigma_B, sigma_Y] = reg.fit();
    CHECK(A == doctest::Approx(0));
    CHECK(B == doctest::Approx(1));
  }
}