/// \file 2021Day22.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/22.



#include <iostream>
#include <string>
#include <cstdio>
#include <cassert>
#include <vector>
#include <climits>

#include "utilities.hpp"

struct Region {
    int m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
    bool m_on;
    std::vector<Region> m_holes;

    unsigned long long sizeIgnoringHoles () const {
        return (unsigned long long)(m_maxX - m_minX + 1) * (unsigned long long)(m_maxY - m_minY + 1) * (unsigned long long)(m_maxZ - m_minZ + 1);
    }

    unsigned long long size () const {
        unsigned long long count = sizeIgnoringHoles ();
        for (Region const& hole : m_holes) {
            count -= hole.size ();
        }
        return count;
    }

    bool intersects (Region const& other) const {
        if (m_minX > other.m_maxX) { return false; }
        if (m_minY > other.m_maxY) { return false; }
        if (m_minZ > other.m_maxZ) { return false; }
        if (m_maxX < other.m_minX) { return false; }
        if (m_maxY < other.m_minY) { return false; }
        if (m_maxZ < other.m_minZ) { return false; }
        return true;
    }
};


using Steps = std::vector<Region>;

Steps getInput () {
    Steps steps;
    std::string line;
    int count;
    while (std::getline (std::cin, line)) {
        Region step {};
        if (line.substr (0, 2) == "on") {
            step.m_on = true;
            count = sscanf (line.c_str (), "on x=%d..%d,y=%d..%d,z=%d..%d", &step.m_minX, &step.m_maxX, &step.m_minY, &step.m_maxY, &step.m_minZ, &step.m_maxZ);
        }
        else {
            assert (line.substr (0, 3) == "off");
            step.m_on = false;
            count = sscanf (line.c_str (), "off x=%d..%d,y=%d..%d,z=%d..%d", &step.m_minX, &step.m_maxX, &step.m_minY, &step.m_maxY, &step.m_minZ, &step.m_maxZ);
            assert (count == 6);
        }
        assert (step.m_minX <= step.m_maxX);
        assert (step.m_minY <= step.m_maxY);
        assert (step.m_minZ <= step.m_maxZ);
        steps.push_back (step);
    }
    return steps;
}


unsigned long part1BruteForce (Steps const& steps) {
    constexpr int OFFSET = 50;
    constexpr int INTERESTING_SIZE = 101;
    bool cubes[INTERESTING_SIZE][INTERESTING_SIZE][INTERESTING_SIZE];
    unsigned long count {0UL};
    for (Region const& step : steps) {
        Region adjusted;
        adjusted.m_minX = std::max (0, step.m_minX + OFFSET);
        adjusted.m_maxX = std::min (INTERESTING_SIZE - 1, step.m_maxX + OFFSET);
        adjusted.m_minY = std::max (0, step.m_minY + OFFSET);
        adjusted.m_maxY = std::min (INTERESTING_SIZE - 1, step.m_maxY + OFFSET);
        adjusted.m_minZ = std::max (0, step.m_minZ + OFFSET);
        adjusted.m_maxZ = std::min (INTERESTING_SIZE - 1, step.m_maxZ + OFFSET);
        for (int x {adjusted.m_minX}; x <= adjusted.m_maxX; ++x) {
            for (int y {adjusted.m_minY}; y <= adjusted.m_maxY; ++y) {
                for (int z {adjusted.m_minZ}; z <= adjusted.m_maxZ; ++z) {
                    if (step.m_on && !cubes[x][y][z]) {
                        ++count;
                        cubes[x][y][z] = true;
                    }
                    else if (!step.m_on && cubes[x][y][z]) {
                        --count;
                        cubes[x][y][z] = false;
                    }
                }
            }
        }
    }
    return count;
}

Region boundingBox (Steps const& steps) {
    Region box {INT_MAX, INT_MIN, INT_MAX, INT_MIN, INT_MAX, INT_MIN, false, {}};
    for (Region const& step : steps) {
        box.m_minX = std::min (box.m_minX, step.m_minX);
        box.m_maxX = std::max (box.m_maxX, step.m_maxX);
        box.m_minY = std::min (box.m_minY, step.m_minY);
        box.m_maxY = std::max (box.m_maxY, step.m_maxY);
        box.m_minZ = std::min (box.m_minZ, step.m_minZ);
        box.m_maxZ = std::max (box.m_maxZ, step.m_maxZ);
    }
    return box;
}


/// \brief Runs the program.
/// \return Always 0.
int main () {
    Steps steps = getInput ();
    std::cout << part1BruteForce (steps) << "\n";
    return 0;
}