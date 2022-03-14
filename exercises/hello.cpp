#include <iostream>

int main() {
  std::string name;
  std::cin >> name;  // Note that if you put name and surname separated by a
                     // space only the first name is stored
  std::cout << "Hello " << name << "!\n";
}