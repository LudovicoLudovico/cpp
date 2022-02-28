#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "doctest.h"

template <typename T>
class Matrix {
 private:
  std::vector<T> mat_;
  std::vector<std::vector<T>> input_;
  int col_per_row_{};
  int row_num_{};

 public:
  Matrix(std::vector<std::vector<T>> const& input) : input_{input} {
    for (auto i = 0; i < input_.size(); i++) {
      row_num_++;
      for (auto j = 0; j < input_[i].size(); j++) {
        mat_.push_back(input_[i][j]);
      }
    }

    col_per_row_ = input_[1].size();
  }

  std::vector<T> get_mat() const { return mat_; }
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

  T calculate_trace() const {
    assert(row_num_ == col_per_row_);
    int trace = mat_[0];
    for (int i = 1; i < row_num_; i++) {
      trace += mat_[i * (col_per_row_ + 1)];
    }
    return trace;
  }
};

template <typename T>
double determinant(std::vector<T> const& mat, int size) {
  if (size == 2) {
    return mat[0] * mat[3] - mat[1] * mat[2];
  }
  double det = 0;
  for (int i = 0; i < size; i++) {
    std::vector<T> copy = mat;
    std::vector<T> result;
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
  Matrix<int> a{{{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
  SUBCASE("Printing Matrix") { a.print_matrix(); }
  SUBCASE("Calculating Trace") { CHECK(a.calculate_trace() == 21); }
  SUBCASE("Calculating detemrinant") {
    CHECK(determinant(a.get_mat(), a.get_size()) == -36);
  }
}