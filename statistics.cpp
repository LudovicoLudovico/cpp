#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

template <typename T>
struct Point {
  T X;
  T Y;
};
template <typename T>
struct RegressionResult {
  T A;
  T sA;
  T B;
  T sB;
};
template <typename T>
struct RegressionResultY {
  T A;
  T sA;
  T B;
  T sB;
  T sY;
};

template <typename T>
struct MaxMin {
  T min;
  T max;
};
template <typename T>
class Statistics {
 private:
  int N_{};
  T sum_x_{};
  T sum_x2_{};
  T sum_y_{};
  T sum_xy_{};
  std::vector<Point<T>> data_points_{};
  std::vector<T> data_{};

 public:
  int size() const { return N_; };
  void add(std::vector<Point<T>> const& new_data) {
    data_points_ = new_data;
    N_ = new_data.size();
  };

  void add(std::vector<T> const& new_data) {
    data_ = new_data;
    N_ = new_data.size();
  }

  auto w_mean() const {
    if (N_ == 0)
      throw std::runtime_error{"Can't calculate mean without values"};
    T sum{};
    T w_sum{};
    for (auto& point : data_points_) {
      if (point.Y <= 0) throw std::runtime_error{"Go fucking study"};
      sum += point.X * point.Y;
      w_sum += point.Y;
    }

    return (sum / w_sum);
  }

  T mean() const {
    if (N_ == 0)
      throw std::runtime_error{"Can't calculate mean without values"};
    T sum{};
    for (auto& value : data_) {
      sum += value;
    }

    return (sum / N_);
  }

  auto get_max_min() const {
    if (N_ == 0)
      throw std::runtime_error{"Can't calculate min and max without values"};
    T max = data_[0];
    T min = data_[0];
    for (auto& num : data_) {
      if (num > max) max = num;
      if (num < min) min = num;
    }

    return MaxMin<T>{min, max};
  }

  T interval_center() {
    MaxMin result = get_max_min();
    return ((result.max + result.min) / 2);
  }

  T partial_dispertion() {
    MaxMin result = get_max_min();
    return ((result.max - result.min) / 2);
  }

  T standard_deviation() {
    T calculated_mean = mean();

    T sum_of_difference;
    for (auto& num : data_) {
      sum_of_difference += (num - calculated_mean) * (num - calculated_mean);
    }

    return sqrt(sum_of_difference / (N_ - 1));
  }
  T standard_deviation_mean() {
    T calculated_mean = mean();

    T sum_of_difference;
    for (auto& num : data_) {
      sum_of_difference += (num - calculated_mean) * (num - calculated_mean);
    }

    return (sqrt(sum_of_difference / (N_ - 1))) / sqrt(N_);
  }

  T variance() {
    T calculated_standard_deviations = standard_deviation();
    return (calculated_standard_deviations * calculated_standard_deviations);
  }

  RegressionResult<T> fit(T sigma_y) {
    if (N_ < 2) throw std::runtime_error{"Not enough points to run a fit"};

    for (auto& point : data_points_) {
      sum_x_ += point.X;
      sum_x2_ += point.X * point.X;
      sum_y_ += point.Y;
      sum_xy_ += point.X * point.Y;
    }
    T const d = N_ * sum_x2_ - sum_x_ * sum_x_;
    if (d == 0.) throw std::runtime_error{"Trying to fit a vertical line"};

    T const a = (sum_y_ * sum_x2_ - sum_x_ * sum_xy_) / d;
    T const b = (N_ * sum_xy_ - sum_x_ * sum_y_) / d;
    T const sigma_a =
        sigma_y * sqrt(sum_x2_ / (N_ * sum_x2_ - (sum_x_ * sum_x_)));
    T const sigma_b = sigma_y * sqrt(N_ / (N_ * sum_x2_ - (sum_x_ * sum_x_)));
    return {a, sigma_a, b, sigma_b};
  }
  RegressionResultY<T> fit() {
    if (N_ < 2) throw std::runtime_error{"Not enough points to run a fit"};

    for (auto& point : data_points_) {
      sum_x_ += point.X;
      sum_x2_ += point.X * point.X;
      sum_y_ += point.Y;
      sum_xy_ += point.X * point.Y;
    }
    T const d = N_ * sum_x2_ - sum_x_ * sum_x_;
    if (d == 0.) throw std::runtime_error{"Trying to fit a vertical line"};

    T const a = (sum_y_ * sum_x2_ - sum_x_ * sum_xy_) / d;
    T const b = (N_ * sum_xy_ - sum_x_ * sum_y_) / d;

    T sum_a_b{};
    for (auto& point : data_points_) {
      sum_a_b += (point.Y - a - (b * point.X)) * (point.Y - a - (b * point.X));
    }

    T sigma_y = sqrt(sum_a_b / (N_ - 2));
    T const sigma_a =
        sigma_y * sqrt(sum_x2_ / (N_ * sum_x2_ - (sum_x_ * sum_x_)));
    T const sigma_b = sigma_y * sqrt(N_ / (N_ * sum_x2_ - (sum_x_ * sum_x_)));
    return {a, sigma_a, b, sigma_b, sigma_y};
  }
};

