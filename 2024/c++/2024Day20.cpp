/// \file 2024Day20.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-20.


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

const char TILE_WALL = '#';
const char TILE_FLOOR = '.';
const char TILE_START = 'S';
const char TILE_END = 'E';

using Grid = std::vector<std::vector<char>>;

struct Location
{
  int y;
  int x;

  auto operator<=>(const Location& other) const = default;
};

Location operator+ (const Location& a, const Location& b)
{
  return {a.y + b.y, a.x + b.x};
}

Location operator- (const Location& a, const Location& b)
{
  return {a.y - b.y, a.x - b.x};
}

int
manhattan (const Location& a, const Location& b)
{
  return std::abs (a.y - b.y) + std::abs (a.x - b.x);
}

std::ostream&
operator<< (std::ostream& out, const Grid& grid)
{
  for (unsigned int y = 0; y < grid.size (); ++y) {
    for (unsigned int x = 0; x < grid.at (y).size (); ++x) {
      out << grid.at (y).at (x);
    }
    out << "\n";
  }
  return out;
}

struct Problem
{
  Grid grid;
  Location start;
  Location end;
};

using Distances = std::vector<std::vector<int>>;

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::vector<char> next;
    for (int x = 0; x < (int)line.size (); ++x) {
      if (line[x] == TILE_START) { prob.start = {(int)prob.grid.size (), x}; next.push_back (TILE_FLOOR); }
      else if (line[x] == TILE_END) { prob.end = {(int)prob.grid.size (), x}; next.push_back (TILE_FLOOR); }
      else { next.push_back (line[x]); }
    }
    prob.grid.push_back (next);
  }
  return prob;
}

Distances
findDistances (const Problem& prob)
{
  Distances dist;
  for (unsigned int y = 0; y < prob.grid.size (); ++y) {
    std::vector<int> row;
    for (unsigned int x = 0; x < prob.grid.at (y).size (); ++x) {
      row.push_back (INT_MAX);
    }
    dist.push_back (row);
  }

  Location current = prob.end;
  dist[current.y][current.x] = 0;
  while (current != prob.start) {
    Location next = {0, 0};
    std::vector<Location> possible = {current + Location {1, 0}, current + Location {-1, 0}, current + Location {0, 1}, current + Location {0, -1}};
    for (const Location& loc : possible) {
      if (prob.grid.at (loc.y).at (loc.x) == TILE_FLOOR && dist.at (loc.y).at (loc.x) == INT_MAX) {
        assert ((next == Location {0, 0}));
        next = loc;
      }
    }
    assert (!(next == Location {0, 0}));
    dist.at (next.y).at (next.x) = dist.at (current.y).at (current.x) + 1;
    current = next;
  }
  return dist;
}

struct Cheat
{
  Location loc;
  Location endLoc;
  int saved;
};

using Cheats = std::vector<Cheat>;

Cheats
findCheats (const Problem& prob, const Distances& dist) 
{
  Cheats cheats;
  for (int y = 1; y < (int)prob.grid.size () - 1; ++y) {
    for (int x = 1; x < (int)prob.grid.at (y).size () - 1; ++x) {
      if (prob.grid.at (y).at (x) == TILE_WALL) {
        if (prob.grid.at (y - 1).at (x) == TILE_FLOOR && prob.grid.at (y + 1).at (x) == TILE_FLOOR) {
          int distDiff = dist.at (y + 1).at (x) - dist.at (y - 1).at (x);
          Location from = distDiff > 0 ? Location {y + 1, x} : Location {y - 1, x};
          Location to = distDiff > 0 ? Location {y - 1, x} : Location {y + 1, x};
          cheats.push_back ({from, to, std::abs (distDiff) - 2});
        }
        if (prob.grid.at (y).at (x - 1) == TILE_FLOOR && prob.grid.at (y).at (x + 1) == TILE_FLOOR) {
          int distDiff = dist.at (y).at (x + 1) - dist.at (y).at (x - 1);
          Location from = distDiff > 0 ? Location {y, x + 1} : Location {y, x - 1};
          Location to = distDiff > 0 ? Location {y, x - 1} : Location {y, x + 1};
          cheats.push_back ({from, to, std::abs (distDiff) - 2});
        }
      }
    }
  }
  return cheats;
}

int
countCheatsAtLeast (const Cheats& cheats, int minSaved)
{
  int count = 0;
  for (const Cheat& cheat : cheats) {
    if (cheat.saved >= minSaved) {
      ++count;
      //std::cout << "Found a cheat from (" << cheat.loc.y << ", " << cheat.loc.x << ") to (" << cheat.endLoc.y << ", " << cheat.endLoc.x << ") that saves " << cheat.saved << "\n";
    }
  }
  return count;
}

Cheats findLongerCheats (const Problem& prob, const Distances& dist, int maxLength)
{
  Cheats cheats;
  for (int y = 1; y < (int)prob.grid.size () - 1; ++y) {
    for (int x = 1; x < (int) prob.grid.at (y).size () - 1; ++x) {
      if (prob.grid.at (y).at (x) == TILE_FLOOR) {
        for (int y2 = std::max (1, y - maxLength); y2 < std::min ((int)prob.grid.size (), y + maxLength + 1); ++y2) {
          for (int x2 = std::max (1, x - maxLength); x2 < std::min ((int)prob.grid.at (y).size (), x + maxLength + 1); ++x2) {
            if (prob.grid.at (y2).at (x2) == TILE_FLOOR) {
              int skipDist = manhattan ({y, x}, {y2, x2});
              if (skipDist <= maxLength) {
                int distance = dist.at (y2).at (x2) - dist.at (y).at (x);
                int saved = distance - skipDist;
                if (saved > 0) {
                  cheats.push_back ({{y2, x2}, {y, x}, saved});
                }
              }
            }
          }
        }
      }
    }
  }
  return cheats;
}


/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  Distances dist = findDistances (prob);
  Cheats cheats = findCheats (prob, dist);
  std::cout << countCheatsAtLeast (cheats, 100) << "\n";
  Cheats cheats2 = findLongerCheats (prob, dist, 20);
  std::cout << countCheatsAtLeast (cheats2, 100) << "\n";
  return 0;
}
