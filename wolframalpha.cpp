#include <iostream>
#include <cstdlib>
#include <cmath>

class Complex
{
private:
    double r;
    double i;

public:
    Complex(double a, double b) : r{a}, i{b} {};
    double real() { return r; };
    double imag() { return i; };
};

class Matrix2
{
private:
    double a11;
    double a12;
    double a21;
    double a22;

public:
    Matrix2(double oo, double ot, double to, double tt) : a11{oo}, a12{ot}, a21{to}, a22{tt} {};
    double ga11() { return a11; };
    double ga12() { return a12; };
    double ga21() { return a21; };
    double ga22() { return a22; };
};

// Defining Solution Class that combines two complex
class Solution
{
private:
    double a;
    double ai;
    double b;
    double bi;

public:
    Solution(Complex x1, Complex x2) : a{x1.real()}, ai{x1.imag()}, b{x2.real()}, bi{x2.imag()}
    {
    }
    Complex first() { return Complex{a, ai}; };
    Complex second() { return Complex{b, bi}; };
};

// Calculates the solution of a second grade equation and return both solutions in a Solution type
Solution calculate_solution(double a, double b, double c)
{
    bool hasComplexSol = false;
    double delta = (b * b) + (-4 * a * c);
    if (delta < 0)
    {
        delta *= -1;
        hasComplexSol = true;
    }

    if (hasComplexSol)
    {
        Complex x1 = {(b * -1 / (2 * a)), sqrt(delta) / (2 * a)};
        Complex x2 = {(b * -1 / (2 * a)), (sqrt(delta) / (2 * a)) * -1};
        Solution sol = {x1, x2};
        return sol;
    }
    else
    {
        Complex x1 = {((b * -1) + sqrt(delta)) / (2 * a), 0};
        Complex x2 = {((b * -1) - sqrt(delta)) / (2 * a), 0};
        Solution sol = {x1, x2};
        return sol;
    }
}

double determinant(Matrix2 &mat)
{
    return mat.ga11() * mat.ga22() - mat.ga12() * mat.ga21();
}

Solution autovalues(Matrix2 &mat)
{
    double a = 1;
    double b = mat.ga11() * -1. + mat.ga22() * -1.;
    double c = mat.ga11() * mat.ga22() - mat.ga12() * mat.ga21();

    Solution sol = calculate_solution(a, b, c);
    return sol;
}

int main()
{
    std::cout << '\n';
    std::cout << "---------------------------------------------- \n";
    std::cout << "           INSERISCI UNA MATRICE 2x2           \n";
    std::cout << "---------------------------------------------- \n";
    std::cout << '\n';

    double a11, a12, a21, a22;
    std::cin >> a11 >> a12 >> a21 >> a22;

    if (std::cin.fail())
    {
        std::cerr << "Input invalido\n";
        return EXIT_FAILURE;
    }

    Matrix2 mat = {a11, a12, a21, a22};

    double det = determinant(mat);
    std::cout << "\nDeterminant is: " << det << "\n\n";

    if (det == 0)
    {
        std::cout << "The matrix is invertible\n";
    }

    Solution autoval = autovalues(mat);

    std::cout << "First autovalue\n"
              << "Real: " << autoval.first().real() << " Imag: " << autoval.first().imag() << "\n\n";
    std::cout << "Second autovalue\n"
              << "Real: " << autoval.second().real() << " Imag: " << autoval.second().imag() << "\n\n";

    return 0;
}