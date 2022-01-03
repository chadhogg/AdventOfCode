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

long long find100SZquare (NumbersList const& prog) {
    std::vector<std::pair<Number, Number>> rows;
    ICComputer comp;
    Number currentRow {0U};
    Number startCol {0U};
    while (true) {
        bool found1 = false;
        Number currentCol {startCol};
        Number first1 {0};
        while (true) {
            comp.loadProgram (prog, {currentCol, currentRow});
            comp.executeAllInstructions ();
            Number output = comp.getOutputs ().back ();
            if (output == 1) {
                if (!found1) {
                    first1 = currentCol;
                    found1 = true;
                }
            }
            else if (found1) {
                rows.push_back ({first1, currentCol - 1});
                startCol = first1;
                Coordinate upperLeft {(int)currentRow - 99, (int)first1};
                Coordinate lowerRight {(int)currentRow, (int)first1 + 99};
                if (currentRow >= 99 && rows[currentRow - 99].second >= first1 + 99) {
                    return upperLeft.col * 10000LL + upperLeft.row;
                }
                break;
            }
            else if (currentCol - startCol > 1000) { 
                // give up, assume this row doesn't have any 1s at all
                rows.push_back ({-1, -1});
                break;
            }
            ++currentCol;
        }
        ++currentRow;
    }
}

int main () {
    std::ifstream fin ("../inputs/Day19.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    std::cout << countAffectedPoints (prog, 49, 49) << "\n";
    std::cout << find100SZquare (prog) << "\n";
    return 0;
}