TEST_CASE("Testing Regression") {
  Statistics<double> reg;

  REQUIRE(reg.size() == 0);

  SUBCASE("Fitting with no points throws") { CHECK_THROWS(reg.fit(1)); }

  SUBCASE("Fitting on one point throws") {
    reg.add({Point<double>{.0, .0}});
    CHECK_THROWS(reg.fit(1));
  }

  SUBCASE("Fitting on two, vertically aligned, points throws") {
    reg.add({{1., 0.}, {1., 2.}});
    CHECK_THROWS(reg.fit(1));
  }

  SUBCASE("Fitting on two points") {
    reg.add({{0., 0.}, {1., 1.}});
    auto result = reg.fit(1);
    CHECK(result.A == doctest::Approx(0));
    CHECK(result.B == doctest::Approx(1));
  }

  SUBCASE("Fitting on two points, negative slope") {
    reg.add({{0., 1.}, {1., 0.}});
    auto result = reg.fit(1);
    CHECK(result.A == doctest::Approx(1));
    CHECK(result.B == doctest::Approx(-1));
  }

  SUBCASE("Fitting on five points") {
    reg.add({{2.1, 3.2}, {6.9, 7.3}, {2.2, 3.0}, {0.1, 1.3}, {4.7, 5.6}});
    auto result = reg.fit(1);
    CHECK(result.A == doctest::Approx(1.2).epsilon(0.01));
    CHECK(result.B == doctest::Approx(0.9).epsilon(0.01));
  }
  SUBCASE("Checking sigmas") {
    reg.add({{50, 12.6}, {100, 14.8}, {150, 18.2}});
    auto result = reg.fit(0.5);
    CHECK(result.A == doctest::Approx(9.6).epsilon(0.1));
    CHECK(result.B == doctest::Approx(0.056).epsilon(0.01));
    CHECK(result.sA == doctest::Approx(0.76).epsilon(0.01));
    CHECK(result.sB == doctest::Approx(0.007).epsilon(0.001));
  }
  SUBCASE("Approximating sigma Y") {
    reg.add({{50, 12.6}, {100, 14.8}, {150, 18.2}});
    auto result = reg.fit();
    CHECK(doctest::Approx(result.sY).epsilon(0.00001) == 0.48989);
  }
}

TEST_CASE("Mean") {
  Statistics<double> r;

  SUBCASE("Weighted mean without values") { CHECK_THROWS(r.w_mean()); }
  SUBCASE("Weighted mean with negative weight value") {
    r.add({{25, -1}, {29, 0}});
    CHECK_THROWS(r.w_mean());
  }
  SUBCASE("Weighted mean") {
    r.add({{25, 10}, {27, 9}});
    CHECK(doctest::Approx(r.w_mean()).epsilon(0.01) == 25.95);
  }
  SUBCASE("Normal mean") {
    r.add({10, 20, 30});
    CHECK(doctest::Approx(r.mean()).epsilon(0.1) == 20);
  }
}

TEST_CASE("Testing MaxMin") {
  Statistics<long double> r;
  SUBCASE("Testing max min on 0 values") { CHECK_THROWS(r.get_max_min()); }
  SUBCASE("Testing max min") {
    r.add({0, 1, 1, 2, 3, 4, 1, 1, -1, 10});
    auto result = r.get_max_min();
    CHECK(result.min == -1);
    CHECK(result.max == 10);
  }
  SUBCASE("Another one") {
    r.add({10, 12, 23, 23, 16, 23, 21, 16});
    auto result = r.get_max_min();
    CHECK(result.min == 10);
    CHECK(result.max == 23);
  }
}

TEST_CASE("Testing statistics") {
  Statistics<long double> r;
  SUBCASE("Everything") {
    r.add({10, 12, 23, 23, 16, 23, 21, 16});
    CHECK(doctest::Approx(r.standard_deviation()).epsilon(0.001) == 5.237);
    CHECK(doctest::Approx(r.standard_deviation_mean()).epsilon(0.001) == 1.851);
    CHECK(doctest::Approx(r.variance()).epsilon(0.01) == 27.42);
    CHECK(doctest::Approx(r.interval_center()).epsilon(0.1) == 16.5);
    CHECK(doctest::Approx(r.partial_dispertion()).epsilon(0.1) == 6.5);
  }
}