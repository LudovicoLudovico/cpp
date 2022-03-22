#include <iostream>
#include <string>

#include "regression.hpp"

void print_fit_result(Result const& res) {
  std::cout << "---- Fit Result ----\n";
  std::cout << "Regression: " << res.B << 'x' << (res.A >= 0 ? '+' : '-')
            << res.A << '\n'
            << '\n';
}

void add_point(Regression& reg) {
  double x;
  double y;
  std::cin >> x >> y;
  if (!std::cin.good()) {
    throw std::runtime_error{"Input not valid (Not a number)"};
  } else {
    reg.add(x, y);
  }
}

int main() {
  std::cout << "-) Press 'a' to add a point \n-) Press 'f' to fit the "
               "points\n-) Press 'q' to quit without saving\n";
  char choice;
  Regression reg;

  while (std::cin.good()) {
    try {
      std::cin >> choice;

      switch (choice) {
        case 'a':
          add_point(reg);
          break;
        case 'f':
          print_fit_result(reg.fit());
          return 0;
        case 'q':
          return 0;
        default:
          const std::string error = "Invalid Symbol: ";
          throw std::runtime_error{error + choice};
      }
    } catch (std::exception const& e) {
      std::cerr << e.what() << '\n';
    }
  }
}
