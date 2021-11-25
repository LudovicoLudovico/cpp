#include <iostream>
#include <vector>
#include <math.h>

// Calculate mean
double mean(std::vector<double> &data)
{
    double sum;

    for (int i = 0; i < data.size(); i++)
    {
        sum += data[i];
    }
    return (sum / data.size());
}

double interval_center(std::vector<double> &vect)
{
    double max;
    double min;

    for (int i = 0; i < vect.size(); i++)
    {
        if (i == 0)
        {
            max = vect[i];
            min = vect[i];
        }
        else
        {
            if (vect[i] > max)
            {
                max = vect[i];
            }

            if (vect[i] < min)
            {
                min = vect[i];
            }
        }
    }

    return ((max + min) / 2);
}

double partial_dispertion(std::vector<double> &vect)
{
    double max;
    double min;

    for (int i = 0; i < vect.size(); i++)
    {
        if (i == 0)
        {
            max = vect[i];
            min = vect[i];
        }
        else
        {
            if (vect[i] > max)
            {
                max = vect[i];
            }

            if (vect[i] < min)
            {
                min = vect[i];
            }
        }
    }

    return ((max - min) / 2);
}

double standard_deviation(std::vector<double> &vect)
{
    double calculated_mean = mean(vect);

    double sum_of_difference;
    for (int i = 0; i < vect.size(); i++)
    {
        sum_of_difference += (vect[i] - calculated_mean) * (vect[i] - calculated_mean);
    }

    double deviation = sqrt(sum_of_difference / (vect.size() - 1));

    return deviation;
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

    if (user_choice == 1)
    {
        double calculated_mean = mean(data);
        std::cout << "The mean of your data set is: " << calculated_mean << std::endl;
    }
    else if (user_choice == 2)
    {
        double calculated_interval = interval_center(data);
        std::cout << "The center of the interval is: " << calculated_interval << std::endl;
    }
    else if (user_choice == 3)
    {
        double calculated_partial = partial_dispertion(data);
        std::cout << "The partial dispertion is: " << calculated_partial << std::endl;
    }
    else if (user_choice == 4)
    {
        double deviation = standard_deviation(data);
        std::cout << "The standard deviation is: " << deviation << std::endl;
    }
    else if (user_choice == 5)
    {
        double calculated_varicance = variance(data);
        std::cout << "The variance of your data set is:  " << calculated_varicance << std::endl;
    }
    return 0;
}