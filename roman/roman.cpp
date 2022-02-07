#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <string>
#include <iostream>
#include <stdexcept>

class Roman
{
private:
   int num_ = 0;
   std::string result_ = "";

public:
   void setNum(int number)
   {
      num_ = number;
   }

   void checkIsDivisible(int divider, char std_symbol, std::string alt_symbol)
   {
      if ((num_ + 1) / divider >= 1)
      {
         if (num_ / divider >= 1)
         {
            num_ -= divider;
            result_ += std_symbol;
         }
         else
         {
            num_ -= (divider - 1);
            result_ += alt_symbol;
         }
      }
   }

   std::string getRomanized()
   {
      if (num_ == 0)
         throw std::runtime_error{"A value has either not been inserted or is zero"};

      while (num_ > 3)
      {
         if (num_ >= 999)
         {
            checkIsDivisible(1000, 'M', "CMXCIX");
            continue;
         }
         if (num_ >= 99)
         {
            checkIsDivisible(100, 'C', "XCIX");
            continue;
         }

         if (num_ >= 49)
         {
            checkIsDivisible(50, 'L', "XLIX");
            continue;
         }

         if (num_ >= 9)
         {
            checkIsDivisible(10, 'X', "IX");
            continue;
         }

         if (num_ >= 4)
         {
            checkIsDivisible(5, 'V', "IV");
            continue;
         }
      }

      while (num_ != 0)
      {
         result_ += 'I';
         num_ -= 1;
      }

      return result_;
   }
};

TEST_CASE("Testing Roman Class")
{
   Roman r;

   SUBCASE("Romanize without number") { CHECK_THROWS(r.getRomanized()); }
   SUBCASE("Convert number 3")
   {
      r.setNum(3);
      std::string result = r.getRomanized();
      CHECK(result == "III");
   }

   SUBCASE("Convert number 4")
   {
      r.setNum(4);
      std::string result = r.getRomanized();
      CHECK(result == "IV");
   }

   SUBCASE("Convert number 5")
   {
      r.setNum(5);
      std::string result = r.getRomanized();
      CHECK(result == "V");
   }

   SUBCASE("Convert number 7")
   {
      r.setNum(7);
      std::string result = r.getRomanized();
      CHECK(result == "VII");
   }
   SUBCASE("Convert number 19")
   {
      r.setNum(19);
      std::string result = r.getRomanized();
      CHECK(result == "XIX");
   }
   SUBCASE("Convert number 26")
   {
      r.setNum(26);
      std::string result = r.getRomanized();
      CHECK(result == "XXVI");
   }
   SUBCASE("Convert number 51")
   {
      r.setNum(51);
      std::string result = r.getRomanized();
      CHECK(result == "LI");
   }
   SUBCASE("Convert number 49")
   {
      r.setNum(49);
      std::string result = r.getRomanized();
      CHECK(result == "XLIX");
   }
   SUBCASE("Convert number 99")
   {
      r.setNum(99);
      std::string result = r.getRomanized();
      CHECK(result == "XCIX");
   }
   SUBCASE("Convert number 108")
   {
      r.setNum(108);
      std::string result = r.getRomanized();
      CHECK(result == "CVIII");
   }
   SUBCASE("Convert number 999")
   {
      r.setNum(999);
      std::string result = r.getRomanized();
      CHECK(result == "CMXCIX");
   }
   SUBCASE("Convert number 1234")
   {
      r.setNum(1234);
      std::string result = r.getRomanized();
      CHECK(result == "MCCXXXIV");
   }
}
