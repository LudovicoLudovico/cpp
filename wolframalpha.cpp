#include <cmath>
#include <cstdlib>
#include <iostream>

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
    Matrix2(double oo, double ot, double to, double tt)
        : a11{oo}, a12{ot}, a21{to}, a22{tt} {};
    double ga11() { return a11; };
    double ga12() { return a12; };
    double ga21() { return a21; };
    double ga22() { return a22; };
};

Matrix2 operator+(Matrix2 a, Matrix2 b)
{
    return Matrix2{a.ga11() + b.ga11(), a.ga12() + b.ga12(), a.ga21() + b.ga21(), a.ga22() + b.ga22()};
}
Matrix2 operator-(Matrix2 a, Matrix2 b)
{
    return Matrix2{a.ga11() - b.ga11(), a.ga12() - b.ga12(), a.ga21() - b.ga21(), a.ga22() - b.ga22()};
}
Matrix2 operator*(Matrix2 a, Matrix2 b)
{
    return Matrix2{a.ga11() * b.ga11() + a.ga12() * b.ga21(), a.ga11() * b.ga12() + a.ga12() * b.ga22(), a.ga21() * b.ga11() + a.ga22() * b.ga21(), a.ga21() * b.ga12() + a.ga22() * b.ga22()};
}

// Defining Solution Class that combines two complex
class Solution
{
private:
    double a;
    double ai;
    double b;
    double bi;

public:
    Solution(Complex x1, Complex x2)
        : a{x1.real()}, ai{x1.imag()}, b{x2.real()}, bi{x2.imag()} {}
    Complex first() { return Complex{a, ai}; };
    Complex second() { return Complex{b, bi}; };
};

// Calculates the solution of a second grade equation and return both solutions
// in a Solution type
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

double trace(Matrix2 &mat) { return mat.ga11() + mat.ga22(); }

int main()
{
    std::cout << '\n';
    std::cout << "---------------------------------------------- \n";
    std::cout << "                  MATRICI 2x2                  \n";
    std::cout << "---------------------------------------------- \n";
    std::cout << '\n';

    std::cout << "Select an operation: \n (1) - Sum \n (2) - Substract \n (3) - "
                 "Multiply \n (4) - Get all properties of a matrix \n";

    int selection;
    std::cin >> selection;

    if (std::cin.fail() || selection < 1 || selection > 4)
    {
        std::cerr << "Invalid input\n";
        return EXIT_FAILURE;
    }
    if (selection < 4)
    {
        std::cout << "Insert the first matrix (a11, a12, a21, a22): \n";
        double a11, a12, a21, a22;
        std::cin >> a11 >> a12 >> a21 >> a22;

        std::cout << "Insert the second matrix (b11, b12, b21, b22): \n";
        double b11, b12, b21, b22;
        std::cin >> b11 >> b12 >> b21 >> b22;

        Matrix2 first = {a11,
                         a12,
                         a21,
                         a22};
        Matrix2 second = {b11,
                          b12,
                          b21,
                          b22};
        Matrix2 result = {0, 0, 0, 0};

        switch (selection)
        {
        case 1:
            result = first + second;
            break;
        case 2:
            result = first - second;
            break;
        case 3:
            result = first * second;
        }

        std::cout << result.ga11() << " | " << result.ga12() << std::endl
                  << result.ga21() << " | " << result.ga22() << std::endl;
        return EXIT_SUCCESS;
    }

    if (selection == 4)
    {
        std::cout << "Insert the matrix (a11, a12, a21, a22): \n";
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
                  << "Real: " << autoval.first().real()
                  << " Imag: " << autoval.first().imag() << "\n\n";
        std::cout << "Second autovalue\n"
                  << "Real: " << autoval.second().real()
                  << " Imag: " << autoval.second().imag() << "\n\n";

        std::cout << "The trace is: " << trace(mat) << std::endl;
        return EXIT_SUCCESS;
    }

    return 0;
}