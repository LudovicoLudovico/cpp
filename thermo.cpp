#include <cmath>
#include <iostream>
#include <vector>

#include "statistics.hpp"

// Calculate mean
double mean(std::vector<double> &vect) {
  double sum;
  int i = 0;
  for (auto &num : vect) {
    sum += num;
    ++i;
  }

  return (sum / i);
}

std::vector<double> get_max_min(std::vector<double> &vect) {
  double max;
  double min;
  int i = 0;
  for (auto &num : vect) {
    if (i == 0) {
      max = num;
      min = num;
    } else {
      if (num > max) {
        max = num;
      }

      if (num < min) {
        min = num;
      }
    }
    ++i;
  }

  return {max, min};
}

double interval_center(std::vector<double> &vect) {
  std::vector<double> result = get_max_min(vect);

  return ((result[0] + result[1]) / 2);
}

double partial_dispertion(std::vector<double> &vect) {
  std::vector<double> result = get_max_min(vect);

  return ((result[0] - result[1]) / 2);
}

double standard_deviation(std::vector<double> &vect) {
  double calculated_mean = mean(vect);

  double sum_of_difference;
  int i;
  for (auto &num : vect) {
    sum_of_difference += (num - calculated_mean) * (num - calculated_mean);
    ++i;
  }

  return sqrt(sum_of_difference / (vect.size() - 1));
}

double variance(std::vector<double> &vect) {
  double calculated_standard_deviations = standard_deviation(vect);
  return (calculated_standard_deviations * calculated_standard_deviations);
}

std::vector<double> calculate_a_b(std::vector<double> &x,
                                  std::vector<double> &y, double sigma_y) {
  double sum_x;
  double sum_x_squared;
  double sum_y;
  double sum_x_y;
  double a;
  double b;
  int i = 0;

  for (auto &sx : x) {
    sum_x += sx;
    sum_x_squared += sx * sx;
    sum_y += y[i];
    sum_x_y += sx * y[i];
    ++i;
  }

  a = (sum_x_squared * sum_y - sum_x * sum_x_y) /
      (x.size() * sum_x_squared - (sum_x * sum_x));
  b = (x.size() * sum_x_y - sum_x * sum_y) /
      (x.size() * sum_x_squared - (sum_x * sum_x));

  double sigma_a = sigma_y * sqrt(sum_x_squared /
                                  (x.size() * sum_x_squared - (sum_x * sum_x)));
  double sigma_b =
      sigma_y * sqrt(x.size() / (x.size() * sum_x_squared - (sum_x * sum_x)));

  return {a, sigma_a, b, sigma_b};
}

int main() { return 0; }