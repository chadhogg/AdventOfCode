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

bool operator< (const Location& l1, const Location& l2)
{
  return l1.row < l2.row || (l1.row == l2.row && l1.col < l2.col);
}

struct Blizzard
{
  Direction facing;
  Location where;
};

bool
operator< (const Blizzard& b1, const Blizzard& b2)
{
  return b1.facing < b2.facing || (b1.facing == b2.facing && b1.where < b2.where);
}

using Blizzards = std::vector<Blizzard>;

struct Problem
{
  std::vector<std::vector<Terrain>> map;
  Location entrance;
  Location exit;
  Blizzards initialBlizzards;
};

struct State
{
  Location expedition;
  std::vector<Move> path;
};

bool
operator< (const State& s1, const State& s2)
{
  return s1.expedition.row < s2.expedition.row
    || (s1.expedition.row == s2.expedition.row && s1.expedition.col < s2.expedition.col)
    || (s1.expedition.row == s2.expedition.row && s1.expedition.col == s2.expedition.col && s1.path.size () < s2.path.size ());
}

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
moveBlizzards (const Problem& prob, Blizzards& all)
{
  for (Blizzard& bliz : all)
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
hasNoBlizzards (const Blizzards& all, const Location& loc)
{
  for (const Blizzard& bliz : all)
  {
    if (bliz.where == loc) { return false; }
  }
  return true;
}

void
draw (const Problem& prob, const State& state, const Blizzards& bliz)
{
  for (Number row = 0; row < prob.map.size (); ++row)
  {
    for (Number col = 0; col < prob.map[0].size (); ++col)
    {
      if (prob.map[row][col] == Terrain::WALL) { std::cout << "#"; }
      else if (state.expedition.row == row && state.expedition.col == col) { std::cout << "E"; }
      else if (hasNoBlizzards (bliz, {row, col})) { std::cout << "."; }
      else { std::cout << "*"; }
    }
    std::cout << "\n";
  }
  for (auto x : state.path)
  {
    if (x == Move::UP) { std::cout << "U "; }
    else if (x == Move::DOWN) { std::cout << "D "; }
    else if (x == Move::LEFT) { std::cout << "L "; }
    else if (x == Move::RIGHT) { std::cout << "R "; }
    else { std::cout << "W "; }
  }
  std::cout << "\n\n";
}

std::vector<Blizzards> blizCache;

State
bfsSearch (const Problem& prob)
{
  blizCache.clear ();
  blizCache.push_back (prob.initialBlizzards);
  std::set<State> stateCache;
  State initial {prob.entrance, {}};
  std::list<State> frontier;
  frontier.push_back (initial);
  stateCache.insert (initial);
  while (true)
  {
    assert (!frontier.empty ());
    State current = frontier.front ();
    frontier.pop_front ();
    if (current.path.size () >= blizCache.size ())
    {
      Blizzards currentBliz = blizCache[current.path.size () - 1];
      moveBlizzards (prob, currentBliz);
      blizCache.push_back (currentBliz);
    }
    const Blizzards& updatedBliz = blizCache[current.path.size ()];
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
      if (prob.map[pair.first.row][pair.first.col] == Terrain::OPEN && hasNoBlizzards (updatedBliz, pair.first))
      {
        State successor {pair.first, current.path};
        successor.path.push_back (pair.second);
        if (successor.expedition == prob.exit)
        {
          return successor;
        }
        else
        {
          if (stateCache.count (successor) == 0)
          {
            //draw (prob, successor, updatedBliz);
            stateCache.insert (successor);
            frontier.push_back (successor);
          }
        }
      }
    }
  }
}

int
main (int args, char* argv[])
{
  Problem prob = getInput ();
  State fastest = bfsSearch (prob);
  std::cout << fastest.path.size () - 1 << "\n";
  std::swap (prob.entrance, prob.exit);
  prob.initialBlizzards = blizCache.back ();
  State back = bfsSearch (prob);
  std::swap (prob.entrance, prob.exit);
  prob.initialBlizzards = blizCache.back ();
  State again = bfsSearch (prob);
  std::cout << fastest.path.size () - 1 << " + " << back.path.size () - 1 << " + " << again.path.size () - 1 << " = ";
  std::cout << fastest.path.size () + back.path.size () + again.path.size () - 3 << "\n";
}

// My answers:
// Part 1: 245
// Part 2: 
