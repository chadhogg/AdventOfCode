/// \file 2021Day15.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/15.

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "utilities.hpp"

/// The risk level of entering a location.
using Risk = unsigned int;

/// A 2-D matrix of unknown size, any type.
template<typename T>
using Grid = std::vector<std::vector<T>>;

/// A sequence of coordinates passed through.
using Path = std::vector<Coordinate>;

/// A coordinate with information about itself.
struct CoordinatePlus {
    /// \brief Constructs one.
    /// \param[in] c The coordinate.
    CoordinatePlus (Coordinate c) 
        : coord {c}, cheapestPath {}, riskToGetHere {0}, estimatedRiskToGoal {0} {
    }
    /// The coordinate.
    Coordinate coord;
    /// The cheapest path we have found so far to the coordinate.
    Path cheapestPath;
    /// Total risk encountered along the cheapest path.
    Risk riskToGetHere;
    /// A low-ball estimate of how much further risk might be entailed in reaching the goal from here.
    Risk estimatedRiskToGoal;

    inline Risk getRisk () const { return riskToGetHere + estimatedRiskToGoal; }
};

/// The largest legal risk value.
constexpr Risk MAX_RISK = 9U;
/// The smallest legal risk value.
constexpr Risk MIN_RISK = 1U;


/// \brief Reads in a grid from standard input.
/// \return A grid of risk levels.
Grid<Risk> getInput () {
    Grid<Risk> problem;
    std::string line;
    while (std::cin >> line) {
        std::vector<Risk> row;
        for (char c : line) {
            row.push_back (c - '0');
        }
        problem.push_back (row);
    }
    return problem;
}

/// \brief Computes the heuristic cost of reaching the goal from a coordinate.
/// \param[in] current The coordinate you are at.
/// \param[in] goal The coordinate you'd like to be at.
/// \return The minimum possible risk you could incur going from here to there.
Risk heuristicCost (Coordinate const& current, Coordinate const& goal) {
    Risk manhattan = std::abs (current.row - goal.row) + std::abs (current.col - goal.col);
    return manhattan * MIN_RISK;
}

/// A comparator allowing us to create a priority queue ordered by total estimated risk.
class CoordinatePlusComparator {
public:
    /// \brief Compares two coordinate-plus-es.
    /// \param[in] x A coordinate-plus.
    /// \param[in] y Another coordinate-plus.
    /// \return True if x should have lower priority (higher total risk) than y.
    inline bool operator() (CoordinatePlus const& x, CoordinatePlus const& y) const {
        return x.getRisk () > y.getRisk ();
    }
};

/// \brief Uses A* search to find the lowest risk path from upper-left to lower-right.
/// \param[in] problem The grid of risks.
/// \return A coordinate-plus containing the path and its risk level.
CoordinatePlus findCheapestPath (Grid<Risk> const& problem) {
    Coordinate goal {(int)problem.size () - 1, (int)problem.at (0).size () - 1};
    CoordinatePlusComparator comp {};

    std::vector<CoordinatePlus> open;
    std::unordered_map<Coordinate, Risk> costs;
    
    CoordinatePlus start {{0, 0}};
    start.cheapestPath.push_back ({0, 0});
    start.estimatedRiskToGoal = heuristicCost (start.coord, goal);
    open.push_back (start);
    costs.insert (std::make_pair (start.coord, start.getRisk ()));
    
    while (!open.empty ()) {
        std::pop_heap (open.begin (), open.end (), comp);
        CoordinatePlus current {open.back ()};
        open.pop_back ();

        if (current.coord == goal) {
            return current;
        }

        if (current.getRisk () > costs.at (current.coord)) {
            continue; // already found a cheaper route here
        }

        std::vector<Coordinate> neighbors;
        if (current.coord.row > 0) {
            neighbors.push_back ({current.coord.row - 1, current.coord.col});
        }
        if (current.coord.row < (int)problem.size () - 1) {
            neighbors.push_back ({current.coord.row + 1, current.coord.col});
        }
        if (current.coord.col > 0) {
            neighbors.push_back ({current.coord.row, current.coord.col - 1});
        }
        if (current.coord.col < (int)problem.at (0).size () - 1) {
            neighbors.push_back ({current.coord.row, current.coord.col + 1});
        }
        for (Coordinate const& n : neighbors) {
            CoordinatePlus next {n};
            next.cheapestPath = current.cheapestPath;
            next.cheapestPath.push_back (n);
            next.riskToGetHere = current.riskToGetHere + problem.at (n.row).at (n.col);
            next.estimatedRiskToGoal = heuristicCost (next.coord, goal);
            if (costs.find (n) == costs.end () || costs.at (n) > next.getRisk ()) {
                open.push_back (next);
                std::push_heap (open.begin (), open.end (), comp);
                costs[n] = next.getRisk ();
            }
        }

    }
    
    throw std::runtime_error ("No path found.");
}

/// \brief Duplicates a problem to the left and right.
/// \param[in] orig The original problem.
/// \param[in] factor How much larger to make it.
/// \return An expanded version of the problem.
Grid<Risk> expandProblem (Grid<Risk> const& orig, unsigned int factor) {
    const unsigned int ROWS = orig.size ();
    const unsigned int COLS = orig.at (0).size ();
    Grid<Risk> revised;
    for (unsigned int row {0U}; row < ROWS * factor; ++row) {
        std::vector<Risk> line;
        for (unsigned int col {0U}; col < COLS * factor; ++col) {
            unsigned int additions = row / ROWS + col / COLS;
            Risk value = orig.at (row % ROWS).at (col % COLS);
            for(unsigned int slow {0U}; slow < additions; ++slow) {
                ++value;
                if (value > MAX_RISK) { value = MIN_RISK; }
            }
            line.push_back (value);
        }
        revised.push_back (line);
    }
    return revised;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Grid<Risk> problem = getInput ();
    CoordinatePlus answer1 = findCheapestPath (problem);
    std::cout << answer1.riskToGetHere << "\n";
    Grid<Risk> revised = expandProblem (problem, 5U);
    CoordinatePlus answer2 = findCheapestPath (revised);
    std::cout << answer2.riskToGetHere << "\n";
    return 0;
}