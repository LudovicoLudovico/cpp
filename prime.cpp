#include <iostream>
#include <vector>
#include <cstdlib>

std::vector<int> is_prime(int num)
{
    if (num < 0)
        num *= -1;

    std::vector<int> result = {0, 1};
    if (num < 4)
    {
        return result;
    }
    else
    {
        int counter = 2;
        while (counter <= num / 2 + 1)
        {
            if (num % counter == 0)
            {
                return {1, counter};
            }
            ++counter;
        }
    }

    return result;
}

int get_closest_prime(int num)
{
    int range = 1;
    while (true)
    {
        if (is_prime(num + range)[0])
            return num + range;
        if (is_prime(num - range)[0])
            return num - range;
    }
    return 0;
}

int main()
{
    std::cout << "\n--------- PRIME NUMBER ----------\n";
    int num;
    std::cin >> num;
    if (std::cin.fail())
    {
        std::cout << "Input non valido";
        return EXIT_FAILURE;
    }

    std::vector<int> result = is_prime(num);
    if (result[0] == 0)
    {
        std::cout << "It's prime\n";
    }
    else
    {
        std::cout << "It's not prime\n";
        std::cout << "It's divisible by: " << result[1] << '\n';
        std::cout << "Its closest prime is: ";
        std::cout << get_closest_prime(num) << '\n';
    }
}