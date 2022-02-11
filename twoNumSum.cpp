#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <string>
#include <vector>
#include <stdexcept>

class TwoNumberSum
{
private:
   std::vector<int> list_ = {};
   int target_sum = 0;

public:
   void set_list(std::vector<int> const &new_list)
   {
      list_ = new_list;
   }
   void set_target_sum(int sum)
   {
      target_sum = sum;
   }

   // Runs in O(nlog(n))
   std::vector<int> sum_with_reorder()
   {
      if (list_.size() == 0)
         throw std::runtime_error{"A list of numbers has not been provided"};

      sort(list_.begin(), list_.end());

      int left = 0;
      int right = list_.size() - 1;

      while (list_[left] + list_[right] != target_sum)
      {
         if (list_[left] + list_[right] == target_sum)
            return {list_[left], list_[right]};
         if (list_[left] + list_[right] < target_sum)
            left += 1;
         if (list_[left] + list_[right] > target_sum)
            right -= 1;
      }

      return {};
   }
};

TEST_CASE("Testing two number sum algorithm")
{
   TwoNumberSum sum;

   SUBCASE("Not providing anything") { CHECK_THROWS(sum.sum_with_reorder()); }
   SUBCASE("Test 1")
   {
      sum.set_list({3, 5, -4, 8, 11, 1, -1, 6});
      sum.set_target_sum(10);
      std::vector<int> result = sum.sum_with_reorder();

      CHECK(result[0] == -1);
      CHECK(result[1] == 11);
   }
   // SUBCASE("Test Empty")
   // {
   //    sum.set_list({-4, 8, 11, 1, -1, 6});
   //    sum.set_target_sum(9);
   //    std::vector<int> result = sum.sum_with_reorder();
   //    CHECK(result.size() == 0);
   // }
   // SUBCASE("Repeating value")
   // {
   //    sum.set_list({1, 2, 3});
   //    sum.set_target_sum(6);
   //    std::vector<int> result = sum.sum_with_reorder();
   //    std::cout << result[0];
   //    std::cout << result[1];
   //    CHECK(result.size() == 0);
   // }
}