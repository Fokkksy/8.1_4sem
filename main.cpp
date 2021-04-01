#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <execution>
#include "Timer.hpp"

inline bool inside(const double& x, const double& y)
{
    return (x * x + y * y < 1);
}


double consistent_pi(const std::size_t quantity)
{
    std::mt19937_64 rand;
    std::uniform_real_distribution <> distr(0.0, 1.0);
    std::size_t points_inside = 0;

    for (auto i = 0U; i < quantity; i++)
    {
        if (inside(distr(rand), distr(rand)))
        {
            points_inside++;
        }
    }
    return points_inside * 4.0 / quantity;
}

double parallel_pi(const std::size_t quantity)
{
    std::mt19937_64 rand;
    std::uniform_real_distribution <> distr(0.0, 1.0);
    std::vector <std::pair<double, double>> points(quantity);


    std::for_each(std::execution::par, std::begin(points), std::end(points), [&distr, &rand](auto& point)
        {
            point.first = distr(rand);
            point.second = distr(rand);
        });

    std::size_t points_inside = std::count_if(std::execution::par,
        std::begin(points), std::end(points), [](const auto& p)
        {
            return inside(p.first, p.second);
        });


    return points_inside * 4.0 / quantity;
}


int main(int argc, const char* argv[])
{
    double efficiency = 0;
    for (auto i = 100000; i <= 1000000; i += 100000)
    {
        std::cout << i / 100000 << " iteration:" << std::endl;
        Timer t_1("consistent pi: ");
        double pi_c = consistent_pi(i);
        t_1.print();
        std::cout << std::setprecision(8) << pi_c << std::endl;

        Timer t("parallel pi: ");
        double pi_p = parallel_pi(i);
        t.print();
        std::cout << std::setprecision(8) << pi_p << std::endl;

        std::cout << "The running time in parallel is " << std::setprecision(2) << t.math() / t_1.math() * 100
            << "% of the consistent" << std::endl << std::endl;

        efficiency += (t.math() / t_1.math());

    }

    std::cout << "On average the running time of the parallel algorithm takes "
        << std::setprecision(2) << efficiency * 10 << "% of the consistent" << std::endl;


    return 0;
}
