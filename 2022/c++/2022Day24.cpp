/// \file 2022Day24.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-24.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>
#include <list>

using Number = std::size_t;

enum class Terrain { WALL, OPEN };

enum class Direction { NORTH, EAST, SOUTH, WEST };

struct Location
{
  Number row;
  Number col;
};

struct Blizzard
{
  Direction facing;
  Location where;
};

struct Problem
{
  std::vector<std::vector<Terrain>> map;
  Location entrance;
  Location exit;
  std::vector<Blizzard> initialBlizzards;
};

struct State
{
  Location expedition;
  std::vector<Blizzard> blizzards;
};


Problem
getInput ()
{
  Problem prob;
  std::string line;
  std::getline (std::cin, line);
  Number rowNum = 0;
  while (std::cin)
  {
    std::vector<Terrain> row;
    for (Number colNum = 0; colNum < (Number)line.size (); ++colNum)
    {
      if (line[colNum] == '#') { row.push_back (Terrain::WALL); }
      else if (line[colNum] == '.') { row.push_back (Terrain::OPEN); }
      else if (line[colNum] == '^')
      {
        row.push_back (Terrain::OPEN);
        prob.initialBlizzards.push_back ({Direction::NORTH, rowNum, colNum});
      }
      else if (line[colNum] == '>')
      {
        row.push_back (Terrain::OPEN);
        prob.initialBlizzards.push_back ({Direction::EAST, rowNum, colNum});
      }
      else if (line[colNum] == 'v')
      {
        row.push_back (Terrain::OPEN);
        prob.initialBlizzards.push_back ({Direction::SOUTH, rowNum, colNum});
      }
      else if(line[colNum] == '<')
      {
        row.push_back (Terrain::OPEN);
        prob.initialBlizzards.push_back ({Direction::WEST, rowNum, colNum});
      }
      else
      {
        assert (false);
      }
    }
    prob.map.push_back (row);
    std::getline (std::cin, line);
    ++rowNum;
  }

  prob.entrance.row = 0;
  prob.entrance.col = 10000;
  for (Number col = 0; col < prob.map[0].size (); ++col)
  {
    if (prob.map[0][col] == Terrain::OPEN)
    {
      assert (prob.entrance.col == 10000);
      prob.entrance.col = col;
    }
    else { assert (prob.map[0][col] == Terrain::WALL); }
  }
  assert (prob.entrance.col != 10000);

  prob.exit.row = prob.map.size () - 1;
  prob.exit.col = 10000;
  Number lastRow = prob.map.size () - 1;
  for (Number col = 0; col < prob.map[lastRow].size (); ++col)
  {
    if (prob.map[lastRow][col] == Terrain::OPEN)
    {
      assert (prob.exit.col == 10000);
      prob.exit.col = col;
    }
    else { assert (prob.map[lastRow][col] == Terrain::WALL); }
  }
  assert (prob.entrance.col != 10000);

  Number lastCol = prob.map[0].size () - 1;
  for (Number row = 0; row < lastRow; ++row)
  {
    assert (prob.map[row].size () == lastCol + 1);
    assert (prob.map[row][0] == Terrain::WALL);
    assert (prob.map[row][lastCol] == Terrain::WALL);
  }

  for (Number row = 1; row < lastRow; ++row)
  {
    for (Number col = 1; col < lastCol; ++col)
    {
      assert (prob.map[row][col] == Terrain::OPEN);
    }
  }

  return prob;
}

int
main (int args, char* argv[])
{
  Problem prob = getInput ();
}

// My answers:
// Part 1: 
// Part 2: 
