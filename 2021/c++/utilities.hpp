#ifndef AOC_2021_UTILITIES_HPP
#define AOC_2021_UTILITIES_HPP
/// \file utilities.hpp
/// \author Chad Hogg
/// \brief Some code that I suspect I will find useful more than once during Advent Of Code 2021.

#include <iostream>
#include <vector>
#include <array>

/// Reads a value from an input stream, but doesn't tell you whether or not it succeeded.
/// \param[in] in The stream.
/// \return The value that was read.
template<typename T>
T read (std::istream& in) {
    T temp;
    in >> temp;
    return temp;
}

/// Reads a value from standard input, but doesn't tell you whether or not it succeeded.
/// \return The value that was read.
template<typename T>
T read () {
    return read<T> (std::cin);
}

/// \brief Prints the contents of a vector to an output stream.
/// \param[in] out The stream.
/// \param[in] vec The vector.
/// \return The stream.
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

/// \brief Prints the contents of an array to an output stream.
/// \param[in] out The stream.
/// \param[in] arr The array.
/// \return The stream.
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

/// \brief Inserts a coordinate into an output stream.
/// \param[inout] out The stream.
/// \param[in] c The coordinate.
/// \return The stream.
/// \post The coordinate has been inserted into the output stream.
std::ostream& operator<< (std::ostream & out, Coordinate const& c) {
    out << "(" << c.row << ", " << c.col << ")";
    return out;
}

/// \brief An object that hashes pairs.
template<typename A, typename B>
struct std::hash<std::pair<A, B>> {
    /// \brief Hashes a pair of things that are already themselves hashable.
    /// \param[in] thing A pair that should be hashed.
    /// \return That pair's hash value.
    std::size_t operator()(std::pair<A, B> const& thing) const {
        auto first = std::hash<A>() (thing.first);
        auto second = std::hash<B>() (thing.second);
        return first + second * 1234567U;
    }
};

/// \brief A function that makes pairs printable.
template<typename A, typename B>
std::ostream & operator<< (std::ostream & out, std::pair<A, B> const& pair) {
    out << "<" << pair.first << ", " << pair.second << ">";
    return out;
}

template<typename A, typename B>
std::string to_string (std::pair<A, B> const& pair) {
    return "<" + std::to_string (pair.first) + ", " + std::to_string (pair.second) + ">";
}


#endif//AOC_2021_UTILITIES_HPP