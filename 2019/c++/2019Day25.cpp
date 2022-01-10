/// \file 2019Day25.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/25.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <queue>
#include <array>

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


void playGame (NumbersList const& prog) {
    ICComputer comp {prog, {}};
    std::string line;
    while (true) {
        comp.executeUntilMissingInput ();
        std::cout << decode (comp.getNewOutputs ());
        std::getline (std::cin, line);
        for (Number n : encode (line + "\n")) {
            comp.addInput (n);
        }
    }
}

int main () {
    std::ifstream fin ("../inputs/Day25.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    playGame(prog);
    return 0;
}

/*
          006
           |
           |
     004--005
      |    |
      |    |
      |   008
      |
      |
     003--007
      |
      |
     002--009
      |    |
      |    |
      |   010--013
      |    |
      |    |
      |   011--012
      |
      |
     001--000
      |    |
      |    |
015--014   |
 |         |
 v         |
016        |
           |
           |
     018--017--019


000: Hull Breach (start location)
001: Storage
002: Crew Quarters (has jam)
003: Hot Chocolate Fountain (has astronaut ice cream)
004: Kitchen
005: Hallway (has molten lava that kills you if you take it)
006: Corridor (has infinite loop that kills you if you take it)
007: Passages (has escape pod that launches you into space if you take it)
008: Observatory (has space law space brochure)
009: Stables
010: Arcade (has asterisk)
011: Engineering (has klein bottle)
012: Navigation (has spool of cat6)
013: Science Lab (has giant electromagnet that prevents you from moving)
014: Holodeck
015: Security Checkpoint
016: ? (where I need the right weight to go)
017: Sick Bay (has photons that kill you if you take them)
018: Gift Wrapping Center (has shell)
019: Warp Drive Maintenance (has space heater)


each item individually is too light
every pair of items is too light except for {jam, astronaut ice cream} and {shell, astronaut ice cream}, both of which are too heavy

astronaut ice cream is 1st and *required* because all other items combined is too light
jam is 2nd/3rd and *must be skipped* because adding it to astronaut ice cream becomes too heavy
shell is 2nd/3rd and *must be skipped* because adding it to astronaut ice cream becomes too heavy
klein bottle is 4th and *required*
space heater is 5th and *required*
spool of cat6 is 6th and *must be skipped*
space law space brochure is 7th and *must be skipped*
asterisk is 8th and *required*


items: spool of cat6     space law space brochure     asterisk     jam     shell     astronaut ice cream     space heater     klein bottle
*/