/// \file 2024Day15.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-15.


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

const char WALL = '#';
const char FLOOR = '.';
const char ROBOT = '@';
const char BOX = 'O';
const char BOXLEFT = '[';
const char BOXRIGHT = ']';

const char NORTH = '^';
const char EAST = '>';
const char SOUTH = 'v';
const char WEST = '<';

using Grid = std::vector<std::vector<char>>;
using Move = char;

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
  std::list<Move> moves;
  Location robotLoc;
};

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    if (line != "" && line[0] == WALL) {
      std::vector<char> gridLine;
      for (char c : line) {
        assert (c == WALL || c == FLOOR || c == ROBOT || c == BOX);
        if (c == ROBOT) {
          prob.robotLoc.y = prob.grid.size ();
          prob.robotLoc.x = gridLine.size ();
        }
        gridLine.push_back (c);
      }
      prob.grid.push_back (gridLine);
    }
    else if(line != "") {
      for (char c : line) {
        assert (c == NORTH || c == EAST || c == SOUTH || c == WEST);
        prob.moves.push_back (c);
      }
    }
  }
  assert (prob.grid.at (prob.robotLoc.y).at (prob.robotLoc.x) == ROBOT);
  return prob;
}

void
move (Problem& prob)
{
  assert (!prob.moves.empty ());
  assert (prob.grid.at (prob.robotLoc.y).at (prob.robotLoc.x) == ROBOT);
  Location direction;
  switch (prob.moves.front ()) {
    case NORTH: direction = {-1, 0}; break;
    case WEST: direction = {0, -1}; break;
    case SOUTH: direction = {1, 0}; break;
    case EAST: direction = {0, 1}; break;
  }
  prob.moves.pop_front ();
  Location next = prob.robotLoc + direction;
  if (prob.grid.at (next.y).at (next.x) == FLOOR) {
    std::swap (prob.grid[prob.robotLoc.y][prob.robotLoc.x], prob.grid[next.y][next.x]);
    prob.robotLoc = next;
    return;
  }
  else if (prob.grid.at (next.y).at (next.x) == BOX) {
    Location last = next + direction;
    while (prob.grid.at (last.y).at (last.x) == BOX) {
      last = last + direction;
    }
    if (prob.grid.at (last.y).at (last.x) == FLOOR) {
      while (last != prob.robotLoc) {
        Location prev = last - direction;
        std::swap (prob.grid.at (last.y).at (last.x), prob.grid.at (prev.y).at (prev.x));
        last = prev;
      }
      prob.robotLoc = next;
    }
  }
}

void
doAllMoves (Problem& prob)
{
  while (!prob.moves.empty ()) {
    //std::cout << "After move " << prob.moves.front () << "\n";
    move (prob);
    //std::cout << prob.grid << "\n";
  }
}

long
gpsSum (const Problem& prob)
{
  long total = 0;
  for (unsigned int y = 1; y < prob.grid.size () - 1; ++y) {
    for (unsigned int x = 1; x < prob.grid.at (y).size () - 1; ++x) {
      if (prob.grid.at (y).at (x) == BOX) {
        long value = 100 * y + x;
        //std::cout << "(" << y << ", " << x << ") has value " << value << "\n";
        total += value;
      }
    }
  }
  return total;
}

Problem
expand (const Problem& prob)
{
  Problem result;
  for (unsigned int y = 0; y < prob.grid.size (); ++y) {
    std::vector<char> row;
    for (unsigned int x = 0; x < prob.grid.size (); ++x) {
      switch (prob.grid.at (y).at (x)) {
        case WALL: row.push_back (WALL); row.push_back (WALL); break;
        case BOX: row.push_back (BOXLEFT); row.push_back (BOXRIGHT); break;
        case FLOOR: row.push_back (FLOOR); row.push_back (FLOOR); break;
        case ROBOT: row.push_back (ROBOT); row.push_back (FLOOR); break;
      }
    }
    result.grid.push_back (row);
  }
  result.moves = prob.moves;
  result.robotLoc = {prob.robotLoc.y, prob.robotLoc.x * 2};
  assert (result.grid.at (result.robotLoc.y).at (result.robotLoc.x) == ROBOT);
  return result;
}

