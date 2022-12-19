/// \file 2022Day17.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-17.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <array>
#include <deque>
#include <iomanip>
#include <unordered_map>

const unsigned int WIDTH = 9;
const unsigned int DROP_Y = 3;
const unsigned int MAX_BLOCK_HEIGHT = 4;

enum Block { HORIZONTAL, PLUS, BACKL, VERTICAL, SQUARE };
enum Space { FLOOR, WALL, ROCK, AIR, FALLING };

using Number = unsigned long;
using JetPattern = std::list<char>;
using Row = std::array<Space, WIDTH>;

struct Cave : public std::deque<Row>
{
  Number m_erasedRows;
};

struct State
{
  Cave m_cave;
  JetPattern m_pattern;
  Block m_dropped;

  bool
  operator== (const State& b) const
  {
    return m_cave == b.m_cave && m_pattern == b.m_pattern && m_dropped == b.m_dropped;
  }
};

namespace std
{
  template<>
  struct hash<Row>
  {
    std::size_t
    operator() (const Row& key) const
    {
      std::size_t result = 0;
      for (std::size_t index = 0; index < key.size (); ++index)
      {
        result += (index + 1) * key[index];
      }
      return result;
    }
  };
  std::hash<Row> rowHasher;

  template<>
  struct hash<Cave>
  {
    std::size_t
    operator() (const Cave& key) const
    {
      std::size_t result = 0;
      for (std::size_t index = 0; index < key.size (); ++index)
      {
        result += (index + 1) * rowHasher (key[index]);
      }
      return result;
    }
  };
  std::hash<Cave> caveHasher;

  template<>
  struct hash<JetPattern>
  {
    std::size_t
    operator() (const JetPattern& pattern) const
    {
      std::size_t result = 0;
      std::size_t index = 0;
      for (char c : pattern)
      {
        ++index;
        result += index * c;
      }
      return result;
    }
  };
  std::hash<JetPattern> patternHasher;

  template<>
  struct hash<Block>
  {
    std::size_t
    operator() (const Block& block) const
    {
      switch (block)
      {
        case VERTICAL: return 1;
        case PLUS: return 2;
        case BACKL: return 3;
        case HORIZONTAL: return 4;
        default: return 5;
      }
    }
  };
  struct hash<Block> blockHasher;

  template<>
  struct hash<State>
  {
    std::size_t
    operator() (const State& key) const
    {
      return caveHasher (key.m_cave) + patternHasher (key.m_pattern) + blockHasher (key.m_dropped);
    }
  };
}

JetPattern
getInput ()
{
  std::string line;
  std::getline (std::cin, line);
  JetPattern pattern (line.begin (), line.end ());
  for (char c : line)
  {
    assert (c == '>' || c == '<');
  }
  return pattern;
}

bool
isEmpty (const Row& row)
{
  return row[1] == AIR && row[2] == AIR && row[3] == AIR && row[4] == AIR && row[5] == AIR && row[6] == AIR && row[7] == AIR;
}

void
extendCave (Cave& cave)
{
  cave.push_back ({WALL, AIR, AIR, AIR, AIR, AIR, AIR, AIR, WALL});
}

void
shrinkCave (Cave& cave)
{
  while (isEmpty (cave.back ()))
  {
    cave.pop_back ();
  }
}

void
expandCave (Cave& cave)
{
  if (cave.size () < DROP_Y + 1 || !isEmpty (cave[cave.size () - 1]) || !isEmpty (cave[cave.size () - 2]) || !isEmpty (cave[cave.size () - 3]) || !isEmpty (cave[cave.size () - 4]))
  {
    extendCave (cave);
    expandCave (cave);
  }
}

void
addRock (Cave& cave, Block block)
{
  switch (block)
  {
    case HORIZONTAL:
      cave.back ()[3] = cave.back ()[4] = cave.back ()[5] = cave.back ()[6] = FALLING;
      break;
    case PLUS:
      cave.back ()[4] = FALLING;
      extendCave (cave);
      cave.back ()[3] = cave.back ()[4] = cave.back ()[5] = FALLING;
      extendCave (cave);
      cave.back ()[4] = FALLING;
      break;
    case BACKL:
      cave.back ()[3] = cave.back ()[4] = cave.back ()[5] = FALLING;
      extendCave (cave);
      cave.back ()[5] = FALLING;
      extendCave (cave);
      cave.back ()[5] = FALLING;
      break;
    case VERTICAL:
      cave.back ()[3] = FALLING;
      extendCave (cave);
      cave.back ()[3] = FALLING;
      extendCave (cave);
      cave.back ()[3] = FALLING;
      extendCave (cave);
      cave.back ()[3] = FALLING;
      break;
    case SQUARE:
      cave.back ()[3] = cave.back ()[4] = FALLING;
      extendCave (cave);
      cave.back ()[3] = cave.back ()[4] = FALLING;
      break;
  }
}

