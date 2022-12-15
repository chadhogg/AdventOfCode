/// \file 2022Day14.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-14.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>

using Point = std::pair<std::size_t, std::size_t>;
using Line = std::vector<Point>;
using Grid = std::vector<std::vector<char>>;

const char ROCK = '#';
const char SAND = 'o';
const char AIR = '.';

const Point SAND_START_LOC = {500, 0};

Line
parse (const std::string text)
{
  Line line;
  std::size_t start = 0;
  std::size_t separate = text.find (" -> ", start);
  while (separate != std::string::npos)
  {
    std::string part = text.substr (start, separate - start);
    std::size_t comma = part.find (",");
    assert (comma != std::string::npos);
    std::string xStr = part.substr (0, comma);
    std::string yStr = part.substr (comma + 1);
    for (char c : xStr) { assert (isdigit (c)); }
    for (char c : yStr) { assert (isdigit (c)); }
    line.push_back ({atoi (xStr.c_str ()), atoi (yStr.c_str ())});
    start = separate + 4;
    separate = text.find (" -> ", start);
  }
  std::string part = text.substr (start);
  std::size_t comma = part.find (",");
  assert (comma != std::string::npos);
  std::string xStr = part.substr (0, comma);
  std::string yStr = part.substr (comma + 1);
  for (char c : xStr) { assert (isdigit (c)); }
  for (char c : yStr) { assert (isdigit (c)); }
  line.push_back ({atoi (xStr.c_str ()), atoi (yStr.c_str ())});
  return line;
}

std::vector<Line>
getInput ()
{
  std::vector<Line> lines;
  std::string text;
  while (std::getline (std::cin, text))
  {
    if (text.empty ()) { break; }
    lines.push_back (parse (text));
  }
  return lines;
}

void
drawLine (Grid& grid, const Line& line)
{
  std::size_t currentX = line[0].first;
  std::size_t currentY = line[0].second;
  grid[currentY][currentX] = ROCK;
  for (std::size_t pointIndex = 1; pointIndex < line.size (); ++pointIndex)
  {
    const Point& target = line[pointIndex];
    while (currentX != target.first || currentY != target.second)
    {
      if (currentX < target.first) { ++currentX; assert (currentY == target.second); }
      else if (currentX > target.first) { --currentX; assert (currentY == target.second); }
      else if (currentY < target.second) { ++currentY; assert (currentX == target.first); }
      else { assert (currentY > target.second); --currentY; }
      grid[currentY][currentX] = ROCK;
    }
  }
}

Grid
makeGrid (const std::vector<Line>& lines)
{
  std::size_t width = SAND_START_LOC.first;
  std::size_t depth = SAND_START_LOC.second;
  for (const Line& line : lines)
  {
    for (const Point& point : line)
    {
      width = std::max (width, point.first);
      depth = std::max (depth, point.second);
    }
  }
  ++width;
  ++depth;
  std::vector<char> defaultRow (width + 2, AIR);
  Grid grid (depth, defaultRow);

  for (const Line& line : lines)
  {
    drawLine (grid, line);
  }

  //for (std::size_t index = 0; index < width; ++index)
  //{
  //  grid.back ()[index] = EXIT;
  //}
  return grid;
}

void
printGrid (const Grid& grid)
{
  std::size_t firstRelevantX = grid[0].size ();
  for (std::size_t x = 0; x < grid[0].size () && firstRelevantX == grid[0].size (); ++x)
  {
    for (std::size_t y = 0; y < grid.size (); ++y)
    {
      if (grid[y][x] != AIR)
      {
        firstRelevantX = x;
      }
    }
  }

  for (std::size_t y = 0; y < grid.size (); ++y)
  {
    for (std::size_t x = firstRelevantX - 1; x < grid[y].size (); ++x)
    {
      std::cout << grid[y][x];
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void
dropSand (Grid& grid, std::size_t& currentRow, std::size_t& currentCol)
{
    while (true)
    {
      if (currentRow + 1 == grid.size ()) { return; }
      if (grid[currentRow + 1][currentCol] == AIR)
      {
        ++currentRow;
      }
      else if (grid[currentRow + 1][currentCol - 1] == AIR)
      {
        ++currentRow;
        --currentCol;
      }
      else if (grid[currentRow + 1][currentCol + 1] == AIR)
      {
        ++currentRow;
        ++currentCol;
      }
      else
      {
        grid[currentRow][currentCol] = SAND;
        return;
      }
    }
}

unsigned int
part1 (Grid grid)
{
  unsigned int count = 0;
  while (true)
  {
    //printGrid (grid);
    std::size_t whereX = SAND_START_LOC.first;
    std::size_t whereY = SAND_START_LOC.second;
    dropSand (grid, whereY, whereX);
    if (whereY + 1 == grid.size ()) { return count; }
    ++count;
  }
  return 0;
}

unsigned int
part2 (Grid grid)
{
  // Make sure we are as wide as we could possibly need to be
  for (std::size_t row = 0; row < grid.size (); ++row)
  {
    grid[row].insert (grid[row].end (), SAND_START_LOC.first, AIR);
  }
  grid.push_back ({});
  grid.back ().insert (grid.back ().end (), grid[0].size (), AIR);
  grid.push_back ({});
  grid.back ().insert (grid.back ().end (), grid[0].size (), ROCK);

  unsigned int count = 0;
  while (grid[SAND_START_LOC.second][SAND_START_LOC.first] != SAND)
  {
    std::size_t y = SAND_START_LOC.second;
    std::size_t x = SAND_START_LOC.first;
    dropSand (grid, y, x);
    ++count;
  }
  return count;
}

int main () {
  std::vector<Line> lines = getInput ();
  Grid grid = makeGrid (lines);
  std::cout << part1 (grid) << "\n";
  std::cout << part2 (grid) << "\n";
  return 0;
}
