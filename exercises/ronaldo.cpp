#include <iostream>

int main() {
  int n{};
  std::cin >> n;
  std::string sium = "si";
  for (int i = 0; i != n; ++i) {
    sium += 'u';
  }
  sium += 'm';
  std::cout << sium << '\n';
}