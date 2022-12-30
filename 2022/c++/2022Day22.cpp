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
const char FACE_UP = '^';
const char FACE_RIGHT = '>';
const char FACE_DOWN = 'v';
const char FACE_LEFT = '<';

using Map = std::vector<std::vector<char>>;

using Action = std::pair<unsigned, char>;

struct Problem
{
  Map map;
  std::vector<Action> actions;
};

Problem
getInput ()
{
  Problem prob;
  std::string line;
  std::getline (std::cin, line);
  while (line != "")
  {
    std::vector<char> row;
    row.reserve (line.size ());
    for (char c : line)
    {
      assert (c == OPEN || c == WALL || c == VOID);
      row.push_back (c);
    }
    prob.map.push_back (row);
    std::getline (std::cin, line);
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

int main ()
{
  Problem prob = getInput ();
  return 0;
}

// My answers:
// Part 1: 
// Part 2: 
