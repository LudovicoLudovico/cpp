#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

int determinant(std::vector<int> const &mat, int size)
{
    if (size == 2)
    {
        return mat[0] * mat[3] - mat[1] * mat[2];
    }
    int det = 0;
    for (int i = 0; i < size; i++)
    {
        std::vector<int> copy = mat;
        std::vector<int> result;
        copy.erase(copy.begin(), copy.begin() + (size));
        // mat[i] * (-1)^(i + 2) * determinant()

        // {4,5,6,7,8,9}
        //    -     -
        int counter = i;
        for (int j = 0; j < copy.size(); j++)
        {
            if (counter == j)
            {
                counter += size;
                continue;
            }

            result.push_back(copy[j]);
        }

        det += mat[i] * std::pow(-1, (i + 2)) * determinant(result, size - 1);
    }
    return det;
}

int main()
{
    std::cout << "\n---------- INSERT MATRIX ----------\n";

    //= "{1,2,3,5}{4,5,6,8}{7,8,9,10}{1,-3,5,6}"
    std::string input;
    input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());
    std::cout << input;
    std::getline(std::cin, input);

    std::vector<int> del;
    int row_num = 0;
    int col_num = 0;

    // Finding dels
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == '{' || input[i] == '}' || input[i] == ',')
        {
            if (input[i] == '}')
                row_num++;
            if (input[i] == ',' || input[i] == '}')
                col_num++;
            del.push_back(i);
        }
    }
    int col_per_row = col_num / row_num;
    std::vector<int> mat;

    std::vector<int> curr_row;
    for (int i = 0; i < del.size() - 1; i++)
    {
        if (del[i + 1] - del[i] == 1)
            continue;
        std::string sub = input.substr(del[i], del[i + 1] - del[i]);
        sub = sub.substr(1, sub.size() - 1);
        mat.push_back(std::stoi(sub));
    }

    //
    // Displaying Matrix Dimensions and error messages
    //
    std::cout << "\n--- Matrix Dimensions ---\n";
    std::cout << row_num << 'x' << col_per_row << '\n';

    if (row_num != col_per_row)
    {
        std::cout << "Can't calculate determinant\n";
        std::cout << "Can't calculate trace\n";
        return EXIT_FAILURE;
    }

    //
    // Printing Matrix
    //
    std::cout << "\n--- Matrix ---\n";
    int counter = 0;
    for (int i = 0; i < mat.size(); i++)
    {
        std::cout << "|    " << mat[i] << "     |";
        ++counter;
        if (counter == col_per_row)
        {
            std::cout << '\n';
            counter = 0;
        }
    }

    //
    // Calculating Trace
    //
    int trace = mat[0];
    for (int i = 1; i < row_num; i++)
    {
        trace += mat[i * (col_per_row + 1)];
    }
    std::cout << "\n--- Trace ---\n"
              << trace << '\n';

    //
    //  Calculating determinant
    //
    std::cout << "\n--- Determinant ---\n";
    std::cout << determinant(mat, col_per_row) << '\n';
}
