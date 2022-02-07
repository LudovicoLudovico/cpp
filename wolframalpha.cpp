#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

struct Matrix
{
    std::vector<int> mat;
    int col_per_row;
    int row_num;
};

bool are_dipendent(const std::vector<int> &vec1, const std::vector<int> &vec2){
    
}
int determinant(const std::vector<int> &mat, int size)
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
Matrix create_matrix(std::string const &input)
{
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

    return Matrix{mat, col_per_row, row_num};
}

std::vector<int> operator+(std::vector<int> const mat1, std::vector<int> const mat2)
{
    std::vector<int> result;
    for (int i = 0; i < mat1.size(); i++)
    {
        result.push_back(mat1[i] + mat2[i]);
    }
    return result;
}

void print_matrix(const std::vector<int> &mat, int col_per_row)
{
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
}

void calculate_trace(const std::vector<int> &mat, int row_num, int col_per_row)
{
    int trace = mat[0];
    for (int i = 1; i < row_num; i++)
    {
        trace += mat[i * (col_per_row + 1)];
    }
    std::cout << "\n--- Trace ---\n"
              << trace << '\n';
}

int main()
{
    std::cout << "\n---------- INSERT MATRIX ----------\n";

    //= "{1,2,3,5}{4,5,6,8}{7,8,9,10}{1,-3,5,6} + {1,2,3,5}{4,5,6,8}{7,8,9,10}{1,-3,5,6}"
    std::string input;
    std::getline(std::cin, input);
    input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());

    std::string result = input;
    if (input.find('+') != std::string::npos)
    {
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] == '+')
            {
                std::string second = input;
                second.erase(second.begin() + i, second.end());
                input.erase(input.begin(), input.begin() + i + 1);
                Matrix mat1 = create_matrix(input);
                Matrix mat2 = create_matrix(second);

                std::vector<int> resultVec = mat1.mat + mat2.mat;
                std::string str(resultVec.begin(), resultVec.end());
                std::cout << str;
                result = str;
            }
        }
    }
    std::cout << result;

    Matrix cmat = create_matrix(result);
    int row_num = cmat.row_num;
    int col_per_row = cmat.col_per_row;

    std::cout << "\n--- Matrix Dimensions ---\n";
    std::cout << row_num << 'x' << col_per_row << '\n';

    if (row_num != col_per_row)
    {
        std::cout << "Can't calculate determinant\n";
        std::cout << "Can't calculate trace\n";
        return EXIT_FAILURE;
    }

    print_matrix(cmat.mat, col_per_row);

    std::cout << "\n--- Determinant ---\n";
    int det = determinant(cmat.mat, col_per_row);
    std::cout << det << '\n';

    if (det != 0)
    {
        std::cout << "The matrix is invertible\n";
    }
}
