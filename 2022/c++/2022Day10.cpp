/// \file 2022Day10.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-10.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

struct AddInstruction
{
  int cyclesBeforeFinished;
  int value;
};

std::vector<AddInstruction>
getInput ()
{
  std::vector<AddInstruction> inst;
  std::string line;
  int noops = 0;
  while (std::getline (std::cin, line))
  {
    if (line == "") { break; }
    if (line == "noop") { ++noops; }
    else
    {
      assert (line.size () > 5 && line.substr (0, 5) == "addx ");
      inst.push_back ({noops + 1, atoi (line.substr (5).c_str ())});
      noops = 0;
    }
  }
  return inst;
}

std::vector<int>
getRegValues (const std::vector<AddInstruction>& problem)
{
  std::vector<int> result;
  // Adding one 0 to make "time 1" be at index 1.
  result.push_back (0);
  // Adding another 0 because I seem to have an off-by-one error that it fixes. :person_shrugging:
  result.push_back (0);
  int current = 1;
  for (const AddInstruction& inst : problem)
  {
    for (int count = 0; count < inst.cyclesBeforeFinished; ++count)
    {
      result.push_back (current);
    }
    current += inst.value;
    result.push_back (current);
  }
  return result;
}

int
part1 (const std::vector<int>& values)
{
  assert (values.size () >= 221);
  return values[20] * 20 + values[60] * 60 + values[100] * 100 + values[140] * 140 + values[180] * 180 + values[220] * 220;
}

void
part2 (const std::vector<int>& values)
{
  int index = 1;
  for (int rowNum = 0; rowNum < 6; ++rowNum)
  {
    for (int colNum = 0; colNum < 40; ++colNum)
    {
      if (index >= (int)values.size ()) { break; }
      if (abs (values[index] - colNum) < 2) { std::cout << "#"; }
      else { std::cout << "."; }
      ++index;
    }
    std::cout << "\n";
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  std::vector<AddInstruction> inst = getInput ();
  std::vector<int> values = getRegValues (inst);
  std::cout << part1 (values) << "\n";
  part2 (values);
  return 0;
}
