#include <iostream>
#include <vector>
#include <math.h>

// Calculate mean
double mean(std::vector<double> &vect)
{
    double sum;
    int i = 0;
    for (auto &num : vect)
    {
        sum += num;
        ++i;
    }

    return (sum / i);
}

std::vector<double> get_max_min(std::vector<double> &vect)
{
    double max;
    double min;
    int i = 0;
    for (auto &num : vect)
    {
        if (i == 0)
        {
            max = num;
            min = num;
        }
        else
        {
            if (num > max)
            {
                max = num;
            }

            if (num < min)
            {
                min = num;
            }
        }
        ++i;
    }

    return {max, min};
}

double interval_center(std::vector<double> &vect)
{
    std::vector<double> result = get_max_min(vect);

    return ((result[0] + result[1]) / 2);
}

double partial_dispertion(std::vector<double> &vect)
{
    std::vector<double> result = get_max_min(vect);

    return ((result[0] - result[1]) / 2);
}

double standard_deviation(std::vector<double> &vect)
{
    double calculated_mean = mean(vect);

    double sum_of_difference;
    int i;
    for (auto &num : vect)
    {
        sum_of_difference += (num - calculated_mean) * (num - calculated_mean);
        ++i;
    }

    return sqrt(sum_of_difference / (vect.size() - 1));
}

double variance(std::vector<double> &vect)
{
    double calculated_standard_deviations = standard_deviation(vect);
    return (calculated_standard_deviations * calculated_standard_deviations);
}

int main()
{

    std::cout << "Select one of these functionalities: \n (1) - Calculate the mean \n (2) - Calculate the center of interval \n (3) - Parital dispertion \n (4) - Standard deviation \n (5) - Variance" << std::endl;

    int user_choice;
    std::cin >> user_choice;

    // Let the user insert its data
    std::vector<double> data;
    std::cout << "Insert your data (Enter a letter to stop inserting): \n";

    double user_data;
    while (std::cin >> user_data)
    {
        data.push_back(user_data);
    }

    double result;
    std::string operation = "";
    switch (user_choice)
    {
    case 1:
        result = mean(data);
        operation = "mean";
        break;
    case 2:
        result = interval_center(data);
        operation = "center of the interval";
        break;
    case 3:
        result = partial_dispertion(data);
        operation = "partial dispertion";
        break;
    case 4:
        result = standard_deviation(data);
        operation = "standard deviation";
        break;
    case 5:
        result = variance(data);
        operation = "variance";
        break;
    default:
        return EXIT_FAILURE;
        break;
    }

    std::cout << "The " << operation << " of your data set is:  " << result << std::endl;

    return 0;
}