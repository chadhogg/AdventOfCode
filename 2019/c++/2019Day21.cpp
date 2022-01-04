/// \file 2019Day21.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/21.

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

std::vector<Number> encode (std::string const& str) {
    std::vector<Number> numbers;
    for (char c : str) { numbers.push_back (c); }
    return numbers;
}

std::string decode (std::vector<Number> const& numbers) {
    std::string str = "";
    for (Number n : numbers) { str += (char)n; }
    return str;
}

Number runSpringDroid (NumbersList const& intcode, std::string const& springscript) {
    ICComputer comp (intcode, {});
    for (Number n : encode (springscript)) { comp.addInput (n); }
    comp.executeAllInstructions ();
    std::cout << decode (comp.getOutputs ());
    return comp.getOutputs ().back ();
}

int main () {
    std::ifstream fin ("../inputs/Day21.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    std::string jumpIfGroundIn4 = "NOT D J\nNOT J J\nWALK\n";
    std::string jumpIfGroundIn4Hole1Or2Or3 =    "NOT C T\n"
                                                "NOT B J\n"
                                                "OR T J\n"
                                                "NOT A T\n"
                                                "OR T J\n"
                                                "NOT D T\n"
                                                "NOT T T\n"
                                                "AND T J\n"
                                                "WALK\n";
    std::cout << runSpringDroid (prog, jumpIfGroundIn4Hole1Or2Or3) << "\n";
    std::string sameButWithRun =    "NOT C T\n"
                                    "NOT B J\n"
                                    "OR T J\n"
                                    "NOT A T\n"
                                    "OR T J\n"
                                    "NOT D T\n"
                                    "NOT T T\n"
                                    "AND T J\n"
                                    "RUN\n";
    // I had no motivation to just keep trying things or to think more about this when there are more inputs than I am allowed instructions.
    std::string fromReddit =    "OR A J\n"
                                "AND B J\n"
                                "AND C J\n"
                                "NOT J J\n"
                                "AND D J\n"
                                "OR E T\n"
                                "OR H T\n"
                                "AND T J\n"
                                "RUN\n";
    std::cout << runSpringDroid (prog, fromReddit) << "\n";
    return 0;
}
