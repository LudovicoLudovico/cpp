#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <iostream>
#include <vector>

#include "doctest.h"

template <typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_{};
  int n_row_{};
  int n_col_{};

 public:
  Matrix(std::vector<std::vector<T>> const& mat)
      : matrix_{mat},
        n_row_{static_cast<int>(mat.size())},
        n_col_{static_cast<int>(mat[0].size())} {}
  auto get_n_row() const { return n_row_; };
  auto get_n_col() const { return n_col_; };
  auto get_matrix() const { return matrix_; };
  void print() const {
    std::cout << '\n';
    for (size_t i = 0; i != n_row_; i++) {
      for (size_t j = 0; j != n_col_; j++) {
        std::cout << "|    " << matrix_[i][j] << "    |";
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }

  T trace() const {
    T trace = 0;
    for (size_t i = 0; i < n_row_; i++) {
      trace += matrix_[i][i];
    }
    return trace;
  }

  bool isSymmetric() const {
    if (n_col_ != n_row_) return false;
    for (size_t i = 0; i != n_row_; i++) {
      for (size_t j = 0; j != n_col_; j++) {
        if (matrix_[i][j] != matrix_[j][i]) return false;
      }
    }
    return true;
  }

  auto transpose(int num_to_delete = 0, bool isRow = true) {
    // std::cout << num_to_delete << ' ' << isRow << '\n';
    if (n_col_ == n_row_) {
      int init = 0;
      for (size_t i = 0; i != n_row_; i++) {
        for (size_t j = init; j != n_col_; j++) {
          T a = matrix_[i][j];
          matrix_[i][j] = matrix_[j][i];
          matrix_[j][i] = a;
        }
        init++;
      }

      // Clean the matrix_rix
      if (num_to_delete != 0) {
        if (isRow) {
          for (size_t i = 0; i != num_to_delete; i++) {
            matrix_.pop_back();
          }
          n_row_ -= num_to_delete;
        } else {
          for (size_t i = 0; i != n_row_; i++) {
            for (size_t j = 0; j != num_to_delete; j++) {
              matrix_[i].pop_back();
            }
          }
          n_col_ -= num_to_delete;
        }
      }

      return *this;
    } else if (n_col_ > n_row_) {
      for (size_t i = 0; i != (n_col_ - n_row_); i++) {
        std::vector<int> row(5);
        matrix_.push_back(row);
      }
      int diff = n_col_ - n_row_;
      n_row_ = n_col_;
      return this->transpose(diff, false);
    } else {
      for (size_t i = 0; i != n_row_; i++) {
        for (size_t j = 0; j != (n_row_ - n_col_); j++) {
          matrix_[i].push_back(0);
        }
      }
      int diff = n_row_ - n_col_;
      n_col_ = n_row_;
      return this->transpose(diff, true);
    }
  }
};

template <typename T, typename R>
bool operator==(Matrix<T> const& a, Matrix<R> const& b) {
  auto matA = a.get_matrix();
  auto matB = b.get_matrix();
  for (size_t i = 0; i != a.get_n_row(); i++) {
    for (size_t j = 0; j != a.get_n_col(); j++) {
      if (matA[i][j] != matB[i][j]) return false;
    }
  }
  return true;
}

template <typename T, typename R>
auto operator+(Matrix<T> const& a, Matrix<R> const& b) {
  auto matA = a.get_matrix();
  auto matB = b.get_matrix();
  for (size_t i = 0; i != a.get_n_row(); i++) {
    for (size_t j = 0; j != a.get_n_col(); j++) {
      matA[i][j] += matB[i][j];
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator-(Matrix<T>& a, Matrix<R> const& b) {
  auto matA = a.get_matrix();
  auto matB = b.get_matrix();
  for (size_t i = 0; i != a.get_n_row(); i++) {
    for (size_t j = 0; j != a.get_n_col(); j++) {
      matA[i][j] -= matB[i][j];
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator*(Matrix<T>& a, R scalar) {
  auto matA = a.get_matrix();
  for (size_t i = 0; i != a.get_n_row(); i++) {
    for (size_t j = 0; j != a.get_n_col(); j++) {
      matA[i][j] *= scalar;
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator/(Matrix<T>& a, R scalar) {
  auto matA = a.get_matrix();
  for (size_t i = 0; i != a.get_n_row(); i++) {
    for (size_t j = 0; j != a.get_n_col(); j++) {
      matA[i][j] /= scalar;
    }
  }
  return Matrix{matA};
}

template <typename T>
Matrix<T> create_matrix_from_dimensions(int size) {
  std::vector<std::vector<T>> mat{};
  for (size_t i = 0; i != size; i++) {
    std::vector<T> row(size);
    mat.push_back(row);
  }
  return Matrix{mat};
}

TEST_CASE("Testing Matrix") {
  Matrix<int> a{{{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
  SUBCASE("Printing matrix") { a.print(); }
  SUBCASE("Calculating Trace") { CHECK(a.trace() == 21); }
  SUBCASE("Testing == operator") {
    Matrix<double> b{
        {{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
    Matrix<double> c{
        {{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 2, 6}}};
    CHECK((a == b));
    CHECK(!(a == c));
  }
  SUBCASE("Testing + operator") {
    Matrix<double> b{
        {{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
    CHECK((a + b) == Matrix<int>{{{2, 4, 6, 10},
                                  {8, 10, 12, 16},
                                  {14, 16, 18, 20},
                                  {2, -6, 10, 12}}});
  }
  SUBCASE("Testing - operator") {
    Matrix<double> b{
        {{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
    CHECK(
        (a - b) ==
        Matrix<int>{{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}});
  }
  SUBCASE("Testing * scalar operator") { CHECK((a * 2) == (a + a)); }
  SUBCASE("Testing * scalar operator") { CHECK((a / 1) == a); }
  SUBCASE("Testing isSymmetric") {
    CHECK(!a.isSymmetric());

    Matrix<double> b{{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    CHECK(b.isSymmetric());
  }
  SUBCASE("Transpose matrix") {
    CHECK(a.transpose() ==
          Matrix<int>{
              {{1, 4, 7, 1}, {2, 5, 8, -3}, {3, 6, 9, 5}, {5, 8, 10, 6}}});
    Matrix<int> b{{{1, 2}, {3, 4}, {4, 5}}};

    CHECK(b.transpose() == Matrix<int>{{{1, 3, 4}, {2, 4, 5}}});
  }
  SUBCASE("Create matrix from dimensions") {
    CHECK(create_matrix_from_dimensions<int>(2) ==
          Matrix<int>{{{0, 0}, {0, 0}}});
  }
}