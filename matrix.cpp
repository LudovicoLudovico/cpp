#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "doctest.h"

template <typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_{};
  int n_row_{};
  int n_col_{};
  int original_col_{};
  int original_row_{};
  int n_of_reorders_{};

 public:
  Matrix(std::vector<std::vector<T>> const& mat)
      : matrix_{mat},
        n_row_{static_cast<int>(mat.size())},
        n_col_{static_cast<int>(mat[0].size())},
        original_col_{static_cast<int>(mat[0].size())},
        original_row_{static_cast<int>(mat.size())} {}
  Matrix(std::string input) {
    int rows{};
    int cols{};
    std::vector<T> numbers{};
    for (size_t i = 0; i < input.length() - 1; i++) {
      if (input[i] == ',' || input[i] == '|') {
        ++cols;
        if (input[i] == '|') {
          ++rows;
        }
      } else {
        std::string tmp = "";
        while (input[i] != ',' && input[i] != '|') {
          tmp += input[i];
          ++i;
        }
        --i;
        numbers.push_back(std::stod(tmp));
      }
    }

    cols /= rows;
    original_row_ = rows;
    n_row_ = rows;
    original_col_ = cols;
    n_col_ = cols;

    int num_counter = 0;
    for (int i = 0; i != rows; i++) {
      auto row = std::vector<T>(cols);
      matrix_.push_back(row);
      for (int j = 0; j != cols; j++) {
        matrix_[i][j] = numbers[num_counter];
        ++num_counter;
      }
    }
  }

  int getRow() const { return n_row_; }
  int getCol() const { return n_col_; }
  auto getMatrix() const { return matrix_; }

  void print() const {
    std::cout << '\n';
    for (int i = 0; i != n_row_; i++) {
      for (int j = 0; j != n_col_; j++) {
        std::cout << "|    " << matrix_[i][j] << "    |";
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }

  T trace() const {
    if (n_col_ != n_row_)
      throw std::runtime_error{"Can't calculate trace on non-square matrix"};

    T trace = 0;
    for (int i = 0; i < n_row_; i++) trace += matrix_[i][i];

    return trace;
  }
  bool isSymmetric() const {
    if (n_col_ != n_row_) return false;

    for (int i = 0; i != n_row_; i++) {
      for (int j = 0; j != n_col_; j++) {
        if (matrix_[i][j] != matrix_[j][i]) return false;
      }
    }
    return true;
  }
  void makeSquare() {
    if (n_col_ > n_row_) {
      for (int i = 0; i != (n_col_ - n_row_); i++) {
        std::vector<int> row(n_col_);
        matrix_.push_back(row);
      }
      n_row_ = n_col_;
    } else {
      for (int i = 0; i != n_row_; i++) {
        for (int j = 0; j != (n_row_ - n_col_); j++) {
          matrix_[i].push_back(0);
        }
      }
      n_col_ = n_row_;
    }
  }
  void reorder() {
    for (int i = 0; i != n_row_ - 1; i++) {
      int zeros = std::count(matrix_[i].begin(), matrix_[i].begin() + i + 1, 0);
      if (zeros > i) {
        auto row = matrix_[i];
        matrix_[i] = matrix_[i + 1];
        matrix_[i + 1] = row;
        ++n_of_reorders_;
      }
    }
  }
  void removeNullRows() {
    for (int i = n_row_ - 1; i != -1; i--) {
      int zeros = 0;
      for (int j = 0; j != n_col_; j++) {
        if (matrix_[i][j] != 0)
          break;
        else
          ++zeros;
      }
      if (zeros == n_col_) {
        matrix_.erase(matrix_.begin() + i);
        --n_row_;
      }
    }
  }
  void removeNRowsFromBottom(int num_to_delete) {
    for (int i = 0; i != num_to_delete; i++) {
      matrix_.pop_back();
    }
    n_row_ -= num_to_delete;
  }
  void removeNColsFromRight(int num_to_delete) {
    for (int i = 0; i != n_row_; i++) {
      for (int j = 0; j != num_to_delete; j++) {
        matrix_[i].pop_back();
      }
    }
    n_col_ -= num_to_delete;
  }
  void removeNColsFromLeft(int num_to_delete) {
    for (int i = 0; i != n_row_; i++) {
      for (int j = 0; j != num_to_delete; j++) {
        matrix_[i].erase(matrix_[i].begin());
      }
    }
    n_col_ -= num_to_delete;
  }
  auto transpose() {
    bool hasRowsToBeDeleted = n_col_ < n_row_;

    this->makeSquare();

    for (int i = 0; i != n_row_; i++) {
      for (int j = i; j != n_col_; j++) {
        T a = matrix_[i][j];
        matrix_[i][j] = matrix_[j][i];
        matrix_[j][i] = a;
      }
    }

    if (hasRowsToBeDeleted) this->removeNRowsFromBottom(n_col_ - original_col_);
    if (!hasRowsToBeDeleted) this->removeNColsFromRight(n_row_ - original_row_);

    return *this;
  }
  double determinant() {
    if (n_col_ != n_row_)
      throw std::runtime_error{
          "Can't calculate determinant on non-square matrix"};

    auto c = *this;
    auto b = this->gauss();

    if (b.getRow() != b.getCol()) return 0;

    auto mat = b.getMatrix();

    double det = 1;
    for (int i = 0; i != b.getRow(); i++) {
      for (int j = 0; j != b.getCol(); j++) {
        if (mat[i][j] != 0) {
          det *= mat[i][j];
          break;
        }
      }
    }

    if (n_of_reorders_ % 2 != 0) det = -det;

    *this = c;
    return det;
  }
  auto gauss() {
    int line = 0;
    int offset = 0;

    for (int k = 0; k != (n_row_ - 1); k++) {
      for (int i = line; i != n_row_ - 1; i++) {
        if (matrix_[i + 1][offset] == 0 && matrix_[i][offset] == 0) continue;

        if (matrix_[i + 1][offset] != 0 && matrix_[line][offset] != 0) {
          double c = -1.0 * (double(matrix_[i + 1][offset]) /
                             double(matrix_[line][offset]));
          for (int j = offset; j != n_col_; j++) {
            matrix_[i + 1][j] += c * matrix_[line][j];
          }
        }
      }
      ++line;
      ++offset;
    }

    this->removeNullRows();
    this->reorder();
    return *this;
  }
  auto makePivotEqualOne() {
    int line = n_row_ - 1;
    for (int i = line; i != -1; i--) {
      if (matrix_[line][line] != 1) {
        double c = (matrix_[line][line] > 0) ? 1. : -1.;
        for (int j = 0; j != n_col_; j++) {
          matrix_[line][j] *= c;
          matrix_[line][j] /= matrix_[line][line];
        }
      }
    }
  }
  auto inverseGauss() {
    for (int line = n_row_ - 1; line != 0; line--) {
      for (int i = 0; i != line; i++) {
        double c = -matrix_[i][line] / matrix_[line][line];

        for (int j = 0; j != n_col_; j++) {
          matrix_[i][j] += c * matrix_[line][j];
        }
      }
    }
  }
  auto invert() {
    if (n_col_ != n_row_)
      throw std::runtime_error{"Can't invert non square matrix"};

    // Adding identity matrix on the right
    for (int i = 0; i != n_row_; i++) {
      for (int j = 0; j != n_col_; j++) {
        if (i == j)
          matrix_[i].push_back(1);
        else
          matrix_[i].push_back(0);
      }
    }
    n_col_ *= 2;

    this->gauss();
    this->makePivotEqualOne();
    this->inverseGauss();
    this->removeNColsFromLeft(n_col_ / 2);

    return *this;
  }
};

template <typename T, typename R>
bool operator==(Matrix<T> const& a, Matrix<R> const& b) {
  if (a.getCol() != b.getCol() || a.getRow() != b.getRow()) return false;

  auto matA = a.getMatrix();
  auto matB = b.getMatrix();

  for (int i = 0; i != a.getRow(); i++) {
    for (int j = 0; j != a.getCol(); j++) {
      if (matA[i][j] != matB[i][j]) return false;
    }
  }
  return true;
}
template <typename T, typename R>
bool operator!=(Matrix<T> const& a, Matrix<R> const& b) {
  return !(a == b);
}

template <typename T, typename R>
auto operator+(Matrix<T> const& a, Matrix<R> const& b) {
  if (a.getCol() != b.getCol() || a.getRow() != b.getRow())
    throw std::runtime_error{"Can't perform operation on the given matrices"};

  auto matA = a.getMatrix();
  auto matB = b.getMatrix();

  for (int i = 0; i != a.getRow(); i++) {
    for (int j = 0; j != a.getCol(); j++) {
      matA[i][j] += matB[i][j];
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator-(Matrix<T>& a, Matrix<R> const& b) {
  if (a.getCol() != b.getCol() || a.getRow() != b.getRow())
    throw std::runtime_error{"Can't perform operation on the given matrices"};

  auto matA = a.getMatrix();
  auto matB = b.getMatrix();

  for (int i = 0; i != a.getRow(); i++) {
    for (int j = 0; j != a.getCol(); j++) {
      matA[i][j] -= matB[i][j];
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator*(Matrix<T>& a, R scalar) {
  auto matA = a.getMatrix();

  for (int i = 0; i != a.getRow(); i++) {
    for (int j = 0; j != a.getCol(); j++) {
      matA[i][j] *= scalar;
    }
  }
  return Matrix{matA};
}

template <typename T, typename R>
auto operator*(Matrix<T>& a, Matrix<R>& b) {
  if (a.getCol() != b.getRow())
    throw std::runtime_error{"Can't multipy the given matrices"};

  std::vector<std::vector<T>> result{};

  auto matA = a.getMatrix();
  auto matB = b.getMatrix();

  for (int i = 0; i != a.getRow(); i++) {
    std::vector<T> row(b.getCol());
    result.push_back(row);

    for (int j = 0; j != b.getCol(); j++) {
      for (int k = 0; k != a.getCol(); k++) {
        result[i][j] += matA[i][k] * matB[k][j];
      }
    }
  }

  return Matrix{result};
}

template <typename T, typename R>
auto operator/(Matrix<T>& a, R scalar) {
  auto matA = a.getMatrix();

  for (auto& row : matA) {
    for (int j = 0; j != a.getCol(); j++) {
      row[j] /= scalar;
    }
  }

  return Matrix{matA};
}

TEST_CASE("Testing Matrix") {
  // Matrix<int> a{{{1, 2, 3, 5}, {4, 5, 6, 8}, {7, 8, 9, 10}, {1, -3, 5, 6}}};
  Matrix<int> a{"|1,2,3,5|4,5,6,8|7,8,9,10|1,-3,5,6|"};

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
  SUBCASE("Testing * between matrices") {
    Matrix<int> a = {{{1, 2}, {2, 3}}};
    Matrix<int> b = {{{1, 2}, {2, 3}}};
    CHECK(a * b == Matrix<int>{{{5, 8}, {8, 13}}});

    Matrix<int> c{{{1, 2, 3}, {4, 5, 6}}};
    Matrix<int> d{{{1, 1, 2}, {3, 4, 5}, {6, 7, 8}}};
    CHECK(c * d == Matrix<int>{{{25, 30, 36}, {55, 66, 81}}});
  }
  SUBCASE("Testing * scalar operator") { CHECK((a * 2) == (a + a)); }
  SUBCASE("Testing / scalar operator") { CHECK((a / 1) == a); }
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
  SUBCASE("Calculating determinant") {
    Matrix<int> simple{{{1, 2}, {3, 4}}};
    CHECK(simple.determinant() == -2);
    CHECK(a.determinant() == -36);
  }

  SUBCASE("Inverting") {
    CHECK(Matrix<int>{{{1, 2}, {3, 5}}}.invert() ==
          Matrix<int>{{{-5, 2}, {3, -1}}});

    CHECK(Matrix<int>{{{1, 0, 1}, {0, 1, 1}, {1, 1, 1}}}.invert() ==
          Matrix<int>{{{0, -1, 1}, {-1, 0, 1}, {1, 1, -1}}});

    CHECK(Matrix<int>{{{1, 2, -3}, {0, 1, 2}, {0, 0, 1}}}.invert() ==
          Matrix<int>{{{1, -2, 7}, {0, 1, -2}, {0, 0, 1}}});
  }
}