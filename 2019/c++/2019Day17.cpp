/// \file 2019Day17.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/17.

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

using Picture = std::vector<std::vector<char>>;

Picture generatePicture (NumbersList const& prog) {
    Picture picture;
    picture.push_back ({});
    ICComputer comp (prog, {});
    comp.executeAllInstructions ();
    NumbersList output = comp.getOutputs ();
    for (Number num : output) {
        if (num == '\n') {
            picture.push_back ({});
        }
        else {
            picture.back ().push_back ((char)num);
        }
    }
    return picture;
}

void drawPicture (Picture const& picture) {
    for (std::vector<char> const& vec : picture) {
        for (char c : vec) {
            std::cout << c;
        }
        std::cout << "\n";
    }
}

unsigned int sumIntersectionDistances (Picture const& picture) {
    unsigned int total {0U};
    for (unsigned int row {1U}; row < picture.size () - 1 && !picture[row + 1].empty (); ++row) {
        for (unsigned int col {1U}; col < picture[row].size () - 1; ++col) {
            if (picture[row][col] == '#' && picture[row - 1][col] == '#' && picture[row + 1][col] == '#' && picture[row][col - 1] == '#' && picture[row][col + 1] == '#') {
                total += row * col;
            }
        }
    }
    return total;
}

int main () {
    std::ifstream fin ("../inputs/Day17.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    Picture view = generatePicture (prog);
    drawPicture (view);
    std::cout << sumIntersectionDistances (view) << "\n";
    return 0;
}
