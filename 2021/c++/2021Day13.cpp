/// \file 2021Day13.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/13.

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cassert>

#include "utilities.hpp"

/// A fold of the paper.
struct Fold {
    /// True for folding up, false for folding left.
    bool up;
    /// The y-value (or x-value) on which the fold is to occur.
    int where;
};

/// A problem to solve.
struct Problem {
    /// A collection of dots on the transparent paper.
    std::unordered_set<Coordinate> dots;
    /// A list of folds to perform.
    std::vector<Fold> folds;
};

/// Inserts a picture of the paper into an output stream.
/// \param[in] out The stream.
/// \param[in] prob The problem.
/// \return The stream.
/// \post A picture of the paper has been printed to the stream.
std::ostream& operator<< (std::ostream& out, Problem const& prob) {
    Coordinate upperLeft(0, 0);
    Coordinate lowerRight(0, 0);
    for (Coordinate const& c : prob.dots) {
        if (c.row < upperLeft.row) {
            upperLeft.row = c.row;
        }
        if (c.row > lowerRight.row) {
            lowerRight.row = c.row;
        }
        if (c.col < upperLeft.col) {
            upperLeft.col = c.col;
        }
        if (c.col > lowerRight.col ) {
            lowerRight.col = c.col;
        }
    }
    for (int row = upperLeft.row; row <= lowerRight.row; ++row) {
        for (int col = upperLeft.col; col <= lowerRight.col; ++col) {
            if (prob.dots.count (Coordinate {row, col}) == 1) {
                out << "#";
            }
            else {
                out << " ";
            }
        }
        out << "\n";
    }
    return out;
}

/// \brief Gets the input.
/// \return A problem to solve.
Problem getInput () {
    Problem problem;
    std::string line;
    while (std::cin >> line) {
        size_t pos = line.find (',');
        if (pos != std::string::npos) {
            problem.dots.insert (Coordinate {std::stoi (line.substr (pos + 1)), std::stoi (line.substr (0, pos))});
        }
        else if (line == "fold") {
            std::cin >> line;
            assert (line == "along");
            std::cin >> line;
            pos = line.find ('=');
            if (line.at (pos - 1) == 'y') {
                problem.folds.push_back ({true, std::stoi (line.substr (pos + 1))});
            }
            else {
                assert (line.at (pos - 1) == 'x');
                problem.folds.push_back ({false, std::stoi (line.substr (pos + 1))});
            }
        }
        else {
            assert (line == "");
        }
    }
    return problem;
}

/// Performs the next fold to the paper.
/// \param[in] orig The current problem.
/// \return The way the problem will look after doing the next fold.
Problem afterNextFold (Problem const& orig) {
    Fold fold = orig.folds.at (0);
    Problem revised;
    revised.folds = std::vector<Fold> (orig.folds.cbegin () + 1, orig.folds.cend ());
    if (fold.up) {
        for (Coordinate const& c : orig.dots) {
            if (c.row < fold.where) {
                revised.dots.insert (c);
            }
            else {
                revised.dots.insert (Coordinate (c.row - (c.row - fold.where) * 2, c.col));
            }
        }
    }
    else {
        for (Coordinate const& c : orig.dots) {
            if (c.col < fold.where) {
                revised.dots.insert (c);
            }
            else {
                revised.dots.insert (Coordinate (c.row, c.col - (c.col - fold.where) * 2));
            }
        }
    }
    return revised;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem prob = getInput ();
    Problem postFold = afterNextFold (prob);
    std::cout << postFold.dots.size() << "\n";
    while (!postFold.folds.empty ()) {
        postFold = afterNextFold (postFold);
    }
    std::cout << postFold << "\n";
    return 0;
}