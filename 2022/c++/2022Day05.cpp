/// \file 2022Day05.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-05.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <stack>

using Crate = char;
using Pile = std::stack<Crate>;

struct Move
{
  unsigned count;
  unsigned from;
  unsigned to;
};

struct Problem
{
  std::vector<Pile> piles;
  std::vector<Move> todo;
};

Problem
getInput ()
{
  Problem prob;
  std::vector<Pile> tempStacks;
  std::string line;
  // Read in the stacks.
  while (std::getline (std::cin, line))
  {
    if (line.size () >= 3 && line.substr (0, 3) == " 1 ")
    {
      std::getline (std::cin, line);
      assert (line == "");
      break;
    }

    std::size_t index = 0;
    while (index < line.size ())
    {
      if (line[index + 1] == ' ')
      {
        assert (line[index] == ' ' && line[index + 2] == ' ');
      }
      else
      {
        assert (line[index] == '[' && line[index + 2] == ']');
        assert (isupper (line[index + 1]));
        std::size_t stackNo = index / 4;
        while (stackNo >= tempStacks.size ())
        {
          tempStacks.push_back ({});
        }
        tempStacks[stackNo].push (line[index + 1]);
      }
      index += 4;
    }
  }
  // Reverse the stacks so that the bottom is actually at the bottom.
  for (std::size_t index = 0; index < tempStacks.size (); ++index)
  {
    prob.piles.push_back ({});
    while (!tempStacks[index].empty ())
    {
      prob.piles[index].push (tempStacks[index].top ());
      tempStacks[index].pop ();
    }
  }

  // Now read the instructions.
  while (std::getline (std::cin, line))
  {
    unsigned x, y, z;
    if (sscanf (line.c_str (), "move %d from %d to %d", &x, &y, &z) == 3)
    {
      prob.todo.push_back ({x, y, z});
    }
    else
    {
      assert (line == "");
    }
  }

  return prob;
}

std::string
formatAnswer (const std::vector<Pile>& piles)
{
  std::string result = "";
  for (const auto& pile : piles)
  {
    result += pile.top ();
  }
  return result;
}

std::string
part1 (Problem prob)
{
  for (std::size_t moveIndex = 0; moveIndex < prob.todo.size (); ++moveIndex)
  {
    Move& move = prob.todo[moveIndex];
    while (move.count > 0)
    {
      prob.piles[move.to - 1].push (prob.piles[move.from - 1].top ());
      prob.piles[move.from - 1].pop ();
      --move.count;
    }
  }
  return formatAnswer (prob.piles);
}

std::string
part2 (Problem prob)
{
  for (std::size_t moveIndex = 0; moveIndex < prob.todo.size (); ++moveIndex)
  {
    Move& move = prob.todo[moveIndex];
    Pile temp;
    while (move.count > 0)
    {
      temp.push (prob.piles[move.from - 1].top ());
      prob.piles[move.from - 1].pop ();
      --move.count;
    }
    while (!temp.empty ())
    {
      prob.piles[move.to - 1].push (temp.top ());
      temp.pop ();
    }
  }
  return formatAnswer (prob.piles);
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  Problem prob = getInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
