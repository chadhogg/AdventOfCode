/// \file 2024Day21.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-21.
/// It's mostly a mess of ideas that either weren't correct or weren't efficient enough.
/// I plan to never look at or think about this again.


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

using Code = std::string;
using Problem = std::vector<Code>;

Problem
readInput ()
{
  Problem prob;
  std::string word;
  std::cin >> word;
  while (std::cin) {
    prob.push_back (word);
    std::cin >> word;
  }
  return prob;
}

using RobotPosition = char;
const RobotPosition POS_NINE = '9';
const RobotPosition POS_EIGHT = '8';
const RobotPosition POS_SEVEN = '7';
const RobotPosition POS_SIX = '6';
const RobotPosition POS_FIVE = '5';
const RobotPosition POS_FOUR = '4';
const RobotPosition POS_THREE = '3';
const RobotPosition POS_TWO = '2';
const RobotPosition POS_ONE = '1';
const RobotPosition POS_ZERO = '0';
const RobotPosition POS_A = 'A';
const RobotPosition POS_NORTH = '^';
const RobotPosition POS_EAST = '>';
const RobotPosition POS_SOUTH = 'v';
const RobotPosition POS_WEST = '<';

const std::set<RobotPosition> validKeypadPositions = {POS_NINE, POS_EIGHT, POS_SEVEN, POS_SIX, POS_FIVE, POS_FOUR, POS_THREE, POS_TWO, POS_ONE, POS_ZERO, POS_A};
const std::set<RobotPosition> validDirpadPositions = {POS_A, POS_NORTH, POS_EAST, POS_SOUTH, POS_WEST};

std::set<Code>
getOptimalKeypadMoves (RobotPosition start, RobotPosition end)
{
  switch (start) {
    case POS_NINE: switch (end) {
      case POS_NINE: return {{}};
      case POS_EIGHT: return {{POS_WEST}};
      case POS_SEVEN: return {{POS_WEST, POS_WEST}};
      case POS_SIX: return {{POS_SOUTH}};
      case POS_FIVE: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}};
      case POS_FOUR: return {{POS_SOUTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_SOUTH}};
      case POS_THREE: return {{POS_SOUTH, POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH, POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH, POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH, POS_SOUTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_SOUTH, POS_SOUTH}};
      case POS_ZERO: return {{POS_SOUTH, POS_SOUTH, POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_EIGHT: switch (end) {
      case POS_NINE: return {{POS_EAST}};
      case POS_EIGHT: return {{}};
      case POS_SEVEN: return {{POS_WEST}};
      case POS_SIX: return {{POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH}};
      case POS_FIVE: return {{POS_SOUTH}};
      case POS_FOUR: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}};
      case POS_THREE: return {{POS_SOUTH, POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH, POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH, POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH, POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH, POS_SOUTH}};
      case POS_ZERO: return {{POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_SOUTH, POS_SOUTH, POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_SEVEN: switch (end) {
      case POS_NINE: return {{POS_EAST, POS_EAST}};
      case POS_EIGHT: return {{POS_EAST}};
      case POS_SEVEN: return {{}};
      case POS_SIX: return {{POS_EAST, POS_EAST, POS_SOUTH}, {POS_SOUTH, POS_EAST, POS_EAST}};
      case POS_FIVE: return {{POS_EAST, POS_SOUTH}, {POS_SOUTH, POS_EAST}};
      case POS_FOUR: return {{POS_SOUTH}};
      case POS_THREE: return {{POS_SOUTH, POS_SOUTH, POS_EAST, POS_EAST}, {POS_EAST, POS_EAST, POS_SOUTH, POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH, POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH, POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH, POS_SOUTH}};
      case POS_ZERO: return {{POS_EAST, POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_EAST, POS_EAST, POS_SOUTH, POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_SIX: switch (end) {
      case POS_NINE: return {{POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_NORTH}};
      case POS_SIX: return {{}};
      case POS_FIVE: return {{POS_WEST}};
      case POS_FOUR: return {{POS_WEST, POS_WEST}};
      case POS_THREE: return {{POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_SOUTH}};
      case POS_ZERO: return {{POS_SOUTH, POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_FIVE: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}};
      case POS_SIX: return {{POS_EAST}};
      case POS_FIVE: return {{}};
      case POS_FOUR: return {{POS_WEST}};
      case POS_THREE: return {{POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}};
      case POS_ZERO: return {{POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_SOUTH, POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_FOUR: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_EAST, POS_EAST}, {POS_EAST, POS_EAST, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH}};
      case POS_SIX: return {{POS_EAST, POS_EAST}};
      case POS_FIVE: return {{POS_EAST}};
      case POS_FOUR: return {{}};
      case POS_THREE: return {{POS_SOUTH, POS_EAST, POS_EAST}, {POS_EAST, POS_EAST, POS_SOUTH}};
      case POS_TWO: return {{POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH}};
      case POS_ONE: return {{POS_SOUTH}};
      case POS_ZERO: return {{POS_EAST, POS_SOUTH, POS_SOUTH}};
      case POS_A: return {{POS_EAST, POS_EAST, POS_SOUTH, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_THREE: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_NORTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_NORTH, POS_NORTH}};
      case POS_SIX: return {{POS_NORTH}};
      case POS_FIVE: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}};
      case POS_FOUR: return {{POS_NORTH, POS_WEST, POS_WEST}, {POS_WEST, POS_WEST, POS_NORTH}};
      case POS_THREE: return {{}};
      case POS_TWO: return {{POS_WEST}};
      case POS_ONE: return {{POS_WEST, POS_WEST}};
      case POS_ZERO: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}};
      case POS_A: return {{POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_TWO: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH, POS_NORTH}};
      case POS_SIX: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_FIVE: return {{POS_NORTH}};
      case POS_FOUR: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}};
      case POS_THREE: return {{POS_EAST}};
      case POS_TWO: return {{}};
      case POS_ONE: return {{POS_WEST}};
      case POS_ZERO: return {{POS_SOUTH}};
      case POS_A: return {{POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_ONE: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_NORTH, POS_EAST, POS_EAST}, {POS_EAST, POS_EAST, POS_NORTH, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_NORTH}};
      case POS_SIX: return {{POS_NORTH, POS_EAST, POS_EAST}, {POS_EAST, POS_EAST, POS_NORTH}};
      case POS_FIVE: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_FOUR: return {{POS_NORTH}};
      case POS_THREE: return {{POS_EAST, POS_EAST}};
      case POS_TWO: return {{POS_EAST}};
      case POS_ONE: return {{}};
      case POS_ZERO: return {{POS_EAST, POS_SOUTH}};
      case POS_A: return {{POS_EAST, POS_EAST, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_ZERO: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_NORTH, POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH, POS_NORTH, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_NORTH, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_NORTH, POS_NORTH, POS_WEST}};
      case POS_SIX: return {{POS_NORTH, POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH, POS_NORTH}};
      case POS_FIVE: return {{POS_NORTH, POS_NORTH}, {POS_NORTH, POS_NORTH}};
      case POS_FOUR: return {{POS_NORTH, POS_NORTH, POS_WEST}};
      case POS_THREE: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_TWO: return {{POS_NORTH}};
      case POS_ONE: return {{POS_NORTH, POS_WEST}};
      case POS_ZERO: return {{}};
      case POS_A: return {{POS_EAST}};
      default: throw "Impossible";
    }
    case POS_A: switch (end) {
      case POS_NINE: return {{POS_NORTH, POS_NORTH, POS_NORTH}};
      case POS_EIGHT: return {{POS_NORTH, POS_NORTH, POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH, POS_NORTH, POS_NORTH}};
      case POS_SEVEN: return {{POS_NORTH, POS_NORTH, POS_NORTH, POS_WEST, POS_WEST}};
      case POS_SIX: return {{POS_NORTH, POS_NORTH}};
      case POS_FIVE: return {{POS_NORTH, POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH, POS_NORTH}};
      case POS_FOUR: return {{POS_NORTH, POS_NORTH, POS_WEST, POS_WEST}};
      case POS_THREE: return {{POS_NORTH}};
      case POS_TWO: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}};
      case POS_ONE: return {{POS_NORTH, POS_WEST, POS_WEST}};
      case POS_ZERO: return {{POS_WEST}};
      case POS_A: return {{}};
      default: throw "Impossible";
    }
    default: throw "Also Impossible";
  }
}

