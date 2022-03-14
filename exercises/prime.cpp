#include <iostream>

bool isPrime(int num) {
  for (int i = 2; i != num / 2; i++) {
    if (num % i == 0) return false;
  }
  return true;
}

int main() {
  std::cout << "Insert a number: \n";
  int num;
  std::cin >> num;
  std::cout << (isPrime(num) ? "Prime" : "Not Prime");
}