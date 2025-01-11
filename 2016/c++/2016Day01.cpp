/// \file 2016Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-01.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <set>

struct Instruction
{
  char turn;
  int walk;
};

struct Position
{
  int row;
  int col;
  auto operator<=> (const Position& p) const = default;
};

std::vector<Instruction>
readInput ()
{
  std::vector<Instruction> instructions;
  std::string word;
  while (std::cin >> word) {
    if (word.ends_with (",")) { word = word.substr (0, word.size () - 1); }
    assert (word[0] == 'R' || word[0] == 'L');
    int i = atoi (word.substr (1).c_str ());
    assert (i > 0);
    instructions.push_back ({word[0], i});
  }
  return instructions;
}

Position
followInstructions (const std::vector<Instruction>& instructions, bool stopAtRepeated = false)
{
  Position current ({0, 0});
  std::set<Position> visited;
  visited.insert (current);
  char facing = 'N';
  for (const Instruction& inst : instructions) {
    if (inst.turn == 'R') {
      if (facing == 'N') { facing = 'E'; }
      else if (facing == 'E') { facing = 'S'; }
      else if (facing == 'S') { facing = 'W'; }
      else if (facing == 'W') { facing = 'N'; }
      else { throw "Impossible"; }
    }
    else {
      if (facing == 'N') { facing = 'W'; }
      else if (facing == 'W') { facing = 'S'; }
      else if (facing == 'S') { facing = 'E'; }
      else if (facing == 'E') { facing = 'N'; }
      else { throw "Impossible"; }
    }
    for (int i = 0; i < inst.walk; ++i) {
      if (facing == 'N') { --current.row; }
      else if (facing == 'E') { ++current.col; }
      else if (facing == 'S') { ++current.row; }
      else if (facing == 'W') { --current.col; }
      else { throw "Impossible"; }
      if (stopAtRepeated && visited.contains (current)) { return current; }
      visited.insert (current);
    }
  }
  return current;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Instruction> instructions = readInput ();
  assert (followInstructions ({{'R', 2}, {'L', 3}}) == Position ({-3, 2}));
  Position after = followInstructions (instructions);
  std::cout << std::abs (after.row) + std::abs (after.col) << "\n";
  assert (followInstructions ({{'R', 8}, {'R', 4}, {'R', 4}, {'R', 8}}, true) == Position ({0, 4}));
  Position after2 = followInstructions (instructions, true);
  std::cout << std::abs (after2.row) + std::abs (after2.col) << "\n";
  return 0;
}