void
pushLeftIfPossible (Cave& cave, std::size_t lowestRelevantRow)
{
  // Check if possible.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = 1; column < WIDTH - 1; ++column)
    {
      if (cave[row][column] == FALLING && (cave[row][column - 1] == WALL || cave[row][column - 1] == ROCK))
      {
        return;
      }
    }
  }
  // Actually do it.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = 1; column < WIDTH - 1; ++column)
    {
      if (cave[row][column] == FALLING)
      {
        assert (cave[row][column - 1] == AIR);
        std::swap (cave[row][column], cave[row][column - 1]);
      }
    }
  }
}

void
pushRightIfPossible (Cave& cave, std::size_t lowestRelevantRow)
{
  // Check if possible.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = WIDTH - 2; column > 0; --column)
    {
      if (cave[row][column] == FALLING && (cave[row][column + 1] == WALL || cave[row][column + 1] == ROCK))
      {
        return;
      }
    }
  }
  // Actually do it.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = WIDTH - 2; column > 0; --column)
    {
      if (cave[row][column] == FALLING)
      {
        assert (cave[row][column + 1] == AIR);
        std::swap (cave[row][column], cave[row][column + 1]);
      }
    }
  }
}

bool
fallIfPossible (Cave& cave, std::size_t lowestRelevantRow)
{
  // Check if possible.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = 1; column < WIDTH - 1; ++column)
    {
      if (cave[row][column] == FALLING && (cave[row - 1][column] == FLOOR || cave[row - 1][column] == ROCK))
      {
        return false;
      }
    }
  }
  // Actually do it.
  for (std::size_t row = lowestRelevantRow; row < cave.size () && row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
  {
    for (std::size_t column = 1; column < WIDTH - 1; ++column)
    {
      if (cave[row][column] == FALLING)
      {
        assert (cave[row - 1][column] == AIR);
        std::swap (cave[row][column], cave[row - 1][column]);
      }
    }
  }
  return true;
}

void
draw (const Cave& cave)
{
  std::size_t lineNum = cave.size () - 1;
  for (auto iter = cave.crbegin (); iter != cave.crend (); ++iter)
  {
    std::cout << std::setw(5) << lineNum << " ";
    for (const Space& space : *iter)
    {
      switch (space)
      {
        case FLOOR: std::cout << "_"; break;
        case WALL: std::cout << "|"; break;
        case ROCK: std::cout << "#"; break;
        case AIR: std::cout << "."; break;
        case FALLING: std::cout << "@"; break;
      }
    }
    std::cout << "\n";
    --lineNum;
  }
  std::cout << "  (+ " << cave.m_erasedRows << " rows that have been erased as no longer relevant.\n";
  std::cout << "\n";
}

std::size_t
findHighestLine (Cave& cave)
{
  for (std::size_t leftStart = cave.size () - 1; leftStart > 0; --leftStart)
  {
      std::stack <std::vector<std::size_t>> partialLines;
      if (cave[leftStart][1] == ROCK)
      {
        partialLines.push ({leftStart});
        while (!partialLines.empty ())
        {
          std::vector<std::size_t> partial = partialLines.top ();
          partialLines.pop ();
          if (partial.size () == WIDTH - 2)
          {
            return *std::min_element (partial.begin (), partial.end ());
          }
          if (cave[partial.back () - 1][partial.size () + 1] == ROCK)
          {
            std::vector<std::size_t> down = partial;
            down.push_back (partial.back () - 1);
            partialLines.push (down);
          } 
          if (cave[partial.back () + 1][partial.size () + 1] == ROCK)
          {
            std::vector<std::size_t> up = partial;
            up.push_back (partial.back () + 1);
            partialLines.push (up);
          } 
          if (cave[partial.back ()][partial.size () + 1] == ROCK)
          {
            std::vector<std::size_t> over = partial;
            over.push_back (partial.back ());
            partialLines.push (over);
          } 
        }
      }
  }
  return std::string::npos;
}

