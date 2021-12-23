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

    void processNewRegions (std::vector<Region> & newRegions) {
        std::vector<Region> revisedNewRegions;
        for (Region const& region : newRegions) {
            if (region.m_on == m_on) {
                revisedNewRegions.push_back (region);
            }
            else {
                // TODO
            }
        }
        newRegions = revisedNewRegions;
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

    bool isValid () const {
        return m_minX <= m_maxX && m_minY <= m_maxY && m_minZ <= m_maxZ;
    }

    Region intersection (Region const& other) const {
        Region result;
        result.m_minX = std::max (m_minX, other.m_minX);
        result.m_maxX = std::min (m_maxX, other.m_maxX);
        result.m_minY = std::max (m_minY, other.m_minY);
        result.m_maxY = std::min (m_maxY, other.m_maxY);
        result.m_minZ = std::max (m_minZ, other.m_minZ);
        result.m_maxZ = std::min (m_maxZ, other.m_maxZ);
        return result;
    }

    std::vector<Region> inMeButNotOther (Region const& other) const {
        if (!intersects (other)) { return {*this}; };

        int topPlane, bottomPlane, leftPlane, rightPlane;
        std::vector<Region> regions;

        if (m_maxY > other.m_maxY) {
            topPlane = other.m_maxY;
            regions.push_back ({m_minX, m_maxX, other.m_maxY + 1, m_maxY, m_minZ, m_maxZ, m_on, {}});
            assert (regions.back ().isValid ());
        }
        else {
            topPlane = m_maxY;
        }

        if (m_minY < other.m_minY) {
            bottomPlane = other.m_minY;
            regions.push_back ({m_minX, m_maxX, m_minY, other.m_minY - 1, m_minZ, m_maxZ, m_on, {}});
            assert (regions.back ().isValid ());
        }
        else {
            bottomPlane = m_minY;
        }

        if (m_minX < other.m_minX) {
            leftPlane = other.m_minX;
            regions.push_back ({m_minX, other.m_minX - 1, bottomPlane, topPlane, m_minZ, m_maxZ, m_on, {}});
            assert (regions.back ().isValid ());
        }
        else {
            leftPlane = m_minX;
        }

        if (m_maxX > other.m_maxX) {
            rightPlane = other.m_maxX;
            regions.push_back ({other.m_maxX + 1, m_maxX, bottomPlane, topPlane, m_minZ, m_maxZ, m_on, {}});
            assert (regions.back ().isValid ());
        }
        else {
            rightPlane = m_maxX;
        }

        if (m_minZ < other.m_minZ) {
            regions.push_back ({leftPlane, rightPlane, bottomPlane, topPlane, m_minZ, other.m_minZ - 1, m_on, {}});
            assert (regions.back ().isValid ());
        }

        if (m_maxZ > other.m_maxZ) {
            regions.push_back ({leftPlane, rightPlane, bottomPlane, topPlane, other.m_maxZ + 1, m_maxZ, m_on, {}});
            assert (regions.back ().isValid ());
        }

        return regions;
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
        assert (step.isValid ());
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

unsigned long long addSizes (std::vector<Region> const& regions) {
    unsigned long long count {0ULL};
    for (Region const& region : regions) {
        count += region.size ();
    }
    return count;
}

std::vector<Region> process (std::vector<Region> const& onRegions, Region const& step) {
    std::vector<Region> revised {onRegions};
    std::vector<Region> toAdd;
    toAdd.push_back (step);
    while (!toAdd.empty ()) {
        std::vector<Region> temp {revised};
        revised.clear ();
        Region next = toAdd.back ();
        toAdd.pop_back ();

        if (next.m_on) {
            // adding regions
            bool didASplit = false;
            for (Region const& existing : temp) {
                if (didASplit || !next.intersects (existing)) {
                    revised.push_back (existing);
                }
                else {
                    Region intersection = next.intersection (existing);
                    std::vector<Region> inNext = next.inMeButNotOther (existing);
                    std::vector<Region> inExisting = existing.inMeButNotOther (next);
                    assert (addSizes (inNext ) + intersection.size () == next.size ());
                    assert (addSizes (inExisting) + intersection.size () == existing.size ());
                    revised.push_back (intersection);
                    for (Region const& piece : inExisting) { revised.push_back (piece); }
                    for (Region const& piece : inNext) { toAdd.push_back (piece); }
                    didASplit = true;
                }
            }
            if (!didASplit) { revised.push_back (next); }
        }
        else {
            // shrinking existing regions
            for (Region const& existing : temp) {
                if (!next.intersects (existing)) {
                    revised.push_back (existing);
                }
                else {
                    Region intersection = next.intersection (existing);
                    std::vector<Region> inNext = next.inMeButNotOther (existing);
                    std::vector<Region> inExisting = existing.inMeButNotOther (next);
                    assert (addSizes (inNext ) + intersection.size () == next.size ());
                    assert (addSizes (inExisting) + intersection.size () == existing.size ());
                    for (Region const& piece : inExisting) { revised.push_back (piece); }                    
                }
            }
        }
    }
    return revised;
}

unsigned long long part2Dos (Steps const& steps) {
    std::vector<Region> onRegions;

    unsigned int x {0U};
    for (Region const& step : steps) {
        onRegions = process (onRegions, step);
        std::cout << " Finished step (" << x << "/" << steps.size () << ")\n";
        ++x;
    }

    return addSizes (onRegions);
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    Steps steps = getInput ();
    std::cout << part1BruteForce (steps) << "\n";
    std::cout << part2Dos (steps) << "\n";
    return 0;
}