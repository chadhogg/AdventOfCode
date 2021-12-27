/// \file 2019Day07.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/7.

#include <iostream>
#include <algorithm>
#include <array>

#include "utilities.hpp"
#include "intcode.hpp"

constexpr unsigned int NUM_AMPLIFIERS {5U};
using Phases = std::array<int, NUM_AMPLIFIERS>;
constexpr Phases PART1_PHASES = {0, 1, 2, 3, 4};
constexpr Phases PART2_PHASES = {5, 6, 7, 8, 9};

int findMaxThrusterSignal (NumbersList const& prog) {
    ICComputer comp;
    int largest {0};
    Phases phases = PART1_PHASES;
    int output;
    do {
        output = 0;
        for (unsigned int amp {0U}; amp < NUM_AMPLIFIERS; ++amp) {
            comp.loadProgram (prog, {phases[amp], output});
            comp.executeAllInstructions ();
            output = comp.getOutputs ()[0];
        }
        largest = std::max (largest, output);
    } while (std::next_permutation (phases.begin (), phases.end ()));

    return largest;
}

int feedbackOperation (NumbersList const& prog, Phases const& phases) {
    std::array<ICComputer, NUM_AMPLIFIERS> amps;
    for (unsigned int index {0U}; index < NUM_AMPLIFIERS; ++index) {
        ICComputer & amp = amps[index];
        amp.loadProgram (prog, {phases[index]});
    }
    amps[0].addInput (0);
    while (!amps.back ().isTerminated ()) {
        for (unsigned int index {0U}; index < NUM_AMPLIFIERS; ++index) {
            amps[index].executeUntilMissingInput ();
            amps[(index + 1) % NUM_AMPLIFIERS].addInput (amps[index].getOutputs ().back ());
        }
    }
    return amps.back ().getOutputs ().back ();
}

int bestFeedback (NumbersList const& prog) {
    int largest {0};
    Phases phases = PART2_PHASES;
    do {
        largest = std::max (largest, feedbackOperation (prog, phases));
    } while (std::next_permutation (phases.begin (), phases.end ()));
    return largest;
}

int main () {
    NumbersList prog = parseNumbersList (read<std::string> ());
    std::cout << findMaxThrusterSignal (prog) << "\n";
    std::cout << bestFeedback (prog) << "\n";
    return 0;
}
