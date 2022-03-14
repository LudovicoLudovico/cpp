#include <cmath>
#include <iostream>

int isqrt(int num) {
  for (int i = 1; i * i <= num; ++i)
    if (i * i == num) return i;

  return -1;  // Return -1 is the inserted num is not a perfect square
}

int main() {
  std::cout << "Insert an integer number: ";
  int num;
  std::cin >> num;
  std::cout << isqrt(num) << '\n';
}