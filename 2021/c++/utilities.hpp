#ifndef AOC_2021_UTILITIES_HPP
#define AOC_2021_UTILITIES_HPP
/// \file utilities.hpp
/// \author Chad Hogg
/// \brief Some code that I suspect I will find useful more than once during Advent Of Code 2021.

#include <iostream>
#include <vector>

template<typename T>
T read (std::istream& in) {
    T temp;
    in >> temp;
    return temp;
}

template<typename T>
T read () {
    return read<T> (std::cin);
}

template<typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> const& vec) {
    if (!vec.empty ()) {
        out << "[";
        bool first {true};
        for (T const& val : vec) {
            if (!first) {
                out << ", ";
            }
            out << val;
            first = false;
        }
        out << "]";
    }
    return out;
}


#endif//AOC_2021_UTILITIES_HPP