void
sanityCheckKeypadMoves ()
{
  for (RobotPosition pos : validKeypadPositions) {
    for (RobotPosition pos2 : validKeypadPositions) {
      std::set<Code> solutions = getOptimalKeypadMoves (pos, pos2);
      for (const Code& code : solutions) {
        RobotPosition current = pos;
        for (RobotPosition move : code) {
          if (move == POS_NORTH) {
            switch (current) {
              case POS_NINE: throw "Bad";
              case POS_EIGHT: throw "Bad";
              case POS_SEVEN: throw "Bad";
              case POS_SIX: current = POS_NINE; break;
              case POS_FIVE: current = POS_EIGHT; break;
              case POS_FOUR: current = POS_SEVEN; break;
              case POS_THREE: current = POS_SIX; break;
              case POS_TWO: current = POS_FIVE; break;
              case POS_ONE: current = POS_FOUR; break;
              case POS_A: current = POS_THREE; break;
              case POS_ZERO: current = POS_TWO; break;
              default: throw "Impossible";
            }
          }
          else if (move == POS_EAST) {
            switch (current) {
              case POS_NINE: throw "Bad";
              case POS_EIGHT: current = POS_NINE; break;
              case POS_SEVEN: current = POS_EIGHT; break;
              case POS_SIX: throw "Bad";
              case POS_FIVE: current = POS_SIX; break;
              case POS_FOUR: current = POS_FIVE; break;
              case POS_THREE: throw "Bad";
              case POS_TWO: current = POS_THREE; break;
              case POS_ONE: current = POS_TWO; break;
              case POS_A: throw "Bad";
              case POS_ZERO: current = POS_A; break;
              default: throw "Impossible";
            }
          }
          else if (move == POS_SOUTH) {
            switch (current) {
              case POS_NINE: current = POS_SIX; break;
              case POS_EIGHT: current = POS_FIVE; break;
              case POS_SEVEN: current = POS_FOUR; break;
              case POS_SIX: current = POS_THREE; break;
              case POS_FIVE: current = POS_TWO; break;
              case POS_FOUR: current = POS_ONE; break;
              case POS_THREE: current = POS_A; break;
              case POS_TWO: current = POS_ZERO; break;
              case POS_ONE: throw "Bad";
              case POS_A: throw "Bad";
              case POS_ZERO: throw "Bad";
              default: throw "Impossible";
            }
          }
          else if (move == POS_WEST) {
            switch (current) {
              case POS_NINE: current = POS_EIGHT; break;
              case POS_EIGHT: current = POS_SEVEN; break;
              case POS_SEVEN: throw "Bad";
              case POS_SIX: current = POS_FIVE; break;
              case POS_FIVE: current = POS_FOUR; break;
              case POS_FOUR: throw "Bad";
              case POS_THREE: current = POS_TWO; break;
              case POS_TWO: current = POS_ONE; break;
              case POS_ONE: throw "Bad";
              case POS_A: current = POS_ZERO; break;
              case POS_ZERO: throw "Bad";
              default: throw "Impossible";
            }
          }
          else {
            throw "Really?";
          }
        }
        assert (current == pos2);
      }
    }
  }
}

