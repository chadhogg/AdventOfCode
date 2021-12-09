/// \file 2021Day09.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/9.

#include <iostream>
#include <vector>
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "utilities.hpp"

/// The largest possible height value any location can have.
constexpr unsigned int MAX_HEIGHT = 9;
/// The smallest possible height value any location can have.
constexpr unsigned int MIN_HEIGHT = 0;

/// The height of a location.
using Height = unsigned int;

/// A 2-d array of heights.
using HeightMap = std::vector<std::vector<Height>>;

/// A location in a 2D grid.
struct Location {
    /// The y-coordinate (0 at top, increasing as you go down vertically).
    unsigned int row;
    /// The x-coordinate (0 at left, increasing as you go across horizontally).
    unsigned int col;

    /// Tests whether or not two locations are equal to each other.
    /// \param[in] other The other location.
    /// \return True if this location is equal to the other one.
    bool operator== (Location const& other) const {
        return row == other.row && col == other.col;
    }
};

/// A collection of locations that all drain to the same low point.
using Basin = std::unordered_set<Location>;

template<>
/// A hash function for locations.
struct std::hash<Location> {
    /// \brief Hashes a location.
    /// \param[in] loc The location to hash.
    /// \return It's hash value.
    std::size_t operator()(Location const& loc) const {
        return loc.row + loc.col * 1234567;
    }
};

/// \brief Reads a height map from standard input.
/// \return A height map.
HeightMap getInput () {
    HeightMap hm;
    std::string line;
    while (std::cin >> line) {
        hm.push_back (std::vector<Height> ());
        for (char c : line) {
            hm.back ().push_back (c - '0');
        }
    }
    return hm;
}

/// \brief Finds the "low points" in a height map.
/// A "low point" is one that has no lower point in any cardinal direction.
/// \param[in] hm The heigh map.
/// \return A set of locations that are loc points.
std::unordered_set<Location> findLowPoints (HeightMap const& hm) {
    std::unordered_set<Location> lowPoints;
    for (unsigned int row {0U}; row < hm.size (); ++row) {
        for (unsigned int col {0U}; col < hm.at (row).size (); ++col) {
            Height value = hm.at (row).at (col);
            bool lowest = true;
            if (row > 0 && hm.at (row - 1).at (col) <= value) { lowest = false; }
            if (row < hm.size () - 1 && hm.at (row + 1).at (col) <= value) { lowest = false; }
            if (col > 0 && hm.at (row).at (col - 1) <= value) { lowest = false; }
            if (col < hm.at (row).size () - 1 && hm.at (row).at (col + 1) <= value) { lowest = false; }
            if (lowest) { lowPoints.insert ({row, col}); }
        }
    }
    return lowPoints;
}

/// \brief Adds up the risk levels of all low points on a height map.
/// \param[in] hm The height map.
/// \param[in] locations The low points.
/// \return The sum of each of their heights plus one.
unsigned int sumRiskLevels (HeightMap const& hm, std::unordered_set<Location> const& locations) {
    unsigned int sum {0U};
    for (Location const& loc : locations) {
        sum += hm.at (loc.row).at (loc.col) + 1;
    }
    return sum;
}

/// \brief Recursively descends from a location until reaching a low point, or a point from which we know which low point would be reached.
/// \param[in] hm The heigh map.
/// \param[inout] basins A map from low points to their basins.
/// \param[inout] finished A map from locations to the low points they drain to.
/// \param[in] loc The location that should be descended from.
/// \return The low point that is reached when desccending from this this location.
/// \post This location, and all the ones you pass through getting from it to a low point, have been added to that low point's basin and are marked finished.
Location descend (HeightMap const& hm, std::unordered_map<Location, Basin> & basins, std::unordered_map<Location, Location> finished, Location loc) {
    if (finished.count (loc) == 1) {
        return finished.at (loc);
    }
    else {
        Height north = loc.row > 0 ? hm.at (loc.row - 1).at (loc.col) : MAX_HEIGHT;
        Height south = loc.row < hm.size () - 1 ? hm.at (loc.row + 1).at (loc.col) : MAX_HEIGHT;
        Height west = loc.col > 0 ? hm.at (loc.row).at (loc.col - 1) : MAX_HEIGHT;
        Height east = loc.col < hm.at (loc.row).size () - 1 ? hm.at (loc.row).at (loc.col + 1) : MAX_HEIGHT;
        Location smallest;
        if (north <= south && north <= west && north <= east) {
            smallest.row = loc.row - 1;
            smallest.col = loc.col;
        }
        else if (south <= west && south <= east) {
            smallest.row = loc.row + 1;
            smallest.col = loc.col;
        }
        else if (west <= east) {
            smallest.row = loc.row;
            smallest.col = loc.col - 1;
        }
        else {
            smallest.row = loc.row;
            smallest.col = loc.col + 1;
        }
        Location result = descend (hm, basins, finished, smallest);
        basins.at (result).insert (loc);
        finished[loc] = result;
        return result;
    }
}

/// \brief Finds all of the basins in a height map.
/// \param[in] hm The heigh map.
/// \param[in] lowPoints The low points.
/// \return A map from low points to their basins.
/// \note Locations with a height of 9 are not in any basin.
std::unordered_map<Location, Basin> computeBasins (HeightMap const& hm, std::unordered_set<Location> const& lowPoints) {
    std::unordered_map<Location, Basin> basins;
    std::unordered_map<Location, Location> finished;
    for (Location const& lp : lowPoints) {
        basins[lp] = Basin ();
        basins[lp].insert (lp);
        finished[lp] = lp;
    }
    for (unsigned int row {0U}; row < hm.size (); ++row) {
        for (unsigned int col {0U}; col < hm.at (row).size (); ++col) {
            if (hm.at (row).at (col) < MAX_HEIGHT) {
                descend (hm, basins, finished, {row, col});
            }
        }
    }
    return basins;
}

/// Computes the product of the sizes of the three largest basins.
/// \param[in] basins A map from low points to their basins.
/// \return The product of the sizes of the three largest basins.
unsigned int multiplyLargestBasinSizes (std::unordered_map<Location, Basin> const& basins) {
    unsigned int sizes[3] = {0U, 0U, 0U};
    for (auto it = basins.begin (); it != basins.end (); ++it) {
        unsigned int size = (*it).second.size ();
        if (size > sizes[0]) {
            sizes[2] = sizes[1];
            sizes[1] = sizes[0];
            sizes[0] = size;
        }
        else if (size > sizes[1]) {
            sizes[2] = sizes[2];
            sizes[1] = size;
        }
        else if (size > sizes[2]) {
            sizes[2] = size;
        }
    }
    return sizes[0] * sizes[1] * sizes[2];
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    HeightMap hm = getInput ();
    std::unordered_set<Location> lowPoints = findLowPoints (hm);
    std::cout << sumRiskLevels (hm, lowPoints) << "\n";
    std::unordered_map<Location, Basin> basins = computeBasins (hm, lowPoints);
    std::cout << multiplyLargestBasinSizes (basins) << "\n";
    return 0;
}