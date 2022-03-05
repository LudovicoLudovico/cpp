#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <cmath>
#include <iostream>
#include <ratio>
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

template <typename T>
struct GaussResult {
  Matrix<T> r;
  int reorder;
};

template <typename T>
GaussResult<T> gauss(Matrix<T> const& a) {
  auto b = a;
  auto mat = b.get_matrix();
  auto n_col = b.get_n_col();
  auto n_row = b.get_n_row();
  int offset = 0;
  int line = 0;
  int num_of_reorders = 0;

  for (size_t i = 0; i != n_col; i++) {
    // Reordering vector
    for (size_t i = 0; i != n_row - 1; i++) {
      int allowed_zero = i;
      int zeros = 0;
      for (size_t j = 0; j != n_col; j++) {
        if (mat[i][j] != 0)
          break;
        else
          ++zeros;
      }

      if (zeros == n_col) {
        mat.erase(mat.begin() + i);
        --n_row;
        --n_col;
        break;
      }
      if (zeros > allowed_zero) {
        auto row = mat[i + 1];
        mat[i + 1] = mat[i];
        mat[i] = row;
        ++num_of_reorders;
      }
    }

    // Gauss
    for (size_t i = line; i != n_row - 1; i++) {
      if (mat[i + 1][offset] != 0 && mat[line][offset] != 0) {
        double c =
            -1.0 * (double(mat[i + 1][offset]) / double(mat[line][offset]));
        for (size_t j = offset; j != n_col; j++) {
          mat[i + 1][j] += c * mat[line][j];
        }
      }
    }
    ++line;
    ++offset;
  }

  return {Matrix{mat}, num_of_reorders};
}

template <typename T>
auto determinant(Matrix<T> const& a) {
  auto b = gauss(a);
  auto mat = b.r.get_matrix();
  double det = 1;
  for (size_t i = 0; i != b.r.get_n_row(); i++) {
    for (size_t j = 0; j != b.r.get_n_col(); j++) {
      if (mat[i][j] != 0) {
        det *= mat[i][j];
        break;
      }
    }
  }

  if (b.reorder % 2 != 0) {
    det = -det;
  }
  return det;
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
  SUBCASE("Gauss") {
    Matrix<double> b{{{1, 2, 3}, {1, 2, 3}, {0, 0, 1}}};
    auto c = gauss(b);
    c.r.print();

    auto d = gauss(a);
    d.r.print();

    Matrix<double> f{{{1, 2, 3}, {1, 2, 3}, {1, 2, 2}}};
    auto g = gauss(f);
    g.r.print();
  }
  SUBCASE("Calculating determinant") {
    Matrix<int> simple{{{1, 2}, {3, 4}}};
    CHECK(determinant(simple) == -2);
    CHECK(determinant(a) == -36);
  }
}