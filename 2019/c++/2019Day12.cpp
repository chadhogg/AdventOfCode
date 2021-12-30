/// \file 2019Day12.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/12.

#include <iostream>
#include <cassert>
#include <cstdio>
#include <numeric>

#include "utilities.hpp"

constexpr unsigned int NUM_MOONS = 4;
constexpr unsigned int STEPS = 1000;

struct Vec3D {
    int x, y, z;
};

struct Moon {
    Vec3D position;
    Vec3D velocity;
};

bool operator== (Moon const& a, Moon const& b) {
    return a.position.x == b.position.x && a.position.y == b.position.y && a.position.z == b.position.z &&
        a.velocity.x == b.velocity.x && a.velocity.y == b.velocity.y && a.velocity.z == b.velocity.z;
}

using System = std::array<Moon, NUM_MOONS>;

System getInput () {
    System system;
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        int count = scanf ("<x=%d, y=%d, z=%d>\n", &system[i].position.x, &system[i].position.y, &system[i].position.z);
        assert (count == 3);
        system[i].velocity.x = 0;
        system[i].velocity.y = 0;
        system[i].velocity.z = 0;
    }
    return system;
}

void printSystem (System const& system) {
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        printf ("pos=<x=%4d, y=%4d, z=%4d>, vel=<x=%4d, y=%4d, z=%4d>\n", system[i].position.x, system[i].position.y, system[i].position.z, system[i].velocity.x, system[i].velocity.y, system[i].velocity.z);
    }
}

void doTimeStep (System & system) {
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        for (unsigned int j {i + 1}; j < NUM_MOONS; ++j) {
            if (system[i].position.x < system[j].position.x) {
                ++system[i].velocity.x;
                --system[j].velocity.x;
            }
            else if(system[i].position.x > system[j].position.x) {
                --system[i].velocity.x;
                ++system[j].velocity.x;
            }
            if (system[i].position.y < system[j].position.y) {
                ++system[i].velocity.y;
                --system[j].velocity.y;
            }
            else if(system[i].position.y > system[j].position.y) {
                --system[i].velocity.y;
                ++system[j].velocity.y;
            }
            if (system[i].position.z < system[j].position.z) {
                ++system[i].velocity.z;
                --system[j].velocity.z;
            }
            else if(system[i].position.z > system[j].position.z) {
                --system[i].velocity.z;
                ++system[j].velocity.z;
            }
        }
    }
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        system[i].position.x += system[i].velocity.x;
        system[i].position.y += system[i].velocity.y;
        system[i].position.z += system[i].velocity.z;
    }
}

System afterSeveralTimeSteps (System const& orig, unsigned int steps) {
    System result {orig};
    for (unsigned int x {0U}; x < steps; ++x) {
        doTimeStep (result);
    }
    return result;
}

unsigned int energyLevel (System const& system) {
    unsigned int total {0U};
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        unsigned int potential = std::abs (system[i].position.x) + std::abs (system[i].position.y) + std::abs (system[i].position.z);
        unsigned int kinetic = std::abs (system[i].velocity.x) + std::abs (system[i].velocity.y) + std::abs (system[i].velocity.z);
        total += potential * kinetic;
    }
    return total;
}

using Positions1D = std::array<int, NUM_MOONS>;
using Velocities1D = std::array<int, NUM_MOONS>;

void doStep1D (Positions1D & positions, Velocities1D & velocities) {
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        for (unsigned int j {i + 1}; j < NUM_MOONS; ++j) {
            if (positions[i] < positions[j]) {
                ++velocities[i];
                --velocities[j];
            }
            else if(positions[i] > positions[j]) {
                --velocities[i];
                ++velocities[j];
            }
        }
    }
    for (unsigned int i {0U}; i < NUM_MOONS; ++i) {
        positions[i] += velocities[i];
    }
}

unsigned long long findPeriod1D (Positions1D positions, Velocities1D velocities) {
    Positions1D originalPos = positions;
    Velocities1D originalVel = velocities;
    unsigned long long count {0U};
    while (true) {
        doStep1D (positions, velocities);
        ++count;
        if (positions == originalPos && velocities == originalVel) { break; }
    }
    return count;
}


unsigned long long findPeriod (System const& system) {
    unsigned long long xPeriod = findPeriod1D ({system[0].position.x, system[1].position.x, system[2].position.x, system[3].position.x}, {system[0].velocity.x, system[1].velocity.x, system[2].velocity.x, system[3].velocity.x});
    unsigned long long yPeriod = findPeriod1D ({system[0].position.y, system[1].position.y, system[2].position.y, system[3].position.y}, {system[0].velocity.y, system[1].velocity.y, system[2].velocity.y, system[3].velocity.y});
    unsigned long long zPeriod = findPeriod1D ({system[0].position.z, system[1].position.z, system[2].position.z, system[3].position.z}, {system[0].velocity.z, system[1].velocity.z, system[2].velocity.z, system[3].velocity.z});
    return std::lcm (std::lcm (xPeriod, yPeriod), zPeriod);
}


int main () {
    System system = getInput ();
    System finished = afterSeveralTimeSteps (system, STEPS);
    std::cout << energyLevel (finished) << "\n";
    std::cout << findPeriod (system) << "\n";
    return 0;
}
