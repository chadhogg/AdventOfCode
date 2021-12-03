/// \file 2021Day03.cpp
/// \author Chad Hogg
/// \brief My solution to 


#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> getInput () {
    std::vector<std::string> inputs;
    std::string s;
    while (std::cin >> s) {
        inputs.push_back (s);
    }
    return inputs;
}

std::string mostCommon (std::vector<std::string> const& inputs) {
    std::string result;
    while (result.size () < inputs.at (0).size ()) {
        unsigned int count = 0;
        for (std::string const& str : inputs) {
            if (str.at (result.size ()) == '1') {
                ++count;
            }
        }
        if (count * 2U > inputs.size ()) {
            result.push_back ('1');
        }
        else {
            result.push_back ('0');
        }
    }
    return result;
}

std::string bitwiseXor (std::string bits) {
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

int stringToInt (std::string bits) {
    if (bits.size () == 1) {
        return atoi (bits.c_str ());
    }
    else {
        return stringToInt (bits.substr(0UL, bits.size () - 1)) * 2 + stringToInt (bits.substr (bits.size () - 1));
    }
}

std::string filter (std::vector<std::string> inputs, bool most) {
    for (unsigned int bit {0U}; bit < inputs.at (0).size (); ++bit) {
        unsigned int count = 0;
        for (std::string const& str : inputs) {
            if (str.at (bit) == '1') {
                ++count;
            }
        }
        if (most) {
            if (count * 2U >= inputs.size ()) {
                for (auto it = inputs.begin (); it != inputs.end (); ) {
                    if ((*it).at (bit) == '0') {
                        inputs.erase (it);
                    }
                    else {
                        ++it;
                    }
                }
            }
            else {
                for (auto it = inputs.begin (); it != inputs.end (); ) {
                    if ((*it).at (bit) == '1') {
                        inputs.erase (it);
                    }
                    else {
                        ++it;
                    }
                }
            }

        }
        else {
            if (count * 2U >= inputs.size ()) {
                for (auto it = inputs.begin (); it != inputs.end (); ) {
                    if ((*it).at (bit) == '1') {
                        inputs.erase (it);
                    }
                    else {
                        ++it;
                    }
                }
            }
            else {
                for (auto it = inputs.begin (); it != inputs.end (); ) {
                    if ((*it).at (bit) == '0') {
                        inputs.erase (it);
                    }
                    else {
                        ++it;
                    }
                }
            }

        }
        if (inputs.size () == 1) {
            return inputs.at (0);
        }
    }
    throw std::runtime_error ("Should never happen.");
}

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