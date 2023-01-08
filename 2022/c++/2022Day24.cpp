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

using Number = unsigned int;

enum class Terrain { WALL, OPEN };

enum class Direction { NORTH, EAST, SOUTH, WEST };

enum class Move { UP, DOWN, LEFT, RIGHT, WAIT };

struct Location
{
  Number row;
  Number col;
};

bool operator== (const Location& l1, const Location& l2)
{
  return l1.row == l2.row && l1.col == l2.col;
}

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
  std::vector<Move> path;
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
    assert (prob.map[row].size () == (std::size_t)(lastCol + 1));
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

void
moveBlizzards (const Problem& prob, State& state)
{
  for (Blizzard& bliz : state.blizzards)
  {
    if (bliz.facing == Direction::NORTH)
    {
      if (bliz.where.row == 1) { bliz.where.row = prob.map.size () - 2; }
      else { --bliz.where.row; }
    }
    else if (bliz.facing == Direction::EAST)
    {
      if (bliz.where.col == prob.map.at (0).size () - 2) { bliz.where.col = 1; }
      else { ++bliz.where.col; }
    }
    else if (bliz.facing == Direction::SOUTH)
    {
      if (bliz.where.row == prob.map.size () - 2) { bliz.where.row = 1; }
      else { ++bliz.where.row; }
    }
    else
    {
      if (bliz.where.col == 1) { bliz.where.col = prob.map.at (0).size () - 2; }
      else { --bliz.where.col; }
    }
  }
}

bool
hasNoBlizzards (const State& state, const Location& loc)
{
  for (const Blizzard& bliz : state.blizzards)
  {
    if (bliz.where == loc) { return false; }
  }
  return true;
}

State
bfsSearch (const Problem& prob)
{
  State initial {prob.entrance, prob.initialBlizzards, {}};
  std::list<State> frontier;
  frontier.push_back (initial);
  while (true)
  {
    assert (!frontier.empty ());
    State current = frontier.front ();
    frontier.pop_front ();
    moveBlizzards (prob, current);
    std::vector<std::pair<Location, Move>> neighbors;
    if (current.expedition.row > 0)
    {
      neighbors.push_back ({{current.expedition.row - 1, current.expedition.col}, Move::UP});
    }
    if (current.expedition.row < prob.map.size () - 1)
    {
      neighbors.push_back ({{current.expedition.row + 1, current.expedition.col}, Move::DOWN});
    }
    neighbors.push_back ({{current.expedition.row, current.expedition.col + 1}, Move::RIGHT});
    neighbors.push_back ({{current.expedition.row, current.expedition.col - 1}, Move::LEFT});
    neighbors.push_back ({current.expedition, Move::WAIT});
    for (const auto& pair : neighbors)
    {
      if (prob.map[pair.first.row][pair.first.col] == Terrain::OPEN && hasNoBlizzards (current, pair.first))
      {
        State successor {pair.first, current.blizzards, current.path};
        successor.path.push_back (pair.second);
        if (successor.expedition == prob.exit) { return successor; }
        else { frontier.push_back (successor); }
      }
    }
  }
}

int
main (int args, char* argv[])
{
  Problem prob = getInput ();
  State fastest = bfsSearch (prob);
  std::cout << fastest.path.size () << "\n";
}

// My answers:
// Part 1: 
// Part 2: 