void
compactCave (Cave& cave)
{
  // The idea here is that if I can make a line across the entire formation, nothing 
  //   can ever fall below that line, and I can ignore it.  The line doesn't need to be straight, 
  //   just connected.  My subroutine above only finds lines that never jump a vertical distance
  //   more than 1, but hopefull is still useful.
  std::size_t where = findHighestLine (cave);
  if (where != std::string::npos && where > 2)
  {
    //draw (cave);
    //std::cout << "Compacting all lines below line " << where << " because they are unreachable.\n";
    cave.m_erasedRows += where - 1;
    cave.erase (cave.begin (), cave.begin () + where);
    cave.push_front ({WALL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL});
    //draw (cave);
    //std::cout << "\n\n";
  }
}

void
dropRock (Cave& cave, Block block, JetPattern& pattern)
{
  // Make it start with exactly three empty rows.
  shrinkCave (cave);
  expandCave (cave);
  addRock (cave, block);
  std::size_t lowestRelevantRow = cave.size () - MAX_BLOCK_HEIGHT /*(block == PLUS || block == BACKL) ? 3 : block == VERTICAL ? 4 : block == SQUARE ? 2 : 1*/;
  while (true)
  {
    //draw (cave);
    // Apply jet stream.
    //std::cout << pattern.front ();
    if (pattern.front () == '<')
    {
      pushLeftIfPossible (cave, lowestRelevantRow);
    }
    else
    {
      pushRightIfPossible (cave, lowestRelevantRow);
    }
    pattern.push_back (pattern.front ());
    pattern.pop_front ();
    //draw (cave);
    // Fall
    if (!fallIfPossible (cave, lowestRelevantRow))
    {
      for (std::size_t row = lowestRelevantRow; row < lowestRelevantRow + MAX_BLOCK_HEIGHT; ++row)
      {
        for (std::size_t col = 1; col < WIDTH - 1; ++col)
        {
          if (cave[row][col] == FALLING) { cave[row][col] = ROCK; }
        }
      }
      //draw (cave);
      return;
    }
    lowestRelevantRow = (lowestRelevantRow <= 1 ? lowestRelevantRow : lowestRelevantRow - 1);
  }
}

Cave
run (JetPattern pattern, Number rocksToDrop)
{
  Cave cave;
  cave.push_back ({WALL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL});
  std::list<Block> blocksToDrop = {HORIZONTAL, PLUS, BACKL, VERTICAL, SQUARE};
  Number rocksDropped = 0;
  std::unordered_map<State, std::pair<Number, Number>, std::hash<State>> previousStates;
  while (rocksDropped < rocksToDrop)
  {
    State current {cave, pattern, blocksToDrop.front ()};
    if (previousStates.count (current) == 0)
    {
      previousStates.insert ({current, {rocksDropped, cave.size () + cave.m_erasedRows}});
      dropRock (cave, blocksToDrop.front (), pattern);
      blocksToDrop.push_back (blocksToDrop.front ());
      blocksToDrop.pop_front ();
      ++rocksDropped;
      //draw (cave);
      //std::cout << "\n\n";
      compactCave (cave);
    }
    else
    {
      //std::cout << "Cycle detected!\n";
      //std::cout << "Dropped " << rocksDropped << " for " << cave.size () + cave.m_erasedRows << " rows.\n";
      //draw (cave);
      //std::cout << "Old entry:";
      //std::cout << "Dropped " << previousStates[current].first << " for " << previousStates[current].second << " rows.\n";

      Number droppedDiff = rocksDropped - previousStates[current].first;
      Number rowsDiff = cave.size () + cave.m_erasedRows - previousStates[current].second;
      Number repeats = (rocksToDrop - rocksDropped) / droppedDiff;
      cave.m_erasedRows += rowsDiff * repeats;
      rocksDropped += repeats * droppedDiff;
      //std::cout << "Skipped ahead " << repeats << " cycles of " << droppedDiff << " rocks each.\n";
      previousStates.clear ();
    }
    //std::cout << (rocksToDrop - rocksDropped) << " left to go.\n";
  }
  return cave;
}

int main () {
  JetPattern pattern = getInput ();
  Cave cave = run (pattern, 2022);
  shrinkCave (cave);
  std::cout << cave.size () + cave.m_erasedRows - 1 << "\n";
  cave = run (pattern, 1000000000000);
  shrinkCave (cave);
  std::cout << cave.size () + cave.m_erasedRows - 1 << "\n";
  return 0;
}

// My answers:
// 3083
// 1532183908048
