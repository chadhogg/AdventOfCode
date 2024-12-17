/// \file 2024Day16.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-16.


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

const char DIR_NORTH = '^';
const char DIR_EAST = '>';
const char DIR_SOUTH = 'v';
const char DIR_WEST = '<';

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

struct State
{
  Location where;
  char direction;
  auto operator<=>(const State& other) const = default;
};

struct StateWithCost
{
  State state;
  long cost;
  long estimate;
  bool operator< (const StateWithCost& other) const
  {
    return cost + estimate < other.cost + other.estimate || (cost + estimate == other.cost + other.estimate && state < other.state);
  }
  bool operator== (const StateWithCost& other) const
  {
    return cost + estimate == other.cost + other.estimate && state == other.state;
  }
  bool operator> (const StateWithCost& other) const
  {
    return !(*this < other || *this == other);
  }
};

long
manhattan (const Location& a, const Location& b)
{
  return std::abs (a.y - b.y) + std::abs (a.x - b.x);
}

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::vector<char> gridLine;
    for (char c : line) {
      assert (c == TILE_WALL || c == TILE_FLOOR || c == TILE_START || c == TILE_END);
      if (c == TILE_START) {
        prob.start.y = prob.grid.size ();
        prob.start.x = gridLine.size ();
        gridLine.push_back (TILE_FLOOR);
      }
      else if (c == TILE_END) {
        prob.end.y = prob.grid.size ();
        prob.end.x = gridLine.size ();
        gridLine.push_back (TILE_FLOOR);

      }
      else {
        gridLine.push_back (c);
      }
    }
    prob.grid.push_back (gridLine);
  }
  return prob;
}

const long COST_MOVE = 1;
const long COST_TURN = 1000;

StateWithCost
searchAStar (const Problem& prob)
{
  std::priority_queue<StateWithCost, std::vector<StateWithCost>, std::greater<StateWithCost>> frontier;
  frontier.push ({{prob.start, DIR_EAST}, 0, manhattan (prob.start, prob.end)});
  std::set<State> visited;
  while (true) {
    StateWithCost current = frontier.top ();
    frontier.pop ();
    if (current.state.where == prob.end) {
      return current;
    }
    visited.insert (current.state);
    std::vector<StateWithCost> successors;
    if (current.state.direction == DIR_NORTH) {
      successors.push_back ({{{current.state.where.y - 1, current.state.where.x}, current.state.direction}, current.cost + COST_MOVE, 0});
      successors.push_back ({{current.state.where, DIR_EAST}, current.cost + COST_TURN, 0});
      successors.push_back ({{current.state.where, DIR_WEST}, current.cost + COST_TURN, 0});
    }
    else if (current.state.direction == DIR_EAST) {
      successors.push_back ({{{current.state.where.y, current.state.where.x + 1}, current.state.direction}, current.cost + COST_MOVE, 0});
      successors.push_back ({{current.state.where, DIR_NORTH}, current.cost + COST_TURN, 0});
      successors.push_back ({{current.state.where, DIR_SOUTH}, current.cost + COST_TURN, 0});
    }
    else if (current.state.direction == DIR_SOUTH) {
      successors.push_back ({{{current.state.where.y + 1, current.state.where.x}, current.state.direction}, current.cost + COST_MOVE, 0});
      successors.push_back ({{current.state.where, DIR_EAST}, current.cost + COST_TURN, 0});
      successors.push_back ({{current.state.where, DIR_WEST}, current.cost + COST_TURN, 0});
    }
    else if (current.state.direction == DIR_WEST) {
      successors.push_back ({{{current.state.where.y, current.state.where.x - 1}, current.state.direction}, current.cost + COST_MOVE, 0});
      successors.push_back ({{current.state.where, DIR_NORTH}, current.cost + COST_TURN, 0});
      successors.push_back ({{current.state.where, DIR_SOUTH}, current.cost + COST_TURN, 0});
    }
    for (StateWithCost& next : successors) {
      if (prob.grid.at (next.state.where.y).at (next.state.where.x) == TILE_FLOOR) {
        if (visited.count (next.state) == 0) {
          next.estimate = manhattan (next.state.where, prob.end);
          frontier.push (next);
        }
      }
    }
  }
  throw ("No solution found.");
}

