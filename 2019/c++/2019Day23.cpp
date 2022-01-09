/// \file 2019Day23.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/23.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <queue>
#include <array>

#include "utilities.hpp"
#include "intcode.hpp"

constexpr Number NUM_COMPUTERS = 50;
constexpr Number STOP_ADDRESS = 255;
constexpr Number NO_PACKETS_WAITING = -1;

struct Packet {
    Number X;
    Number Y;
};

Number part1 (NumbersList prog) {
    std::array<ICComputer, NUM_COMPUTERS> computers;
    std::array<std::queue<Packet>, NUM_COMPUTERS> waitingPackets;
    for (std::size_t index {0U}; index < NUM_COMPUTERS; ++index) {
        computers[index].loadProgram (prog, {(Number)index});
        computers[index].executeUntilMissingInput ();
    }
    while (true) {
        for (std::size_t index {0U}; index < NUM_COMPUTERS; ++index) {
            if (waitingPackets[index].empty ()) {
                computers[index].addInput (NO_PACKETS_WAITING);
            }
            else {
                Packet packet = waitingPackets[index].front ();
                waitingPackets[index].pop ();
                computers[index].addInput (packet.X);
                computers[index].addInput (packet.Y);
            }
            computers[index].executeUntilMissingInput ();
            NumbersList packets = computers[index].getNewOutputs ();
            assert (packets.size () % 3 == 0);
            for (std::size_t pIndex {0U}; pIndex < packets.size (); pIndex += 3) {
                if (packets[pIndex] == STOP_ADDRESS) {
                    return packets[pIndex + 2];
                }
                else {
                    waitingPackets[packets[pIndex]].push ({packets[pIndex + 1], packets[pIndex + 2]});
                }
            }
        }
    }
}

int main () {
    std::ifstream fin ("../inputs/Day23.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    std::cout << part1 (prog) << "\n";
    return 0;
}
