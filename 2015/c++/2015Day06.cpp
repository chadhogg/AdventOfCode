/// \file 2015Day06.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-06.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <array>
#include <cassert>

enum ChangeType {OFF, ON, TOGGLE};

struct Position
{
  int row;
  int col;
};

struct Instruction
{
  ChangeType type;
  Position start;
  Position end;
};

std::vector<Instruction>
readInput ()
{
  std::vector<Instruction> instructions;
  std::string line;
  while (std::getline (std::cin, line)) {
    ChangeType type;
    std::string firstPair;
    std::string secondPair;
    if (line.substr (0, 8) == "turn off") {
      type = OFF;
      firstPair = line.substr (9, line.find (' ', 9));
      secondPair = line.substr (line.find ("through ") + 8);
    }
    else if (line.substr (0, 7) == "turn on") {
      type = ON;
      firstPair = line.substr (8, line.find (' ', 8));
      secondPair = line.substr (line.find ("through ") + 8);
    }
    else if (line.substr (0, 6) == "toggle") {
      type = TOGGLE;
      firstPair = line.substr (7, line.find (' ', 7));
      secondPair = line.substr (line.find ("through ") + 8);
    }
    else {
      break;
    }
    instructions.push_back (Instruction {type, 
    {atoi (firstPair.substr (0, firstPair.find (',')).c_str ()), atoi (firstPair.substr (firstPair.find (',') + 1).c_str ())},
    {atoi (secondPair.substr (0, secondPair.find (',')).c_str ()), atoi (secondPair.substr (secondPair.find (',') + 1).c_str ())}
    });
  }
  return instructions;
}

long
bruteForcePart1 (const std::vector<Instruction>& instructions)
{
  std::array<std::array<bool, 1000>, 1000> lights;
  for (int i = 0; i < 1000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      lights[i][j] = false;
    }
  }
  for (const Instruction& inst : instructions) {
    for (int row = std::min (inst.start.row, inst.end.row); row <= std::max (inst.start.row, inst.end.row); ++row) {
      for (int col = std::min (inst.start.col, inst.end.col); col <= std::max (inst.start.col, inst.end.col); ++col) {
        if (inst.type == ON) { lights[row][col] = true; }
        else if (inst.type == OFF) { lights[row][col] = false; }
        else { lights[row][col] = !lights[row][col]; }
      }
    }
  }
  long count = 0;
  for (int row = 0; row < 1000; ++row) {
    for (int col = 0; col < 1000; ++col) {
      if (lights[row][col]) { ++count; }
    }
  }
  return count;
}

long
bruteForcePart2 (const std::vector<Instruction>& instructions)
{
  std::array<std::array<long, 1000>, 1000> lights;
  for (int i = 0; i < 1000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      lights[i][j] = 0;
    }
  }
  for (const Instruction& inst : instructions) {
    for (int row = std::min (inst.start.row, inst.end.row); row <= std::max (inst.start.row, inst.end.row); ++row) {
      for (int col = std::min (inst.start.col, inst.end.col); col <= std::max (inst.start.col, inst.end.col); ++col) {
        if (inst.type == ON) { ++lights[row][col]; }
        else if (inst.type == OFF) { lights[row][col] = std::max (0L, lights[row][col] - 1); }
        else { lights[row][col] += 2; }
      }
    }
  }
  long count = 0;
  for (int row = 0; row < 1000; ++row) {
    for (int col = 0; col < 1000; ++col) {
      count += lights[row][col];
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Instruction> instructions = readInput ();
  std::cout << bruteForcePart1 (instructions) << "\n";
  std::cout << bruteForcePart2 (instructions) << "\n";
  return 0;
}

