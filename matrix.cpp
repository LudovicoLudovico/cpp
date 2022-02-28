#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "doctest.h"

class Matrix {
 private:
  std::vector<int> mat_;
  std::string input_;
  int col_per_row_{};
  int row_num_{};

 public:
  Matrix(std::string const& input) : input_{input} {
    std::vector<int> del;
    int col_num = 0;

    // Finding dels
    for (int i = 0; i < input.size(); i++) {
      if (input[i] == '{' || input[i] == '}' || input[i] == ',') {
        if (input[i] == '}') row_num_++;
        if (input[i] == ',' || input[i] == '}') col_num++;
        del.push_back(i);
      }
    }

    col_per_row_ = col_num / row_num_;

    std::vector<int> curr_row;
    for (int i = 0; i < del.size() - 1; i++) {
      if (del[i + 1] - del[i] == 1) continue;
      std::string sub = input.substr(del[i], del[i + 1] - del[i]);
      sub = sub.substr(1, sub.size() - 1);
      mat_.push_back(std::stoi(sub));
    }
  }

  std::vector<int> get_mat() const { return mat_; }
  int get_size() const { return col_per_row_; }

  void print_matrix() const {
    std::cout << "\n--- Matrix ---\n";
    int counter = 0;
    for (int i = 0; i < mat_.size(); i++) {
      std::cout << "|    " << mat_[i] << "     |";
      ++counter;
      if (counter == col_per_row_) {
        std::cout << '\n';
        counter = 0;
      }
    }
  }

  int calculate_trace() const {
    assert(row_num_ == col_per_row_);
    int trace = mat_[0];
    for (int i = 1; i < row_num_; i++) {
      trace += mat_[i * (col_per_row_ + 1)];
    }
    return trace;
  }
};

double determinant(std::vector<int> const& mat, int size) {
  if (size == 2) {
    return mat[0] * mat[3] - mat[1] * mat[2];
  }
  double det = 0;
  for (int i = 0; i < size; i++) {
    std::vector<int> copy = mat;
    std::vector<int> result;
    copy.erase(copy.begin(), copy.begin() + (size));

    int counter = i;
    for (int j = 0; j < copy.size(); j++) {
      if (counter == j) {
        counter += size;
        continue;
      }

      result.push_back(copy[j]);
    }

    det += mat[i] * std::pow(-1, (i + 2)) * determinant(result, size - 1);
  }
  return det;
}

TEST_CASE("Testing Matrix Class") {
  Matrix r{"{1,2,3,5}{4,5,6,8}{7,8,9,10}{1,-3,5,6}"};
  SUBCASE("Printing Matrix") { r.print_matrix(); }
  SUBCASE("Calculating Trace") { CHECK(r.calculate_trace() == 21); }
  SUBCASE("Calculating detemrinant") {
    CHECK(determinant(r.get_mat(), r.get_size()) == -36);
  }
}