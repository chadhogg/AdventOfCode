/// \file 2019Day19.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/19.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <unordered_map>

#include "utilities.hpp"
#include "intcode.hpp"

unsigned int countAffectedPoints (NumbersList const& prog, unsigned int maxRow, unsigned int maxCol) {
    unsigned int result {0U};
    unsigned int lowCol {0U};
    ICComputer comp;
    for (unsigned int row {0U}; row <= maxRow; ++row) {
        bool found1 = false;
        for (unsigned int col {lowCol}; col <= maxCol; ++col) {
            comp.loadProgram (prog, {col, row});
            comp.executeAllInstructions ();
            assert (comp.getOutputs ().size () == 1);
            Number output = comp.getOutputs ().back ();
            if (output == 1) {
                ++result;
                if (!found1) { lowCol = col; }
                found1 = true;
            }
            else if (found1) {
                break;
            }
        }
    }
    return result;
}

int main () {
    std::ifstream fin ("../inputs/Day19.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    std::cout << countAffectedPoints (prog, 49, 49) << "\n";
    return 0;
}
