#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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
      : matrix_{mat}, n_row_{mat.size()}, n_col_{mat[0].size()} {}
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
T determinant(Matrix<T> const& a) {
  auto mat = a.get_matrix();
  if (a.get_n_row() == 2) {
    return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
  } else {
    return -1;
  }
}

template <typename T>
auto transpose(Matrix<T>& a, int num_to_delete = 0, bool isRow = true) {
  int col = a.get_n_col();
  int row = a.get_n_row();
  auto mat = a.get_matrix();

  if (col == row) {
    int init = 0;
    for (size_t i = 0; i != row; i++) {
      for (size_t j = init; j != col; j++) {
        T a = mat[i][j];
        mat[i][j] = mat[j][i];
        mat[j][i] = a;
      }
      init++;
    }

    // Clean the matrix
    if (num_to_delete != 0) {
      if (isRow) {
        for (size_t i = 0; i != num_to_delete; i++) mat.pop_back();
      } else {
        for (size_t i = 0; i != row; i++) {
          for (size_t j = 0; j != num_to_delete; j++) mat[i].pop_back();
        }
      }
    }

    return Matrix<T>{mat};
  } else if (col > row) {
    for (size_t i = 0; i != (col - row); i++) {
      std::vector<int> row(5);
      mat.push_back(row);
    }
    Matrix<T> c{mat};

    return transpose(c, col - row, false);
  } else {
    for (size_t i = 0; i != row; i++) {
      for (size_t j = 0; j != (row - col); j++) {
        mat[i].push_back(0);
      }
    }
    Matrix<T> c{mat};
    return transpose(c, row - col, true);
  }
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
    CHECK(transpose(a) ==
          Matrix<int>{
              {{1, 4, 7, 1}, {2, 5, 8, -3}, {3, 6, 9, 5}, {5, 8, 10, 6}}});
    Matrix<int> b{{{1, 2}, {3, 4}, {4, 5}}};

    CHECK(transpose(b) == Matrix<int>{{{1, 3, 4}, {2, 4, 5}}});
  }

  SUBCASE("Calculating determinant") {
    Matrix<int> a{{{3, 2}, {1, 4}}};
    CHECK(determinant(a) == 10);
  }
}