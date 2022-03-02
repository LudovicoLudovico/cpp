#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <stdexcept>

#include "cassert"
#include "doctest.h"

/*
   COMPLEX CLASS
*/
template <typename T>
class Complex {
 private:
  T r_{};
  T i_{};

 public:
  Complex(T a, T b) : r_{a}, i_{b} {};
  T real() const { return r_; };
  T imag() const { return i_; };
  void print() const {
    std::cout << r_ << (i_ >= 0 ? " + " : " - ") << i_ << 'i' << '\n';
  }
};

template <typename T, typename R>
bool operator==(Complex<T> const& a, Complex<R> const& b) {
  return (a.real() == b.real()) && (b.imag() == b.imag());
}

template <typename T, typename R>
auto operator+(Complex<T> const& a, Complex<R> const& b) {
  auto real = a.real() + b.real();
  auto imag = a.imag() + b.imag();
  return Complex{real, imag};
}

template <typename T, typename R>
auto operator-(Complex<T> const& a, Complex<R> const& b) {
  return Complex{a.real() - b.real(), a.imag() - b.imag()};
}

template <typename T, typename R>
auto operator*(Complex<T> const& a, Complex<R> const& b) {
  auto real = (a.real() * b.real()) + (a.imag() * b.imag()) * -1;
  auto imag = a.real() * b.imag() + a.imag() * b.real();
  return Complex{real, imag};
}

template <typename T>
double norm2(Complex<T> const& c) {
  return (c.real() * c.real() + c.imag() * c.imag());
}

template <typename T>
auto conjugate(Complex<T>& a) {
  return Complex{a.real(), (a.imag() * 1)};
}

/*
   SOLUTION CLASS
*/
class Solution {
 private:
  double a;
  double ai;
  double b;
  double bi;

 public:
  Solution(Complex<double> x1, Complex<double> x2)
      : a{x1.real()}, ai{x1.imag()}, b{x2.real()}, bi{x2.imag()} {}
  Complex<double> first() { return Complex<double>{a, ai}; };
  Complex<double> second() { return Complex<double>{b, bi}; };
};

Solution calculate_solution(double a, double b, double c) {
  if (a == 0 && b == 0 && c != 0) throw std::runtime_error{"No solutions"};
  bool hasComplexSol = false;
  auto delta = (b * b) + (-4 * a * c);
  if (delta < 0) {
    delta *= -1;
    hasComplexSol = true;
  }

  Complex<double> x1{.0, .0};
  Complex<double> x2{.0, .0};
  if (hasComplexSol) {
    x1 = {(b * -1 / (2 * a)), sqrt(delta) / (2 * a)};
    x2 = {(b * -1 / (2 * a)), (sqrt(delta) / (2 * a)) * -1};
  } else {
    x1 = {((b * -1) + sqrt(delta)) / (2 * a), 0.0};
    x2 = {((b * -1) - sqrt(delta)) / (2 * a), 0.0};
  }
  return {x1, x2};
}

double calculate_solution(double b, double c) { return -c / b; }

/*
   TESTING COMPLEX
*/
TEST_CASE("Testing Complex") {
  SUBCASE("Testing .real()") {
    Complex<int> r{3, 4};
    CHECK(r.real() == 3);

    Complex<double> a{3.2, 4.5};
    CHECK(a.real() == 3.2);
  }

  SUBCASE("Testing .imag()") {
    Complex<int> r{3, 4};
    CHECK(r.imag() == 4);

    Complex<double> a{3.2, 4.5};
    CHECK(a.imag() == 4.5);
  }

  SUBCASE("Printing") {
    Complex<double> a{3.2, 4.5};
    a.print();
  }

  SUBCASE("Testing + operator") {
    Complex<int> a{2, 2};
    Complex<int> b{3, 3};
    Complex<double> c{3.0, 3.0};

    CHECK(a + b == Complex{5, 5});
    CHECK(a + c == Complex{5.0, 5.0});
  }

  SUBCASE("Testing - operator") {
    Complex<int> a{2, 2};
    Complex<int> b{3, 3};
    Complex<double> c{3.0, 3.0};

    CHECK(a - b == Complex{-1, -1});
    CHECK(a - c == Complex{-1.0, -1.0});
  }
  SUBCASE("Testing * operator") {
    Complex<int> a{8, -5};
    Complex<int> b{3, -2};
    Complex<double> c{3.0, -2.0};

    CHECK(a * b == Complex{14, -31});
    CHECK(a * c == Complex{14.0, -31.0});
  }
  SUBCASE("Testing norm2") {
    Complex<int> a{3, 2};
    CHECK(norm2(a) == 13);
  }
  SUBCASE("Testing conjugate") {
    Complex<int> a{2, 3};
    Complex<double> b{2.0, 3.0};

    CHECK(conjugate(a) == Complex{2, -3});
    CHECK(conjugate(b) == Complex{2.0, -3.0});
  }
}

/*
   TESTING SOLUTION
*/
TEST_CASE("Testing Solution") {
  SUBCASE("Testing function") {
    int a = 3;
    int b = 10;
    int c = 3;

    Solution result = calculate_solution(a, b, c);
    result.first().print();
    CHECK(result.second() == Complex<double>{-3, 0});
    CHECK(doctest::Approx(result.first().real()) == -0.333333);
    CHECK(result.first().imag() == 0);
  }
  SUBCASE("Equation with no solution") {
    CHECK_THROWS(calculate_solution(0, 0, 2));
  }
  SUBCASE("First grade equation") {
    int b = 10;
    int c = 20;

    CHECK(calculate_solution(b, c) == -2);
  }
}