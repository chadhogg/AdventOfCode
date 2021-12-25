/// \file 2019Day02.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/2.

#include <iostream>

#include "utilities.hpp"
#include "intcode.hpp"

void test (std::string input) {
    ICComputer comp {input};
    comp.executeAllInstructions ();
    std::cout << input << " -> " << comp.toString () << "\n";
}

int search (ICComputer & comp, ICProgram & prog) {
    for (int noun {0}; noun < 99; ++noun) {
        for (int verb {0}; verb < 99; ++verb) {
            prog[1] = noun;
            prog[2] = verb;
            comp.loadProgram (prog);
            comp.executeAllInstructions ();
            if (comp.getNumber (0) == 19690720) {
                return 100 * noun + verb;
            }
        }
    }
    throw std::runtime_error ("Did not find any working inputs.");
}

int main () {
    /*
    test ("1,0,0,0,99");
    test ("2,3,0,3,99");
    test ("2,4,4,5,99,0");
    test ("1,1,1,4,99,5,6,0,99");
    */

    std::string input = read<std::string> ();
    ICProgram prog = parseProgram (input);
    prog[1] = 12;
    prog[2] = 2;
    ICComputer comp {prog};
    comp.executeAllInstructions ();
    std::cout << comp.getNumber (0U) << "\n";

    std::cout << search (comp, prog) << "\n";
    return 0;
}
