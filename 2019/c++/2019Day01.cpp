/// \file 2019Day01.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/1.

#include <iostream>
#include <vector>
#include <numeric>

#include "utilities.hpp"

std::vector<int> getInput () {
    std::vector<int> masses;
    int mass;
    while (std::cin >> mass) {
        masses.push_back (mass);
    }
    return masses;
}

std::vector<int> calculateFuelCosts (std::vector<int> const& masses) {
    std::vector<int> costs;
    for (int mass : masses) {
        costs.push_back (mass / 3 - 2);
    }
    return costs;
}

int getRecursiveFuelCost (int mass) {
    int cost = mass / 3 - 2;
    if (cost <= 0) {
        return 0;
    }
    else {
        return cost + getRecursiveFuelCost (cost);
    }
}

std::vector<int> calculateFuelCostsPlusFuel (std::vector<int> const& masses) {
    std::vector<int> costs;
    for (int mass : masses) {
        costs.push_back (getRecursiveFuelCost (mass));
    }
    return costs;
}

int main () {
    std::vector<int> masses = getInput ();

    std::vector<int> costs = calculateFuelCosts (masses);
    std::cout << std::accumulate (costs.begin (), costs.end (), 0) << "\n";

    std::vector<int> betterCosts = calculateFuelCostsPlusFuel (masses);
    std::cout << std::accumulate (betterCosts.begin (), betterCosts.end (), 0) << "\n";
    return 0;
}
