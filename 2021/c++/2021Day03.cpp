/// \file 2021Day03.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/3.


#include <string>
#include <vector>
#include <iostream>
#include "utilities.hpp"

/// \brief Gets a vector of bit strings from standard input.
/// \return The input.
std::vector<std::string> getInput () {
    std::vector<std::string> inputs;
    std::string s;
    while (std::cin >> s) {
        inputs.push_back (s);
    }
    return inputs;
}

/// \brief Finds the most common character at a position in a vector of bit strings.
/// \param[in] inputs The bit strings.
/// \param[in] bitIndex The index whose most common element we want.
/// \return A '1' if those are more common in that position, or a '0' otherwise.
std::pair<char, unsigned int> mostCommon (std::vector<std::string> const& inputs, unsigned int bitIndex) {
    unsigned int count = 0;
    for (std::string const& str : inputs) {
        if (str.at (bitIndex) == '1') {
            ++count;
        }
    }
    if (count * 2U > inputs.size ()) {
        return {'1', count};
    }
    else {
        return {'0', count};
    }
}

/// \brief Builds a string containing the most common elements of other bit strings.
/// \param[in] inputs The bit strings.
/// \return A new bit string containing the most common elements of the others.
std::string mostCommon (std::vector<std::string> const& inputs) {
    std::string result;
    while (result.size () < inputs.at (0).size ()) {
        result.push_back (mostCommon (inputs, result.size ()).first);
    }
    return result;
}

/// \brief Gets the opposite of a bit string.
/// \param[in] bits The string the find the opposite of it.
/// \return The opposite of that bit string.
std::string bitwiseXor (std::string const& bits) {
    std::string result;
    for (unsigned int index {0U}; index < bits.size (); ++index) {
        if (bits.at (index) == '0') {
            result.push_back ('1');
        }
        else {
            result.push_back ('0');
        }
    }
    return result;
}

/// \brief Converts a bit string to an actual integer.
/// \param[in] bits The bit string.
/// \return Its integer representation.
int stringToInt (std::string const& bits) {
    if (bits.size () == 1) {
        return atoi (bits.c_str ());
    }
    else {
        return stringToInt (bits.substr(0UL, bits.size () - 1)) * 2 + stringToInt (bits.substr (bits.size () - 1));
    }
}


/// \brief Filters strings, keeping only those that have the most (least) common value in each successive position.
/// \param[in] inputs The inputs.  Not passed by reference, because we are going to destructively process it.
/// \param[in] most True if we want to keep the most common, or false to keep the least common.
/// \return The only string that passes through the filter.
std::string filter (std::vector<std::string> inputs, bool most) {
    for (unsigned int bit {0U}; bit < inputs.at (0).size (); ++bit) {
        std::pair<char, unsigned int> common = mostCommon (inputs, bit);
        if (common.second * 2 == inputs.size ()) {
            common.first = '1';
        }
        for (auto it = inputs.begin (); it != inputs.end (); /* intentionally blank */) {
            if (most ^ ((*it).at (bit) == common.first)) {
                inputs.erase (it);
            }
            else {
                ++it;
            }
        }
        if (inputs.size () == 1) {
            return inputs.at (0);
        }
    }
    throw std::runtime_error ("Should never happen.");
}

/// \brief Runs the program.
/// \return Always 0.
int main () {

    std::vector<std::string> inputs = getInput ();
    std::string gamma = mostCommon (inputs);
    std::string epsilon = bitwiseXor (gamma);
    std::cout << stringToInt (gamma) * stringToInt (epsilon) << std::endl;
    std::string oxygen = filter (inputs, true);
    std::string scrubber = filter (inputs, false);
    std::cout << stringToInt (oxygen) * stringToInt (scrubber) << std::endl;


    return 0;
}