#include <iostream>

int main()
{
   int smallest;
   std::cin >> smallest;

   int num;
   while (std::cin >> num && std::cin.good())
   {
      if (num < smallest)
         smallest = num;
   }

   std::cout << "The smallest number is:" << smallest << '\n';
}