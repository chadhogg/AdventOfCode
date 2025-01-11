/// \file 2016Day02.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-02.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::string>
readInput ()
{
  std::vector<std::string> instructions;
  std::string line;
  while (std::cin >> line) {
    instructions.push_back (line);
  }
  return instructions;
}

struct Position
{
  int row;
  int col;

  char
  toKey () const
  {
    if (row == 0) {
      if (col == 0) { return '1'; }
      else if (col == 1) { return '2'; }
      else if (col == 2) { return '3'; }
    }
    else if (row == 1) {
      if (col == 0) { return '4'; }
      else if (col == 1) { return '5'; }
      else if (col == 2) { return '6'; }
    }
    else if (row == 2) {
      if (col == 0) { return '7'; }
      else if (col == 1) { return '8'; }
      else if (col == 2) { return '9'; }
    }
    throw "Impossible";
  }

  char
  toKey2 () const
  {
    if (row == 0) {
      if (col == 2) { return '1'; }
    }
    else if (row == 1) {
      if (col == 1) { return '2'; }
      else if (col == 2) { return '3'; }
      else if (col == 3) { return '4'; }
    }
    else if (row == 2) {
      if (col == 0) { return '5'; }
      else if (col == 1) { return '6'; }
      else if (col == 2) { return '7'; }
      else if (col == 3) { return '8'; }
      else if (col == 4) { return '9'; }
    }
    else if (row == 3) {
      if (col == 1) { return 'A'; }
      else if (col == 2) { return 'B'; }
      else if (col == 3) { return 'C'; }
    }
    else if (row == 4) {
      if (col == 2) { return 'D'; }
    }
    throw "Impossible";
  }

  bool
  isValid2 () const
  {
    if (row == 0 || row == 4) { return col == 2; }
    else if (row == 1 || row == 3) { return col >= 1 && col <= 3; }
    else if (row == 2) { return col >= 0 && col <= 4; }
    else { return false; }
  }
};

Position
followInstruction (const std::string& inst, const Position& start)
{
  Position current = start;
  for (char c : inst) {
    if (c == 'L') { --current.col; }
    else if (c == 'D') { ++current.row; }
    else if (c == 'R') { ++current.col; }
    else if (c == 'U') { --current.row; }
    else { throw "Impossible"; }
    current.row = std::max (current.row, 0);
    current.row = std::min (current.row, 2);
    current.col = std::max (current.col, 0);
    current.col = std::min (current.col, 2);
  }
  return current;
}

std::string
followInstructions (const std::vector<std::string>& instructions, const Position& start)
{
  Position current = start;
  std::string code;
  for (const std::string& inst : instructions) {
    current = followInstruction (inst, current);
    code += current.toKey ();
  }
  return code;
}

Position
followInstruction2 (const std::string& inst, const Position& start)
{
  Position current = start;
  for (char c : inst) {
    Position next = current;
    if (c == 'L') { --next.col; }
    else if (c == 'D') { ++next.row; }
    else if (c == 'R') { ++next.col; }
    else if (c == 'U') { --next.row; }
    else { throw "Impossible"; }
    if (next.isValid2 ()) { current = next; }
  }
  return current;
}

std::string
followInstructions2 (const std::vector<std::string>& instructions, const Position& start)
{
  Position current = start;
  std::string code;
  for (const std::string& inst : instructions) {
    current = followInstruction2 (inst, current);
    code += current.toKey2 ();
  }
  return code;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<std::string> instructions = readInput ();
  std::cout << followInstructions (instructions, {1, 1}) << "\n";
  std::cout << followInstructions2 (instructions, {2, 0}) << "\n";
  return 0;
}
