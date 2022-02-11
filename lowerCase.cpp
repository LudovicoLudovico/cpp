#include <iostream>

int main()
{
   char letter;
   std::cin >> letter;

   if (letter > 64 && letter < 91)
      letter += 32;

   std::cout << letter << '\n';
}