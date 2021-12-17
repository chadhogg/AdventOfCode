/// \file 2021Day17.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/17.

#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>

#include "utilities.hpp"

/// The x-coordinate where the probe starts.
constexpr int INITIAL_X = 0;
/// The y-coordinate where the probe starts.
constexpr int INITIAL_Y = 0;
/// The amount the x-velocity changes due to drag.
constexpr int DRAG_AMOUNT = 1;
/// The amount the y-velocity changes due to gravity.
constexpr int GRAVITY_AMOUNT = -1;

/// A rectangular region in 2-d space.
struct Region {
    /// The x-coordinate of the left side. 
    int minX;
    /// The x-coordinate of the right side.
    int maxX;
    /// The y-coordinate of the bottom side.
    int minY;
    /// The y-coordinate of the top side.
    int maxY;

    /// \brief Tests whether or not a point lies inside the region.
    /// \param[in] x The x-coordinate.
    /// \param[in] y The y-coordinate.
    /// \return True if that point is inside (or on the boundary of) the region.
    inline bool inside (int x, int y) const {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }

    /// \brief Tests whether or not a point lies past the region.
    /// \param[in] x The x-coordinate.
    /// \param[in] y The y-coordinate.
    /// \return True if that point is past the region.
    /// \note This depends on the fact that after the parabola has reached its peak y cannot increase, and x can never decrease.
    inline bool past (int x, int y) const {
        return x > maxX || y < minY;
    }
};

/// \brief Reads the target region from standard input.
/// \return The target region.
Region getInput () {
    int minX, maxX, minY, maxY;
    scanf ("target area: x=%d..%d, y=%d..%d", &minX, &maxX, &minY, &maxY);
    assert (minX < maxX);
    assert (minY < maxY);
    Region area {minX, maxX, minY, maxY};
    return area;
}

/// \brief Tests whether or not a trajectory hits the target.
/// \param[in] target The target.
/// \param[in] xVelocity The initial velocity in the x direction.
/// \param[in] yVelocity The initial velocity in the y direction.
/// \return A pair of whether or not the region was hit and the highest y-coordinate reached.
std::pair<bool, int> hitsTarget (Region const& target, int xVelocity, int yVelocity) {
    int xPosition = INITIAL_X;
    int yPosition = INITIAL_Y;
    int highestY = yPosition;
    while (!target.past (xPosition, yPosition)) {
        if (target.inside (xPosition, yPosition)) { return {true, highestY}; }
        xPosition += xVelocity;
        yPosition += yVelocity;
        if (yPosition > highestY) {
            highestY = yPosition;
        }
        if (xVelocity > 0) { xVelocity -= DRAG_AMOUNT; }
        else if (xVelocity < 0) { xVelocity += DRAG_AMOUNT; }
        yVelocity += GRAVITY_AMOUNT;
    }
    return {false, highestY};
}

/// \brief Attempts some trajectories to see which ones hit the target.
/// \param[in] target The target.
/// \return The number of trajectories that hit it.
/// \note If necessary, the loop bounds should be adjusted.
int trySome (Region const& target) {
    int count = 0;
    for (int y = -1000; y < 1000; ++y) {
        for (int x = 0; x < 1000; ++x) {
            std::pair<bool, int> result = hitsTarget (target, x, y);
            if (result.first) {
                std::cout << "Got a hit with x = " << x << ", y = " << y << " up to " << result.second << "\n";
                ++count;
            }
        }
    }
    return count;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Region target = getInput ();
    std::cout << trySome (target) << "\n";
    return 0;
}