#ifndef AOC_2021_UTILITIES_HPP
#define AOC_2021_UTILITIES_HPP
/// \file utilities.hpp
/// \author Chad Hogg
/// \brief Some code that I suspect I will find useful more than once during Advent Of Code 2021.

#include <iostream>
#include <vector>
#include <array>

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

template<typename T, std::size_t N>
std::ostream& operator<< (std::ostream& out, std::array<T, N> const& arr) {
    if (!arr.empty ()) {
        out << "[";
        bool first {true};
        for (T const& val : arr) {
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

/// A position in a 2-D grid.
struct Coordinate {
    /// Vertical position -- top is 0, increases as you go down.
    int row;
    /// Horizontal position -- left is 0, increases as you go right.
    int col;

    /// \brief Constructs a coordinate.
    /// \param[in] r The row.
    /// \param[in] c The col.
    /// \post Those values are copied to the fields.
    /// \note I needed to write this to avoid a compiler error, and I'm not sure why.
    Coordinate (int r, int c) 
    : row (r), col (c)
    {
    }

    /// Checks whether or not two coordinates are the same.
    /// \param[in] other Another coordinate.
    /// \return True if this coordinate matches the other.
    bool operator== (Coordinate const& other) const {
        return row == other.row && col == other.col;
    }

};

/// Hashes a coordinate.
/// \param[in] loc A coordinate.
/// \return That coordinate's hash value.
template<>
struct std::hash<Coordinate> {
    std::size_t operator()(Coordinate const& loc) const {
        return loc.row + loc.col * 1234567;
    }
};


#endif//AOC_2021_UTILITIES_HPP