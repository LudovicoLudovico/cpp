#include <iostream>

int main() {
  int smallest = 0;
  while (std::cin.good()) {
    int newNumber;
    std::cin >> newNumber;
    if (smallest == 0 || newNumber < smallest) smallest = newNumber;
  }

  std::cout << smallest;
}