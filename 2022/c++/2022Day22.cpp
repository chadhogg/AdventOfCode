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

// My cube:
//     11112222
//     11112222
//     11112222
//     11112222
//     3333    
//     3333    
//     3333    
//     3333    
// 44445555    
// 44445555    
// 44445555    
// 44445555    
// 6666        
// 6666        
// 6666        
// From 1: moving N goes to 6 facing W. (1, 51) -> (201, 0) and (1, 100) -> (250, 0)
//           means X' = 150X + Y; Y' = 0
//         moving E goes to 2 facing E. (1, 100) -> (1, 100) and (50, 100) -> (50, 100)
//         moving S goes to 3 facing S. (50, 51) -> (50, 51) and (50, 100) -> (50, 100)
//         moving W goes to 4 facing E. (1, 51) -> (200, 0) and (50, 51) -> (151, 0)
//           means X' = 201 - X; Y' = 0
// From 2: moving N goes to 6 facing N. (1, 101) -> (251, 1) and (1, 150) -> (251, 50)
//           means X' = 251; Y' = Y - 100
//         moving E goes to 5 facing W. (1, 150) -> (200, 101) and (50, 150) -> (151, 101)
//           means X' = -X + 201/150; Y' = 101
//         moving S goes to 3 facing W. (50, 101) -> (51, 101) and (50, 150) -> (100, 101)
//           means X' = Y - 50; Y' = 101
//         moving W goes to 1 facing W. (1, 101) -> (1, 101) and (50, 101) -> (50, 101)
// From 3: moving N goes to 1 facing N. (51, 51) -> (51, 51) and (51, 100) -> (51, 100)
//         moving E goes to 2 facing N. (51, 100) -> (51, 101) and (100, 100) -> (51, 150)
//           means X' = 51; Y' = X + 50
//         moving S goes to 5 facing S. (100, 51) -> (150, 51) and (100, 100) -> (150, 100)
//           means X' = 150; Y' = Y
//         moving W goes to 4 facing S. (51, 51) -> (150, 1) and (200, 51) -> (150, 50)
//           means X' = 150; Y' = (200 - X)/149
// From 4: moving N goes to 3 facing E. (151, 1) -> (151, 50) and (151, 50) -> (100, 50)
//           means X' = (7450 - 51Y)/49; Y' = 50 
//         moving E goes to 5 facing E. (151, 50) -> (151, 50) and (200, 50) -> (200, 50)
//         moving S goes to 6 facing S. (200, 1) -> (200, 1) and (200, 50) -> (200, 50)
//         moving W goes to 1 facing E. (151, 1) -> (50, 50) and (200, 1) -> (1, 50)
//           means X' = -X + 201; Y' = 50
// From 5: moving N goes to 3 facing N. (151, 51) -> (101, 51) and (151, 100) -> (101, 100)
//           means X' = 101; Y' = Y
//         moving E goes to 2 facing W. (151, 100) -> (50, 151) and (200, 100) -> (1, 151)
//           means X' = -X + 201; Y' = 151
//         moving S goes to 6 facing W. (200, 51) -> (201, 51) and (200, 100) -> (250, 51)
//           means X' = Y + 150; Y' = 51
//         moving W goes to 4 facing W. (151, 51) -> (151, 51) and (200, 51) -> (200, 51)
// From 6: moving N goes to 4 facing N. (201, 1) -> (201, 1) and (201, 50) -> (201, 50)
//         moving E goes to 5 facing N. (201, 50) -> (201, 51) and (250, 50) -> (201, 100)
//           means X' = 201; Y' = X - 150
//         moving S goes to 2 facing S. (250, 1) -> (0, 101) and (250, 50) -> (0, 150)
//           means X' = 0; Y' = Y + 100
//         moving W goes to 1 facing S. (201, 1) -> (0, 51) and (250, 1) -> (0, 100)
//           means X' = 0; Y' = X - 150

// My answers:
// Part 1: 149250
// Part 2: 
