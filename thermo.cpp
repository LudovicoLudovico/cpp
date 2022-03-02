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

int main() {
  std::cout << "Select one of these functionalities: \n (1) - Calculate the "
               "mean \n (2) - Calculate the center of interval \n (3) - "
               "Parital dispertion \n (4) - Standard deviation \n (5) - "
               "Variance \n (6) - Linear Fit (A and B)"
            << std::endl;

  int user_choice;
  std::cin >> user_choice;

  if (user_choice != 6) {
    std::vector<double> data;
    std::cout << "Insert your data (Enter a letter to stop inserting): \n";

    double user_data;
    while (std::cin >> user_data) {
      data.push_back(user_data);
    }

    double result;
    std::string operation = "";
    switch (user_choice) {
      case 1:
        result = mean(data);
        operation = "mean";
        break;
      case 2:
        result = interval_center(data);
        operation = "center of the interval";
        break;
      case 3:
        result = partial_dispertion(data);
        operation = "partial dispertion";
        break;
      case 4:
        result = standard_deviation(data);
        operation = "standard deviation";
        break;
      case 5:
        result = variance(data);
        operation = "variance";
        break;
      default:
        return EXIT_FAILURE;
        break;
    }

    std::cout << "The " << operation << " of your data set is:  " << result
              << std::endl;
  } else {
    std::vector<double> x;
    std::cout << "Insert values for x: \n";
    double x_ins;
    while (std::cin >> x_ins) {
      x.push_back(x_ins);
    };

    std::cin.clear();
    std::cin.ignore(1, '\n');

    std::vector<double> y;
    std::cout << "Insert values for y: \n";
    double y_ins;
    while (std::cin >> y_ins && y.size() < x.size()) {
      y.push_back(y_ins);
    };

    std::cin.clear();
    std::cin.ignore(1, '\n');

    std::cout << "Insert the error on y values: ";
    double sigma_y;
    std::cin >> sigma_y;

    if (std::cin.fail()) {
      return EXIT_FAILURE;
    }

    std::vector result = calculate_a_b(x, y, sigma_y);
    std::cout << "\nA: " << result[0] << " ± " << result[1]
              << "\nB: " << result[2] << " ± " << result[3] << std::endl;
  }

  return 0;
}