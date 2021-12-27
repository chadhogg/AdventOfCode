/// \file 2019Day09.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/9.

#include <iostream>
#include <algorithm>
#include <array>

#include "utilities.hpp"
#include "intcode.hpp"

NumbersList run (NumbersList const& prog, Number input) {
    ICComputer comp;
    comp.loadProgram (prog, {input});
    comp.executeAllInstructions ();
    return comp.getOutputs ();
}

int main () {
    NumbersList prog = parseNumbersList (read<std::string> ());
    std::cout << run (prog, 1) << "\n";
    std::cout << run (prog, 2) << "\n";
    return 0;
}