void
move2 (Problem& prob)
{
  assert (!prob.moves.empty ());
  assert (prob.grid.at (prob.robotLoc.y).at (prob.robotLoc.x) == ROBOT);
  Location direction;
  char move = prob.moves.front ();
  switch (prob.moves.front ()) {
    case NORTH: direction = {-1, 0}; break;
    case WEST: direction = {0, -1}; break;
    case SOUTH: direction = {1, 0}; break;
    case EAST: direction = {0, 1}; break;
  }
  prob.moves.pop_front ();
  Location next = prob.robotLoc + direction;
  // If we can just move into an empty space, that's just as easy as it used to be.
  if (prob.grid.at (next.y).at (next.x) == FLOOR) {
    std::swap (prob.grid[prob.robotLoc.y][prob.robotLoc.x], prob.grid[next.y][next.x]);
    prob.robotLoc = next;
    return;
  }
  // The only other interesting case is trying to push some boxes.
  else if (prob.grid.at (next.y).at (next.x) == BOXLEFT || prob.grid.at (next.y).at (next.x) == BOXRIGHT) {
    // If pushing horizontally, this is pretty much the same as before.
    if (move == WEST || move == EAST) {
      Location last = next + direction;
      while (prob.grid.at (last.y).at (last.x) == BOXLEFT || prob.grid.at (last.y).at (last.x) == BOXRIGHT) {
        last = last + direction;
      }
      if (prob.grid.at (last.y).at (last.x) == FLOOR) {
        while (last != prob.robotLoc) {
          Location prev = last - direction;
          std::swap (prob.grid.at (last.y).at (last.x), prob.grid.at (prev.y).at (prev.x));
          last = prev;
        }
        prob.robotLoc = next;
      }
    }
    // This is the tricky case, potentially pushing boxes vertically.
    else {
      bool allGood = true;
      std::set<Location> toMove;
      std::set<Location> toCheck;
      toCheck.insert (next);
      if (prob.grid.at (next.y).at (next.x) == BOXLEFT) {
        toCheck.insert ({next.y, next.x + 1});
      }
      else {
        toCheck.insert ({next.y, next.x - 1});
      }
      while (allGood && !toCheck.empty ()) {
        Location current = *toCheck.begin ();
        toCheck.erase (current);
        Location nextRow = current + direction;
        if (prob.grid.at (nextRow.y).at (nextRow.x) == FLOOR) {
          toMove.insert (current);
        }
        else if (prob.grid.at (nextRow.y).at (nextRow.x) == BOXLEFT) {
          toMove.insert (current);
          toCheck.insert (nextRow);
          toCheck.insert ({nextRow.y, nextRow.x + 1});
        }
        else if (prob.grid.at (nextRow.y).at (nextRow.x) == BOXRIGHT) {
          toMove.insert (current);
          toCheck.insert (nextRow);
          toCheck.insert ({nextRow.y, nextRow.x - 1});
        }
        else {
          allGood = false;
        }
      }
      if (allGood) {
        // Now we have to move all of the toMove ones, plus the robot one.
        while (!toMove.empty ()) {
          std::set<Location> notYet;
          for (const Location& loc: toMove) {
            Location nextRow = loc + direction;
            if (prob.grid.at (nextRow.y). at (nextRow.x) == FLOOR) {
              std::swap (prob.grid.at (nextRow.y).at (nextRow.x), prob.grid.at (loc.y).at (loc.x));
            }
            else {
              notYet.insert (loc);
            }
          }
          toMove = notYet;
        }
        assert (prob.grid.at (next.y).at (next.x) == FLOOR);
        std::swap (prob.grid.at (next.y).at (next.x), prob.grid.at (prob.robotLoc.y).at (prob.robotLoc.x));
        prob.robotLoc = next;
      }
    }
  }
}

void
doAllMoves2 (Problem& prob)
{
  while (!prob.moves.empty ()) {
    //std::cout << "After move " << prob.moves.front () << "\n";
    move2 (prob);
    //std::cout << prob.grid << "\n";
  }
}

long
gpsSum2 (const Problem& prob)
{
  long total = 0;
  for (unsigned int y = 1; y < prob.grid.size () - 1; ++y) {
    for (unsigned int x = 1; x < prob.grid.at (y).size () - 1; ++x) {
      if (prob.grid.at (y).at (x) == BOXLEFT) {
        long value = 100 * y + x;
        //std::cout << "(" << y << ", " << x << ") has value " << value << "\n";
        total += value;
      }
    }
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  Problem copy = prob;
  doAllMoves (copy);
  std::cout << gpsSum (copy) << "\n";
  Problem expanded = expand (prob);
  //std::cout << expanded.grid << "\n";
  doAllMoves2 (expanded);
  std::cout << gpsSum2 (expanded) << "\n";
  return 0;
}