std::set<Code>
getOptimalDirpadMoves (RobotPosition start, RobotPosition end)
{
  switch (start) {
    case POS_NORTH: switch (end) {
      case POS_NORTH: return {{}};
      case POS_A: return {{POS_EAST}};
      case POS_WEST: return {{POS_SOUTH, POS_WEST}};
      case POS_SOUTH: return {{POS_SOUTH}};
      case POS_EAST: return {{POS_SOUTH, POS_EAST}, {POS_EAST, POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_A: switch (end) {
      case POS_NORTH: return {{POS_WEST}};
      case POS_A: return {{}};
      case POS_WEST: return {{POS_SOUTH, POS_WEST, POS_WEST}};
      case POS_SOUTH: return {{POS_SOUTH, POS_WEST}, {POS_WEST, POS_SOUTH}}; // West before South, according to https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/ -- except that it doesn't work for me!
      case POS_EAST: return {{POS_SOUTH}};
      default: throw "Impossible";
    }
    case POS_WEST: switch (end) {
      case POS_NORTH: return {{POS_EAST, POS_NORTH}};
      case POS_A: return {{POS_EAST, POS_EAST, POS_NORTH}};
      case POS_WEST: return {{}};
      case POS_SOUTH: return {{POS_EAST}};
      case POS_EAST: return {{POS_EAST, POS_EAST}};
      default: throw "Impossible";
    }
    case POS_SOUTH: switch (end) {
      case POS_NORTH: return {{POS_NORTH}};
      case POS_A: return {{POS_NORTH, POS_EAST}, {POS_EAST, POS_NORTH}};
      case POS_WEST: return {{POS_WEST}};
      case POS_SOUTH: return {{}};
      case POS_EAST: return {{POS_EAST}};
    }
    case POS_EAST: switch (end) {
      case POS_NORTH: return {{POS_NORTH, POS_WEST}, {POS_WEST, POS_NORTH}}; // West before North, according to https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/
      case POS_A: return {{POS_NORTH}};
      case POS_WEST: return {{POS_WEST, POS_WEST}};
      case POS_SOUTH: return {{POS_WEST}};
      case POS_EAST: return {{}};
      default: throw "Impossible";
    }
    default: throw "Also Impossible";
  }
}

void
sanityCheckDirpadMoves ()
{
  for (RobotPosition pos : validDirpadPositions) {
    for (RobotPosition pos2 : validDirpadPositions) {
      std::set<Code> solutions = getOptimalDirpadMoves (pos, pos2);
      for (const Code& code : solutions) {
        RobotPosition current = pos;
        for (RobotPosition move : code) {
          if (move == POS_NORTH) {
            switch (current) {
              case POS_NORTH: throw "Bad";
              case POS_A: throw "Bad";
              case POS_WEST: throw "Bad";
              case POS_SOUTH: current = POS_NORTH; break;
              case POS_EAST: current = POS_A; break;
              default: throw "Impossible";
            }
          } else if (move == POS_EAST) {
            switch (current) {
              case POS_NORTH: current = POS_A; break;
              case POS_A: throw "Bad";
              case POS_WEST: current = POS_SOUTH; break;
              case POS_SOUTH: current = POS_EAST; break;
              case POS_EAST: throw "Bad";
              default: throw "Impossible";
            }
          }
          else if (move == POS_SOUTH) {
            switch (current) {
              case POS_NORTH: current = POS_SOUTH; break;
              case POS_A: current = POS_EAST; break;
              case POS_WEST: throw "Bad";
              case POS_SOUTH: throw "Bad";
              case POS_EAST: throw "Bad";
              default: throw "Impossible";
            }
          }
          else if (move == POS_WEST) {
            switch(current) {
              case POS_NORTH: throw "Bad";
              case POS_A: current = POS_NORTH; break;
              case POS_WEST: throw "Bad";
              case POS_SOUTH: current = POS_WEST; break;
              case POS_EAST: current = POS_SOUTH; break;
              default: throw "Impossible";
            }
          }
          else {
            throw "Really?";
          }
        }
        assert (current == pos2);
      }
    }
  }
}

struct State
{
  RobotPosition currentLocation;
  Code movesToMake;
  bool isKeyPad;

  auto operator<=>(const State&) const = default;
};


std::set<Code>
optimize (const State& state)
{
  static std::map<State, std::set<Code>> cache;
  if (cache.count (state) == 1) { return cache.at (state); }
  else if (state.movesToMake.empty ()) {
    return {{}};
  }
  else {
    std::set<Code> partialResults;
    if (state.isKeyPad) {
      partialResults = getOptimalKeypadMoves (state.currentLocation, state.movesToMake.front ());
    }
    else {
      partialResults = getOptimalDirpadMoves (state.currentLocation, state.movesToMake.front ());
    }
    std::set<Code> results;
    for (const Code& partial: partialResults) {
      std::set<Code> theRest = optimize ({state.movesToMake.front (), {state.movesToMake.begin () + 1, state.movesToMake.end ()}, state.isKeyPad});
      for (const Code& rest : theRest) {
        results.insert (partial + POS_A + rest);
      }
    }
    unsigned int length = results.begin ()->size ();
    for (const Code& code : results) {
      assert (code.length () == length);
    }
    cache[state] = results;
    return results;
  }
}

std::set<Code>
optimize (const std::set<Code>& instructions, bool isKeyPad)
{
  static std::map<std::pair<std::set<Code>, bool>, std::set<Code>> cache;
  if (cache.contains ({instructions, isKeyPad})) { return cache.at ({instructions, isKeyPad}); }
  std::set<Code> results;
  for (const Code& toMove: instructions) {
    std::set<Code> best = optimize ({POS_A, toMove, isKeyPad});
    results.insert (best.begin (), best.end ());
  }
  unsigned int length = results.begin ()->size ();
  for (const Code& code : results) {
    assert (code.length () == length);
  }
  cache[{instructions, isKeyPad}] = results;
  return results;
}

void
print (const std::set<Code>& stuff) {
  for (const Code& thing: stuff) {
    std::cout << thing << "\n";
  }
}

std::set<Code>
findShortestCode (const Code& desired, int thisLevel, int totalLevels)
{
  //static std::map<std::pair<Code, std::pair<int, int>>, std::set<Code>> cache;
  //if (cache.contains ({desired, {thisLevel, totalLevels}})) { return cache[{desired, {thisLevel, totalLevels}}]; }
  if (thisLevel == totalLevels) { return {desired}; }
  else {
    std::set<Code> partials = optimize ({desired}, thisLevel == 0);
    std::set<Code> results;
    for (const Code& code : partials) {
      std::set<Code> these = findShortestCode (code, thisLevel + 1, totalLevels);
      results.insert (these.begin (), these.end ());
    }
    unsigned int length = results.begin ()->length ();
    for (std::set<Code>::iterator it = results.begin (); it != results.end (); ) {
      if (it->length () == length) {
        ++it;
      }
      else {
        it = results.erase (it);
      }
    }
    //cache[{desired, {thisLevel, totalLevels}}] = results;
    //std::cout << "At level " << thisLevel << " returning " << results.size () << " results including " << *results.begin () << "\n";
    return results;
  }
}

long solve (const Problem& prob, int levels) {
  long total = 0;
  for (const Code& code : prob) {
    std::set<Code> patterns = findShortestCode (code, 0, levels);
    //std::cout << *patterns.begin () << ": " << patterns.begin ()->size () << " * " << atoi (code.c_str ()) << "\n";
    total += patterns.begin ()->size () * atoi (code.c_str ());
  }
  return total;
}


//// Restarting, mostly ////

struct State2
{
  RobotPosition currentLocation;
  Code movesToMake;
  int level;

  auto operator<=>(const State2&) const = default;
};


std::set<Code>
findOptimalPaths (const State2& state) {
  std::cout << state.movesToMake.size () << "\n";
  static std::map<State2, std::set<Code>> cache;
  if (cache.contains (state)) { return cache[state]; }
  if (state.movesToMake == "") { return {""}; } // No further movements needed.
  else {
    std::set<Code> firstMoveOptions;
    if (state.level == 0) { firstMoveOptions = getOptimalKeypadMoves (state.currentLocation, state.movesToMake[0]); }
    else { firstMoveOptions = getOptimalDirpadMoves (state.currentLocation, state.movesToMake[0]); }
    std::set<Code> restOptions = findOptimalPaths ({state.movesToMake[0], state.movesToMake.substr (1), state.level});
    std::set<Code> combos;
    for (const Code& a : firstMoveOptions) {
      for (const Code& b: restOptions) {
        combos.insert (a + POS_A + b);
      }
    }
    cache[state] = combos;
    return combos;
  }
}

std::set<Code>
allTheWayDown (const Code& toDo, int totalLevels)
{
  std::set<Code> after = {toDo};
  for (int i = 0; i < totalLevels; ++i) {
    std::set<Code> next;
    for (const Code& code : after) {
      std::set<Code> temp = findOptimalPaths ({POS_A, code, i});
      next.insert (temp.begin (), temp.end ());
    }
    after = next;
  }
  return after;
}

long solve2 (const Problem& prob, int levels) {
  long total = 0;
  for (const Code& code : prob) {
    std::set<Code> patterns = allTheWayDown (code, levels);
    //std::cout << *patterns.begin () << ": " << patterns.begin ()->size () << " * " << atoi (code.c_str ()) << "\n";
    total += patterns.begin ()->size () * atoi (code.c_str ());
  }
  return total;
}


//// Attempt #3 ////

/*
buildSeq(keys, index, prevKey, currPath, result):
    if index is the length of keys:
        add the current path to the result
        return
    foreach path in the keypad graph from prevKey to the currKey:
        buildSeq(keys, index+1, keys[index], currPath + path + 'A', result)
*/

/*
void
buildSeq (const Code& keys, const unsigned int index, const char prevKey, const Code& currPath, std::set<Code>& result)
{
  if (index == keys.size ()) {
    result.insert (currPath);
  }
  else {
    for (Code path : getOptimalDirpadMoves (prevKey, keys.at (index))) {
      buildSeq (keys, index + 1, keys[index], currPath + path + 'A', result);
    }
  }
}
*/

/*
shortestSeq(keys, depth, cache):
    if depth is 0:
        return the length of keys
    if keys, depth in cache:
        return value from cache
    split the keys into subKeys at 'A'
    foreach subKey in the subKeys list:
        biuld the sequence list for the subKey (buildSeq)
        for each sequence in the list:
            find the minimum of shortestSeq(sequence, depth-1, cache)
        add the minimum to the total
    set the total at keys, depth in the cache
    return total
*/
/*
long long shortestSeq (Code keys, unsigned int depth)
{
  static std::map<std::pair<Code, unsigned int>, long long> cache;
  if (depth == 0) { return keys.length (); }
  if (cache.contains ({keys, depth})) { return cache[{keys, depth}]; }
  std::vector<Code> splitOnA;
  std::size_t loc = keys.find (POS_A);
  while (loc != std::string::npos) {
    splitOnA.push_back (keys.substr (0, loc));
    keys = keys.substr (loc);
    loc = keys.find (POS_A);
  }
  assert (keys == "");
  long long total = 0;
  for (Code subkey : splitOnA) {
    std::set<Code> result;
    buildSeq (subkey, 0, 'A', "", result);
    long long minimum = LONG_LONG_MAX;
    for (Code answer : result) {
      long long thing = shortestSeq (answer, depth - 1);
      minimum = std::min (minimum, thing);
    }
    total += minimum;
  }
  cache[{keys, depth}] = total;
  return total;
}

long long hisSolve (const std::vector<Code>& codes, unsigned int maxDepth)
{
  for (Code code : codes) {

  }
}
*/

std::set<Code>
shortestPathsNumPad (char start, char end)
{
  static std::map<std::pair<char, char>, std::set<Code>> cache;
  if (cache.contains ({start, end})) { return cache[{start, end}]; }

  struct Temp
  {
    char at;
    Code moved;
  };

  std::set<Code> best;
  std::list<Temp> frontier;
  frontier.push_back ({start, ""});
  while (!frontier.empty ()) {
    Temp current = frontier.front ();
    frontier.pop_front ();
    if (current.at == end) {
      if (best.empty () || best.begin ()->size () == current.moved.size ()) { best.insert (current.moved); }
    }
    else if (best.empty () || best.begin ()->size () > current.moved.size ()) {
      switch (current.at) {
        case POS_NINE:
        frontier.push_back ({POS_SIX, current.moved + POS_SOUTH});
        frontier.push_back ({POS_EIGHT, current.moved + POS_WEST});
        break;
        case POS_EIGHT:
        frontier.push_back ({POS_NINE, current.moved + POS_EAST});
        frontier.push_back ({POS_FIVE, current.moved + POS_SOUTH});
        frontier.push_back ({POS_SEVEN, current.moved + POS_WEST});
        break;
        case POS_SEVEN:
        frontier.push_back ({POS_EIGHT, current.moved + POS_EAST});
        frontier.push_back ({POS_FOUR, current.moved + POS_SOUTH});
        break;
        case POS_SIX:
        frontier.push_back ({POS_NINE, current.moved + POS_NORTH});
        frontier.push_back ({POS_THREE, current.moved + POS_SOUTH});
        frontier.push_back ({POS_FIVE, current.moved + POS_WEST});
        break;
        case POS_FIVE:
        frontier.push_back ({POS_EIGHT, current.moved + POS_NORTH});
        frontier.push_back ({POS_SIX, current.moved + POS_EAST});
        frontier.push_back ({POS_TWO, current.moved + POS_SOUTH});
        frontier.push_back ({POS_FOUR, current.moved + POS_WEST});
        break;
        case POS_FOUR:
        frontier.push_back ({POS_SEVEN, current.moved + POS_NORTH});
        frontier.push_back ({POS_FIVE, current.moved + POS_EAST});
        frontier.push_back ({POS_ONE, current.moved + POS_SOUTH});
        break;
        case POS_THREE:
        frontier.push_back ({POS_SIX, current.moved + POS_NORTH});
        frontier.push_back ({POS_A, current.moved + POS_SOUTH});
        frontier.push_back ({POS_TWO, current.moved + POS_WEST});
        break;
        case POS_TWO:
        frontier.push_back ({POS_FIVE, current.moved + POS_NORTH});
        frontier.push_back ({POS_THREE, current.moved + POS_EAST});
        frontier.push_back ({POS_ZERO, current.moved + POS_SOUTH});
        frontier.push_back ({POS_ONE, current.moved + POS_WEST});
        break;
        case POS_ONE:
        frontier.push_back ({POS_FOUR, current.moved + POS_NORTH});
        frontier.push_back ({POS_TWO, current.moved + POS_EAST});
        break;
        case POS_A:
        frontier.push_back ({POS_THREE, current.moved + POS_NORTH});
        frontier.push_back ({POS_ZERO, current.moved + POS_WEST});
        break;
        case POS_ZERO:
        frontier.push_back ({POS_TWO, current.moved + POS_NORTH});
        frontier.push_back ({POS_A, current.moved + POS_EAST});
      }
    }
  }
  // Remove zigzags.
  for (auto it = best.begin (); it != best.end (); ) {
    if (it->find ("^>") != std::string::npos && it->find (">^") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("^<") != std::string::npos && it->find ("<^") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("v<") != std::string::npos && it->find ("<v") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("v>") != std::string::npos && it->find (">v") != std::string::npos) {
      it = best.erase (it);
    }
    else {
      ++it;
    }
  }
  cache[{start, end}] = best;
  return best;
}

std::set<Code>
shortestPathsDirPad (char start, char end)
{
  static std::map<std::pair<char, char>, std::set<Code>> cache;
  if (cache.contains ({start, end})) { return cache[{start, end}]; }

  struct Temp
  {
    char at;
    Code moved;
  };

  std::set<Code> best;
  std::list<Temp> frontier;
  frontier.push_back ({start, ""});
  while (!frontier.empty ()) {
    Temp current = frontier.front ();
    frontier.pop_front ();
    if (current.at == end) {
      if (best.empty () || best.begin ()->size () == current.moved.size ()) { best.insert (current.moved); }
    }
    else if (best.empty () || best.begin ()->size () > current.moved.size ()) {
      switch (current.at) {
        case POS_A:
        frontier.push_back ({POS_EAST, current.moved + POS_SOUTH});
        frontier.push_back ({POS_NORTH, current.moved + POS_WEST});
        break;
        case POS_NORTH:
        frontier.push_back ({POS_A, current.moved + POS_EAST});
        frontier.push_back ({POS_SOUTH, current.moved + POS_SOUTH});
        break;
        case POS_EAST:
        frontier.push_back ({POS_A, current.moved + POS_NORTH});
        frontier.push_back ({POS_SOUTH, current.moved + POS_WEST});
        break;
        case POS_SOUTH:
        frontier.push_back ({POS_NORTH, current.moved + POS_NORTH});
        frontier.push_back ({POS_EAST, current.moved + POS_EAST});
        frontier.push_back ({POS_WEST, current.moved + POS_WEST});
        break;
        case POS_WEST:
        frontier.push_back ({POS_SOUTH, current.moved + POS_EAST});
      }
    }
  }
  // Remove zigzags.
  for (auto it = best.begin (); it != best.end (); ) {
    if (it->find ("^>") != std::string::npos && it->find (">^") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("^<") != std::string::npos && it->find ("<^") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("v<") != std::string::npos && it->find ("<v") != std::string::npos) {
      it = best.erase (it);
    }
    else if (it->find ("v>") != std::string::npos && it->find (">v") != std::string::npos) {
      it = best.erase (it);
    }
    else {
      ++it;
    }
  }
  cache[{start, end}] = best;
  return best;
}

struct State3
{
  Code code;
  char pos;
  unsigned int depth;
  auto operator<=>(const State3&) const = default;
};

std::set<Code>
optimize3 (const State3& state)
{
  static std::map<State3, std::set<Code>> cache;
  if (cache.contains (state)) { return cache[state]; }
  else if (state.code == "") { return {""}; }
  else {
    std::set<Code> firstMoveOptions;
    if (state.depth == 0) {
      firstMoveOptions = getOptimalKeypadMoves (state.pos, state.code[0]);
      std::set<Code> alternative = shortestPathsNumPad (state.pos, state.code[0]);
      assert (firstMoveOptions == alternative);
    }
    else {
      firstMoveOptions = getOptimalDirpadMoves (state.pos, state.code[0]);
      std::set<Code> alternative = shortestPathsDirPad (state.pos, state.code[0]);
      assert (firstMoveOptions == alternative);
    }
    std::set<Code> restOptions = optimize3 ({state.code.substr (1), state.code[0], state.depth});
    std::set<Code> combos;
    for (const Code& a : firstMoveOptions) {
      for (const Code& b: restOptions) {
        combos.insert (a + POS_A + b);
      }
    }
    //std::cout << "Answering with " << *combos.begin () << " for " << state.pos << ", " << state.code << ", " << state.depth << "\n";
    cache[state] = combos;
    return combos;
  }
}

std::string
multiCodes (Code multi, unsigned int depth)
{
  std::string result;
  std::size_t aLoc = multi.find (POS_A);
  while (!multi.empty ()) {
    std::string part = multi.substr (0, aLoc + 1);
    multi = multi.substr (aLoc + 1);
    aLoc = multi.find (POS_A);
    std::set<std::string> options = optimize3 ({part, POS_A, depth});
    result += *options.begin ();
  }
  return result;
}

Code
deepDive (Code multi, unsigned int depth)
{
  for (unsigned int i = 0; i < depth; i++) {
    multi = multiCodes (multi, i);
  }
  return multi;
}

long long
part2 (const Problem& prob)
{
  long long result;
  for (const Code& code : prob) {
    std::string shortest = deepDive (code, 26);
    result += shortest.size () * atoi (code.c_str ());
  }
  return result;
}

// Greedy Rules:
// < before ^
// < before v
// v before >
// ^ before >

std::string
greedyNumPad (const char from, const char to)
{
  switch (from) {
    case POS_NINE: {
      switch (to) {
        case POS_NINE: return "A";
        case POS_EIGHT: return "<A";
        case POS_SEVEN: return "<<A";
        case POS_SIX: return "vA";
        case POS_FIVE: return "<vA";
        case POS_FOUR: return "<<vA";
        case POS_THREE: return "vvA";
        case POS_TWO: return "<vA";
        case POS_ONE: return "<<vA";
        case POS_A: return "vvvA";
        case POS_ZERO: return "<vvvA";
        default: throw "Impossible";
      }
    }
    case POS_EIGHT: {
      switch (to) {
        case POS_NINE: return ">A";
        case POS_EIGHT: return "A";
        case POS_SEVEN: return "<A";
        case POS_SIX: return "v>A";
        case POS_FIVE: return "vA";
        case POS_FOUR: return "<vA";
        case POS_THREE: return "vv>A";
        case POS_TWO: return "vvA";
        case POS_ONE: return "<vvA";
        case POS_A: return "vvv>A";
        case POS_ZERO: return "vvvA";
        default: throw "Impossible";
      }
    }
    case POS_SEVEN: {
      switch (to) {
        case POS_NINE: return ">>A";
        case POS_EIGHT: return ">A";
        case POS_SEVEN: return "A";
        case POS_SIX: return "v>>A";
        case POS_FIVE: return "v>A";
        case POS_FOUR: return "vA";
        case POS_THREE: return "vv>>A";
        case POS_TWO: return "vv>A";
        case POS_ONE: return "vvA";
        case POS_A: return ">>vvvA";
        case POS_ZERO: return ">vvvA";
        default: throw "Impossible";
      }
    }
    case POS_SIX: {
      switch (to) {
        case POS_NINE: return "^A";
        case POS_EIGHT: return "<^A";
        case POS_SEVEN: return "<<^A";
        case POS_SIX: return "A";
        case POS_FIVE: return "<A";
        case POS_FOUR: return "<<A";
        case POS_THREE: return "vA";
        case POS_TWO: return "<vA";
        case POS_ONE: return "<<vA";
        case POS_A: return "vvA";
        case POS_ZERO: return "<vvA";
        default: throw "Impossible";
      }
    }
    case POS_FIVE: {
      switch (to) {
        case POS_NINE: return "^>A";
        case POS_EIGHT: return "^A";
        case POS_SEVEN: return "<^A";
        case POS_SIX: return ">A";
        case POS_FIVE: return "A";
        case POS_FOUR: return "<A";
        case POS_THREE: return "v>A";
        case POS_TWO: return "vA";
        case POS_ONE: return "<vA";
        case POS_A: return "vv>A";
        case POS_ZERO: return "vvA";
        default: throw "Impossible";
      }
    }
    case POS_FOUR: {
      switch (to) {
        case POS_NINE: return "^>>A";
        case POS_EIGHT: return "^>A";
        case POS_SEVEN: return "^A";
        case POS_SIX: return ">>A";
        case POS_FIVE: return ">A";
        case POS_FOUR: return "A";
        case POS_THREE: return "v>>A";
        case POS_TWO: return "v>A";
        case POS_ONE: return "vA";
        case POS_A: return ">>vvA";
        case POS_ZERO: return ">vvA";
        default: throw "Impossible";
      }
    }
    case POS_THREE: {
      switch (to) {
        case POS_NINE: return "^^A";
        case POS_EIGHT: return "<^^A";
        case POS_SEVEN: return "<<^^A";
        case POS_SIX: return "^A";
        case POS_FIVE: return "<^A";
        case POS_FOUR: return "<<^A";
        case POS_THREE: return "A";
        case POS_TWO: return "<A";
        case POS_ONE: return "<<A";
        case POS_A: return "vA";
        case POS_ZERO: return "<vA";
        default: throw "Impossible";
      }
    }
    case POS_TWO: {
      switch (to) {
        case POS_NINE: return "^^>A";
        case POS_EIGHT: return "^^A";
        case POS_SEVEN: return "<^^A";
        case POS_SIX: return "^>A";
        case POS_FIVE: return "^A";
        case POS_FOUR: return "<^A";
        case POS_THREE: return ">A";
        case POS_TWO: return "A";
        case POS_ONE: return "<A";
        case POS_A: return "v>A";
        case POS_ZERO: return "vA";
        default: throw "Impossible";
      }
    }
    case POS_ONE: {
      switch (to) {
        case POS_NINE: return "^^>>A";
        case POS_EIGHT: return "^^>A";
        case POS_SEVEN: return "^^A";
        case POS_SIX: return "^>>A";
        case POS_FIVE: return "^>A";
        case POS_FOUR: return "^A";
        case POS_THREE: return ">>A";
        case POS_TWO: return ">A";
        case POS_ONE: return "A";
        case POS_A: return ">>vA";
        case POS_ZERO: return ">vA";
        default: throw "Impossible";
      }
    }
    case POS_A: {
      switch (to) {
        case POS_NINE: return "^^^A";
        case POS_EIGHT: return "<^^^A";
        case POS_SEVEN: return "^^^<<A";
        case POS_SIX: return "^^A";
        case POS_FIVE: return "<^^A";
        case POS_FOUR: return "^^<<A";
        case POS_THREE: return "^A";
        case POS_TWO: return "<^A";
        case POS_ONE: return "^<<A";
        case POS_A: return "A";
        case POS_ZERO: return "<A";
        default: throw "Impossible";
      }
    }
    case POS_ZERO: {
      switch (to) {
        case POS_NINE: return "^^^>A";
        case POS_EIGHT: return "^^^A";
        case POS_SEVEN: return "^^^<A";
        case POS_SIX: return "^^>A";
        case POS_FIVE: return "^^A";
        case POS_FOUR: return "^^<A";
        case POS_THREE: return "^>A";
        case POS_TWO: return "^A";
        case POS_ONE: return "^<A";
        case POS_A: return ">A";
        case POS_ZERO: return "A";
        default: throw "Impossible";
      }
    }
    default: throw "Impossible";
  }
}

std::string
greedyDirPad (const char from, const char to)
{
  switch (from) {
    case POS_A: {
      switch (to) {
        case POS_A: return "A";
        case POS_NORTH: return "<A";
        case POS_EAST: return "vA";
        case POS_SOUTH: return "<vA";
        case POS_WEST: return "v<<A";
        default: throw "Impossible";
      }
    }
    case POS_NORTH: {
      switch (to) {
        case POS_A: return ">A";
        case POS_NORTH: return "A";
        case POS_EAST: return "v>A";
        case POS_SOUTH: return "vA";
        case POS_WEST: return "v<A";
        default: throw "Impossible";
      }
    }
    case POS_EAST: {
      switch (to) {
        case POS_A: return "^A";
        case POS_NORTH: return "<^A";
        case POS_EAST: return "A";
        case POS_SOUTH: return "<A";
        case POS_WEST: return "<<A";
        default: throw "Impossible";
      }
    }
    case POS_SOUTH: {
      switch (to) {
        case POS_A: return "^>A";
        case POS_NORTH: return "^A";
        case POS_EAST: return ">A";
        case POS_SOUTH: return "A";
        case POS_WEST: return "<A";
        default: throw "Impossible";
      }
    }
    case POS_WEST: {
      switch (to) {
        case POS_A: return ">>^A";
        case POS_NORTH: return ">^A";
        case POS_EAST: return ">>A";
        case POS_SOUTH: return ">A";
        case POS_WEST: return "A";
        default: throw "Impossible";
      }
    }
    default: throw "Impossible";
  }
}

/*
Code
greedy (const Code& code, unsigned int level)
{
  static std::map<std::pair<Code, unsigned int>, Code> cache;
  if (cache.contains ({code, level})) { return cache[{code, level}]; }
  std::stringstream newCode;
  char state = POS_A;
  for (char c : code) {
    if (level == 0) {
      newCode << greedyNumPad (state, c);
    }
    else {

    }
    state = c;
  }
}
*/

std::string
greedy (const char c, const unsigned int level, char& bottomPos)
{
  static std::map<std::pair<char, unsigned int>, std::string> cache;
  //if (cache.contains ({c, level})) { return cache[{c, level}]; }
  if (level == 0) {
    std::string answer = greedyNumPad (bottomPos, c);
    bottomPos = c;
    return answer;
  }
  else {
    std::string prev = greedy (c, level - 1, bottomPos);
    std::stringstream current;
    char myCurrent = POS_A;
    for (char d : prev) {
      current << greedyDirPad (myCurrent, d);
      myCurrent = d;
    }
    return current.str ();
  }
}

std::string
severalChars (const std::string& code, unsigned int levels)
{
  char bottomPos = POS_A;
  std::stringstream result;
  for (char c : code) {
    result << greedy (c, levels, bottomPos);
  }
  return result.str ();
}

struct CodesWithCounts
{
  std::map<Code, long long> map;

  void addCode (const Code& code, long long count = 1)
  {
    if (map.contains (code)) { map[code] = map[code] + count; }
    else { map[code] = count; }
  }
};

CodesWithCounts
oneLastIdea (const CodesWithCounts& oldCounts, int level)
{
  CodesWithCounts newCounts;
  for (auto it = oldCounts.map.cbegin (); it != oldCounts.map.end (); ++it) {
    if (level == 0) {
      char current = POS_A;
      for (char c : it->first) {
        newCounts.addCode (greedyNumPad (current, c), it->second);
        current = c;
      }
    }
    else {
      char current = POS_A;
      for (char c : it->first) {
        newCounts.addCode (greedyDirPad (current, c), it->second);
        current = c;
      }
    }
  }
  return newCounts;
}

long long
oneLastIdeaPt2 (const std::vector<Code>& codes, int totalLevels)
{
  long long totalComplexities = 0;
  for (const Code& code : codes) {
    CodesWithCounts counts;
    counts.addCode (code, 1);
    for (int i = 0; i <= totalLevels; ++i) {
      counts = oneLastIdea (counts, i);
    }
    long long length = 0;
    for (auto it = counts.map.cbegin (); it != counts.map.cend (); ++it) {
      length += it->first.size () * it->second;
    }
    totalComplexities += length * atoi (code.c_str ());
  }
  return totalComplexities;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  sanityCheckKeypadMoves ();
  sanityCheckDirpadMoves ();
  Problem prob = readInput ();
  //std::set<Code> ways = optimize ({{POS_ZERO, POS_TWO, POS_NINE, POS_A}}, POS_A, true);
  //print (ways);
  //std::cout << solve (prob, 3) << "\n";
  //std::cout << solve (prob, 26 /*4*/) << "\n";
  //std::set<Code> ways = findOptimalPaths ({POS_A, {POS_ZERO, POS_TWO, POS_NINE, POS_A}, 0});
  /*
  std::set<Code> ways = allTheWayDown ({POS_ZERO, POS_TWO, POS_NINE, POS_A}, 1);
  print (ways);
  ways = allTheWayDown ({POS_ZERO, POS_TWO, POS_NINE, POS_A}, 2);
  print (ways);
  ways = allTheWayDown ({POS_ZERO, POS_TWO, POS_NINE, POS_A}, 3);
  print (ways);
  */
  //std::cout << solve2 (prob, 3) << "\n";
  //std::cout << solve2 (prob, 26) << "\n";

/*
  Code keys = "<A";
  std::set<Code> result;
  buildSeq (keys, 0, 'A', "", result);
  print (result);
  */
 /*
  std::cout << deepDive ("029A", 1) << "\n";
  std::cout << deepDive ("029A", 2) << "\n";
  std::cout << deepDive ("029A", 3) << "\n";
  std::cout << deepDive ("029A", 4) << "\n";
  */
// std::cout << part2 (prob) << "\n";
//  std::cout << deepDive ("029A", 5) << "\n";
/*
char bottomPos = POS_A;
std::cout << greedy ('0', 0, bottomPos) << "\n";
bottomPos = POS_A;
std::cout << greedy ('0', 1, bottomPos) << "\n";
bottomPos = POS_A;
std::cout << greedy ('0', 2, bottomPos) << "\n";
bottomPos = POS_A;
std::cout << greedy ('0', 3, bottomPos) << "\n";
bottomPos = POS_A;
std::cout << greedy ('0', 4, bottomPos) << "\n";
*/
//std::cout << severalChars ("029A", 23).size () << "\n";
std::cout << oneLastIdeaPt2 (prob, 25) << "\n";
  return 0;
}
