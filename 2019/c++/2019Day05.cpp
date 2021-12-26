/// \file 2019Day05.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/5.

#include <iostream>

#include "utilities.hpp"
#include "intcode.hpp"


int main () {
    std::string input = read<std::string> ();
    NumbersList prog = parseNumbersList (input);
    ICComputer comp {prog, {1}};
    comp.executeAllInstructions ();
    std::cout << comp.getOutputs () << "\n";
    return 0;
}
