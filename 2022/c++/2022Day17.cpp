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

const unsigned int WIDTH = 9;
const unsigned int DROP_Y = 3;
const unsigned int MAX_BLOCK_HEIGHT = 4;

enum Block { HORIZONTAL, PLUS, BACKL, VERTICAL, SQUARE };
enum Space { FLOOR, WALL, ROCK, AIR, FALLING };

using JetPattern = std::list<char>;
using Row = std::array<Space, WIDTH>;
using Cave = std::vector<Row>;

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
  for (auto iter = cave.crbegin (); iter != cave.crend (); ++iter)
  {
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
  }
  std::cout << "\n";
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
run (JetPattern pattern, unsigned long rocksToDrop)
{
  Cave cave;
  cave.push_back ({WALL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL});
  std::list<Block> blocksToDrop = {HORIZONTAL, PLUS, BACKL, VERTICAL, SQUARE};
  unsigned long rocksDropped = 0;
  while (rocksDropped < rocksToDrop)
  {
    dropRock (cave, blocksToDrop.front (), pattern);
    blocksToDrop.push_back (blocksToDrop.front ());
    blocksToDrop.pop_front ();
    ++rocksDropped;
    //draw (cave);
    //std::cout << "\n\n";
  }
  return cave;
}

int main () {
  JetPattern pattern = getInput ();
  Cave cave = run (pattern, 2022);
  shrinkCave (cave);
  std::cout << cave.size () - 1 << "\n";
  return 0;
}
