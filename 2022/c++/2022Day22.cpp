/// \file 2022Day22.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-22.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

const char OPEN = '.';
const char WALL = '#';
const char VOID = ' ';
const char RIGHT = 'R';
const char LEFT = 'L';
const char FACE_NORTH = '^';
const char FACE_EAST = '>';
const char FACE_SOUTH = 'v';
const char FACE_WEST = '<';

using Map = std::vector<std::vector<char>>;

using Action = std::pair<unsigned, char>;

struct Problem
{
  Map map;
  std::vector<Action> actions;
};

struct State
{
  std::size_t row;
  std::size_t col;
  char facing;
  std::size_t nextAction;
};

Problem
getInput ()
{
  // Note: I add a border of VOID around the whole thing, so we never have to consider moving off the edge.
  Problem prob;
  std::string line;
  std::getline (std::cin, line);
  std::size_t maxWidth = 0;
  while (line != "")
  {
    std::vector<char> row;
    row.reserve (line.size ());
    row.push_back (VOID);
    for (char c : line)
    {
      assert (c == OPEN || c == WALL || c == VOID);
      row.push_back (c);
    }
    row.push_back (VOID);
    maxWidth = std::max (maxWidth, row.size ());
    prob.map.push_back (row);
    std::getline (std::cin, line);
  }
  std::vector<char> blank (maxWidth, VOID);
  prob.map.insert (prob.map.begin (), blank);
  prob.map.push_back (blank);
  for (auto& row : prob.map)
  {
    while (row.size () < maxWidth) { row.push_back (VOID); }
  }
  std::getline (std::cin, line);
  std::size_t start, end;
  start = 0;
  end = line.find_first_of ("LR", start);
  while (end != std::string::npos)
  {
    prob.actions.push_back ({atoi (line.substr (start, end - start).c_str ()), line[end]});
    start = end + 1;
    end = line.find_first_of ("LR", start);
  }
  prob.actions.push_back ({atoi (line.substr (start).c_str ()), '\0'});
  return prob;
}

void
move (const Problem& prob, State& current)
{
  if (current.facing == FACE_NORTH)
  {
    std::size_t newRow = current.row - 1;
    if (prob.map[newRow][current.col] == VOID)
    {
      newRow = prob.map.size () - 1;
      while (prob.map[newRow][current.col] == VOID) { --newRow; }
    }
    assert (prob.map[newRow][current.col] == OPEN || prob.map[newRow][current.col] == WALL);
    if (prob.map[newRow][current.col] == OPEN)
    {
      current.row = newRow;
    }
  }
  else if (current.facing == FACE_EAST)
  {
    std::size_t newCol = current.col + 1;
    if (prob.map[current.row][newCol] == VOID)
    {
      newCol = 0;
      while (prob.map[current.row][newCol] == VOID) { ++newCol; }
    }
    assert (prob.map[current.row][newCol] == OPEN || prob.map[current.row][newCol] == WALL);
    if (prob.map[current.row][newCol] == OPEN)
    {
      current.col = newCol;
    }
  }
  else if (current.facing == FACE_SOUTH)
  {
    std::size_t newRow = current.row + 1;
    if (prob.map[newRow][current.col] == VOID)
    {
      newRow = 0;
      while (prob.map[newRow][current.col] == VOID) { ++newRow; }
    }
    assert (prob.map[newRow][current.col] == OPEN || prob.map[newRow][current.col] == WALL);
    if (prob.map[newRow][current.col] == OPEN)
    {
      current.row = newRow;
    }
  }
  else
  {
    assert (current.facing == FACE_WEST);
    std::size_t newCol = current.col - 1;
    if (prob.map[current.row][newCol] == VOID)
    {
      newCol = prob.map[current.row].size () - 1;
      while (prob.map[current.row][newCol] == VOID) { --newCol; }
    }
    assert (prob.map[current.row][newCol] == OPEN || prob.map[current.row][newCol] == WALL);
    if (prob.map[current.row][newCol] == OPEN)
    {
      current.col = newCol;
    }
  }
}

void
turn (State& current, char direction)
{
  assert (direction == RIGHT || direction == LEFT || direction == '\0');
  if (direction == RIGHT)
  {
    if (current.facing == FACE_NORTH) { current.facing = FACE_EAST; }
    else if (current.facing == FACE_EAST) { current.facing = FACE_SOUTH; }
    else if (current.facing == FACE_SOUTH) { current.facing = FACE_WEST; }
    else { current.facing = FACE_NORTH; }
  }
  else if (direction == LEFT)
  {
    if (current.facing == FACE_NORTH) { current.facing = FACE_WEST; }
    else if (current.facing == FACE_EAST) { current.facing = FACE_NORTH; }
    else if (current.facing == FACE_SOUTH) { current.facing = FACE_EAST; }
    else { current.facing = FACE_SOUTH; }
  }
}

State
followPath (const Problem& prob, const State& initial)
{
  State current = initial;
  while (current.nextAction != prob.actions.size ())
  {
    unsigned distance = prob.actions[current.nextAction].first;
    char direction = prob.actions[current.nextAction].second;
    for (unsigned x = 0; x < distance; ++x) { move (prob, current); }
    turn (current, direction);
    ++current.nextAction;
  }
  return current;
}

int
passwordFromState (const State& state)
{
  return 1000 * state.row + 4 * state.col + (state.facing == FACE_EAST ? 0 : state.facing == FACE_SOUTH ? 1 : state.facing == FACE_WEST ? 2 : 3);
}

int main ()
{
  Problem prob = getInput ();
  State afterPath = followPath (prob, {1, 1, FACE_EAST, 0});
  std::cout << passwordFromState (afterPath) << "\n";
  return 0;
}

// My answers:
// Part 1: 149250
// Part 2: 