void
draw (const Problem& prob, const std::set<Location>& marked)
{
  for (unsigned int y = 0; y < prob.grid.size (); ++y) {
    for (unsigned int x = 0; x < prob.grid.at (y).size (); ++x) {
      if (marked.count ({(int)y, (int)x}) == 1) {
        std::cout << 'O';
      }
      else {
        std::cout << prob.grid.at (y).at (x);
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}


struct StateWithCostAndPrev
{
  State state;
  long cost;
  long estimate;
  std::set<State> prev;
  bool operator< (const StateWithCostAndPrev& other) const
  {
    return cost + estimate < other.cost + other.estimate || (cost + estimate == other.cost + other.estimate && state < other.state) || (cost + estimate == other.cost + other.estimate && state == other.state && prev < other.prev);
  }
  bool operator== (const StateWithCostAndPrev& other) const
  {
    return cost + estimate == other.cost + other.estimate && state == other.state && prev == other.prev;
  }
  bool operator> (const StateWithCostAndPrev& other) const
  {
    return !(*this < other || *this == other);
  }
};

void
addValidSuccessors (std::priority_queue<StateWithCostAndPrev, std::vector<StateWithCostAndPrev>, std::greater<StateWithCostAndPrev>>& frontier, const StateWithCostAndPrev& current, const Problem& prob)
{
  std::vector<StateWithCostAndPrev> successors;
  std::set<State> justMe;
  justMe.insert (current.state);
  if (current.state.direction == DIR_NORTH) {
    successors.push_back ({{{current.state.where.y - 1, current.state.where.x}, current.state.direction}, current.cost + COST_MOVE, 0, justMe});
    successors.push_back ({{current.state.where, DIR_EAST}, current.cost + COST_TURN, 0, justMe});
    successors.push_back ({{current.state.where, DIR_WEST}, current.cost + COST_TURN, 0, justMe});
  }
  else if (current.state.direction == DIR_EAST) {
    successors.push_back ({{{current.state.where.y, current.state.where.x + 1}, current.state.direction}, current.cost + COST_MOVE, 0, justMe});
    successors.push_back ({{current.state.where, DIR_NORTH}, current.cost + COST_TURN, 0, justMe});
    successors.push_back ({{current.state.where, DIR_SOUTH}, current.cost + COST_TURN, 0, justMe});
  }
  else if (current.state.direction == DIR_SOUTH) {
    successors.push_back ({{{current.state.where.y + 1, current.state.where.x}, current.state.direction}, current.cost + COST_MOVE, 0, justMe});
    successors.push_back ({{current.state.where, DIR_EAST}, current.cost + COST_TURN, 0, justMe});
    successors.push_back ({{current.state.where, DIR_WEST}, current.cost + COST_TURN, 0, justMe});
  }
  else if (current.state.direction == DIR_WEST) {
    successors.push_back ({{{current.state.where.y, current.state.where.x - 1}, current.state.direction}, current.cost + COST_MOVE, 0, justMe});
    successors.push_back ({{current.state.where, DIR_NORTH}, current.cost + COST_TURN, 0, justMe});
    successors.push_back ({{current.state.where, DIR_SOUTH}, current.cost + COST_TURN, 0, justMe});
  }
  for (StateWithCostAndPrev& next : successors) {
    if (prob.grid.at (next.state.where.y).at (next.state.where.x) == TILE_FLOOR) {
      next.prev.insert (current.state);
      next.estimate = manhattan (next.state.where, prob.end);
      frontier.push (next);
    }
  }
}

std::set<Location>
findLocationsOnBestPath (const Problem& prob, const std::map<State, long>& visitedStateToCost, const std::map<State, std::set<State>>& visitedStateToEarlier, long minSolutionCost)
{
  std::set<Location> onABestPath;
  std::set<State> toDo;
  std::set<State> done;
  for (auto it = visitedStateToEarlier.begin (); it != visitedStateToEarlier.end (); ++it) {
    if (it->first.where == prob.end && visitedStateToCost.at (it->first) == minSolutionCost) {
      toDo.insert (it->first);
    }
  }
  
  while (!toDo.empty ()) {
    State current = *toDo.begin ();
    toDo.erase (current);
    if (done.count (current) == 0) {
      for (const State& prev : visitedStateToEarlier.at (current)) {
        toDo.insert (prev);
      }
      onABestPath.insert (current.where);
    }
    done.insert (current);
  }
  return onABestPath;
}

long
findAllShortestPaths3 (const Problem& prob)
{
  // The cost of the cheapest solution found so far (should be the only one, anyway).
  long minSolutionCost = INT32_MAX;
  // The states I have found a way to but not processed.
  std::priority_queue<StateWithCostAndPrev, std::vector<StateWithCostAndPrev>, std::greater<StateWithCostAndPrev>> frontier;
  // The states I have processed, with the cost to reach them.
  std::map<State, long> visitedStateToCost;
  // The states I have processed, with the states from which I can reach them at minimum cost.
  std::map<State, std::set<State>> visitedStateToEarlier;

  frontier.push ({{prob.start, DIR_EAST}, 0, manhattan (prob.start, prob.end), {}});

  while (!frontier.empty ()) {
    StateWithCostAndPrev current = frontier.top ();
    frontier.pop ();
    assert (visitedStateToCost.count (current.state) == 0 || visitedStateToCost.at (current.state) <= current.cost);

    // If this is a solution, potentially update the cost to cheapest solution.
    if (current.state.where == prob.end) {
      minSolutionCost = std::min (minSolutionCost, current.cost);
    }

    // If this is the first time we reached this state, add it to the data structures.
    if (visitedStateToCost.count (current.state) == 0) {
      visitedStateToCost[current.state] = current.cost;
      visitedStateToEarlier[current.state] = current.prev;
    }
    // If this is an equally good way to reach this state, add my predecessors to set.
    else if (visitedStateToCost[current.state] == current.cost) {
      visitedStateToEarlier[current.state].insert (current.prev.begin (), current.prev.end ());
      continue;
    }
    // If this is a worse way to reach this state, ignore it.
    else {
      continue;
    }

    // If this has reached the goal, no need to keep moving from here.
    if (current.state.where == prob.end) {
      continue;
    }

    // Insert moves we could make from current into frontier, with correct cost / estimate / predecessors.
    addValidSuccessors (frontier, current, prob);
  }

  // Walk backward from optimal goal states, finding all locations on any path to them.
  std::set<Location> onABestPath = findLocationsOnBestPath (prob, visitedStateToCost, visitedStateToEarlier, minSolutionCost);
  //draw (prob, onABestPath);
  return onABestPath.size ();
}


/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << searchAStar (prob).cost << "\n";
  std::cout << findAllShortestPaths3 (prob) << "\n";
  return 0;
}
