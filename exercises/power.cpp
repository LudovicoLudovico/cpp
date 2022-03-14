#include <iostream>

auto pow(int base, int exp) {
  int result = 1;
  for (auto i = 0; i != exp; i++) {
    result *= base;
  }
  return result;
}

int main() {
  std::cout << "Insert the base and the exponent: \n";
  int base = 0;
  int exponent = 0;
  std::cin >> base >> exponent;
  std::cout << pow(base, exponent) << '\n';
}