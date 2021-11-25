#include <cmath>
#include <iostream>
#include <cstdlib>

// Defining Class for Complex
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

/*
    Describing operators for complex numbers
*/
Complex operator+(Complex &a, Complex &b)
{
    double real = a.real() + b.real();
    double imag = a.imag() + b.imag();

    Complex c = {real, imag};
    return c;
}
Complex operator-(Complex &a, Complex &b)
{
    double real = a.real() - b.real();
    double imag = a.imag() - b.imag();

    Complex c = {real, imag};
    return c;
}
Complex operator*(Complex &a, Complex &b)
{
    double real = (a.real() * b.real()) + (a.imag() * b.imag()) * -1;
    double imag = (a.real() * b.imag()) + (a.imag() * b.real());
    Complex c = {real, imag};
    return c;
}

bool operator==(Complex &a, Complex &b)
{
    return (a.real() == b.real()) && (b.imag() == b.imag());
}

// Calculating the norm squared of a vector
double norm(Complex &c)
{
    return (c.real() * c.real() + c.imag() * c.imag());
}

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

// Calculates the conjugate of a number a+ib -> a-ib
Complex conjugates(Complex &a)
{
    Complex conjugate = {a.real(), a.imag() * -1};
    return conjugate;
}

int main()
{
    // Operetion selection
    std::cout << "__________   SELECT AN OPERATION   _________\n";
    std::cout << " (1) - Somma\n (2) - Differenza \n (3) - Prodotto \n (4) - Norma \n (5) - Coniugato \n (6) - Equazione di secondo grado \n";

    int selection;
    std::cin >> selection;

    if (std::cin.fail() || selection > 6)
    {
        std::cerr << "Invalid number\n";
        return EXIT_FAILURE;
    }

    if (selection < 4)
    {
        std::cout << "Inserisci i due numeri: \n";
        double a, ai, b, bi;
        std::cin >> a >> ai >> b >> bi;

        if (std::cin.fail())
        {
            std::cerr << "Invalid number\n";
            return EXIT_FAILURE;
        }

        Complex a_compl = {a, ai};
        Complex b_compl = {b, bi};
        Complex result = {0, 0};

        switch (selection)
        {
        case 1:
            result = a_compl + b_compl;
            break;
        case 2:
            result = a_compl - b_compl;
            break;
        case 3:
            result = a_compl * b_compl;
            break;
        default:
            return EXIT_FAILURE;
            break;
        }

        std::cout << "Reale: " << result.real() << " Imag: " << result.imag() << std::endl;
        return 0;
    }

    if (selection == 4 || selection == 5)
    {
        std::cout << "Inserisci il numero: \n";
        double a, ai;
        std::cin >> a >> ai;

        if (std::cin.fail())
        {
            std::cerr << "Invalid number\n";
            return EXIT_FAILURE;
        }

        Complex a_compl = {a, ai};
        Complex result = {0, 0};

        switch (selection)
        {
        case 4:
            result = {norm(a_compl), 0};
            break;
        case 5:
            result = conjugates(a_compl);
            break;
        default:
            return EXIT_FAILURE;
            break;
        }

        std::cout << "Reale: " << result.real() << " Imag: " << result.imag() << std::endl;
        return 0;
    }

    if (selection == 6)
    {
        std::cout << "Inserisci i parametri a, b e c dell'equazione: \n";
        double a, b, c;
        std::cin >> a >> b >> c;

        if (std::cin.fail())
        {
            std::cerr << "Invalid number\n";
            return EXIT_FAILURE;
        }

        Solution sols = calculate_solution(a, b, c);

        std::cout << "Real: " << sols.first().real() << " Imag: " << sols.first().imag() << "i\n";
        std::cout << "Real: " << sols.second().real() << " Imag: " << sols.second().imag() << "i\n";
    }

    return 0;
}