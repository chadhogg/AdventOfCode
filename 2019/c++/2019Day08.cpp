/// \file 2019Day08.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/8.

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <climits>

#include "utilities.hpp"

constexpr unsigned int WIDTH {25U};
constexpr unsigned int HEIGHT {6U};

constexpr int BLACK {0};
constexpr int WHITE {1};
constexpr int TRANSPARENT {2};

using Layer = std::array<std::array<char, WIDTH>, HEIGHT>;
using Image = std::vector<Layer>;
using Counts = std::vector<unsigned int>;

Image getInput () {
    Image image;
    std::string input = read<std::string> ();
    unsigned int index {0U};
    while (index < input.size ()) {
        Layer layer;
        for (unsigned int y {0U}; y < HEIGHT; ++y) {
            for (unsigned int x {0U}; x < WIDTH; ++x) {
                layer[y][x] = input[index] - '0';
                ++index;
            }
        }
        image.push_back (layer);
    }
    return image;
}

std::vector<Counts> computeCounts (Image const& image) {
    std::vector<Counts> counts;
    for (Layer const& layer : image) {
        Counts count = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (unsigned int y {0U}; y < HEIGHT; ++y) {
            for (unsigned int x {0U}; x < WIDTH; ++x) {
                ++count[layer[y][x]];
            }
        }
        counts.push_back (count);
    }
    return counts;
}

unsigned int part1 (std::vector<Counts> const& counts) {
    Counts count = counts.back ();
    for (Counts const& current : counts) {
        if (current[0] < count[0]) { count = current; }
    }
    return count[1] * count[2];
}

Layer decode (Image const& image) {
    Layer output;
    for (unsigned int y {0U}; y < HEIGHT; ++y) {
        for (unsigned int x {0U}; x < WIDTH; ++x) {
            unsigned int layer = 0;
            while (image[layer][y][x] == TRANSPARENT) { ++layer; }
            output[y][x] = image[layer][y][x];
        }
    }
    return output;
}

void print (Layer const& layer) {
    for (unsigned int y {0U}; y < HEIGHT; ++y) {
        for (unsigned int x {0U}; x < WIDTH; ++x) {
            if (layer[y][x] == BLACK) { std::cout << " "; }
            else if (layer[y][x] == WHITE) { std::cout << "#"; }
            else { throw std::runtime_error ("Unexpected digit " + std::to_string (layer[y][x])); }
        }
        std::cout << "\n";
    }
}


int main () {
    Image image = getInput ();
    std::vector<Counts> countsPerLayer = computeCounts (image);
    std::cout << part1 (countsPerLayer) << "\n";
    print (decode (image));
    return 0;
}
