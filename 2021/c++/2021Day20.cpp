/// \file 2021Day20.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/20.



#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <vector>

#include "utilities.hpp"

using BitVector = std::vector<bool>;
using Image = std::vector<BitVector>;


struct Problem {
    BitVector m_algorithm;
    Image m_image;
    bool m_infiniteIsLit;
};


BitVector stringToBitVector (std::string const& str) {
    BitVector vec;
    for (char c : str) {
        if (c == '.') {
            vec.push_back (false);
        }
        else if (c == '#') {
            vec.push_back (true);
        }
        else {
            throw std::runtime_error ("String contained unexpected character.");
        }
    }
    return vec;
}



Problem getInput () {
    Problem problem;
    std::string line;
    std::cin >> line;
    problem.m_algorithm = stringToBitVector (line);

    while (std::cin >> line) {
        problem.m_image.push_back (stringToBitVector (line));
    }
    problem.m_infiniteIsLit = false;
    return problem;
}



unsigned int bitVectorToNumber (BitVector const& bits) {
    unsigned int answer {0U};
    for (bool bit : bits) {
        answer <<= 1;
        answer += bit;
    }
    return answer;
}



BitVector neighborhood (Image const& image, int row, int col, bool infiniteIsLit) {
    BitVector bits;
    for (int r {row - 1}; r <= row + 1; ++r) {
        for (int c {col - 1}; c <= col + 1; ++c) {
            if (r < 0 || c < 0 || r >= (int)image.size () || c >= (int)image.at (0).size ()) {
                bits.push_back (infiniteIsLit);
            }
            else {
                bits.push_back (image.at (r).at (c));
            }
        }
    }
    return bits;
}



Problem iterate (Problem const& input) {
    Problem output {input};
    output.m_infiniteIsLit = !output.m_infiniteIsLit && output.m_algorithm.front ();
    output.m_image.clear ();
    for (unsigned int oRow {0U}; oRow < input.m_image.size () + 4; ++oRow) {
        int iRow = oRow - 2;
        BitVector row;
        for (unsigned int oCol {0U}; oCol < input.m_image.at (0).size () + 4; ++oCol) {
            int iCol = oCol - 2;
            unsigned int index = bitVectorToNumber (neighborhood( input.m_image, iRow, iCol, input.m_infiniteIsLit));
            row.push_back (input.m_algorithm.at (index));
        }
        output.m_image.push_back (row);
    }
    return output;
}



Problem iterate (Problem const& input, unsigned int count) {
    Problem result {input};
    for (unsigned int c {0U}; c < count; ++c) {
        result = iterate (result);
    }
    return result;
}



unsigned int countLights (Problem const& input) {
    unsigned int count {0U};
    for (unsigned int row {0U}; row < input.m_image.size (); ++row) {
        for (unsigned int col {0U}; col < input.m_image.at (0).size (); ++col) {
            if (input.m_image.at (row).at (col)) {
                ++count;
            }
        }
    }
    return count;
}


/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem problem = getInput ();
    Problem after2 = iterate (problem, 2);
    std::cout << countLights (after2) << "\n";
    Problem after50 = iterate (after2, 48);
    std::cout << countLights (after50) << "\n";
    return 0;
}