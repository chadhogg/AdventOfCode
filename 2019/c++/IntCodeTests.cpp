/// \file IntCodeTests.cpp
/// \author Chad Hogg
/// \brief A collection of unit tests to ensure that future changes to my IntCode computer do not break previously working code.

#include <string>
#include <iostream>

#include "intcode.hpp"

bool runTest (std::string const& input, std::string const& expected) {
    ICComputer comp {input};
    comp.executeAllInstructions ();
    std::string actual = comp.toString ();
    return expected == actual;
}

void day02Examples () {
    if (!runTest ("1,9,10,3,2,3,11,0,99,30,40,50", "3500,9,10,70,2,3,11,0,99,30,40,50")) {
        std::cout << "Day 02 first example failed!\n";
    }
    if (!runTest ("1,0,0,0,99", "2,0,0,0,99")) {
        std::cout << "Day 02 second example failed!\n";
    }
    if (!runTest ("2,3,0,3,99", "2,3,0,6,99")) {
        std::cout << "Day 02 third example failed!\n";
    }
    if (!runTest ("2,4,4,5,99,0", "2,4,4,5,99,9801")) {
        std::cout << "Day 02 fourth example failed!\n";
    }
    if (!runTest ("1,1,1,4,99,5,6,0,99", "30,1,1,4,2,5,6,0,99")) {
        std::cout << "Day 02 fifth example failed!\n";
    }
}

void day05Examples () {
    if (!runTest ("", "")) {
        std::cout << "Day 05 first example failed!\n";
    }
}

int main () {
    day02Examples ();
    std::cout << "Finished running tests.\n";
    return 0;
}