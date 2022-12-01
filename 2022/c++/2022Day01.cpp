/// \file 2022Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-01.

#include <iostream>
#include <vector>
#include <string>

/// \brief Reads the problem input.
/// \return A vector in which each element contains the collection of foodstuffs carried by one elf.
/// Knowing now what part 2 is about I see that I could have just done the summing here and made my life much easier.
std::vector<std::vector<int>>
getInput ()
{
  std::vector<std::vector<int>> elves;
  elves.push_back ({});
  std::string line;
  while (std::cin)
  {
    std::getline (std::cin, line);
    if (line == "")
    {
      elves.push_back ({});
    }
    else
    {
      elves.back ().push_back (atoi (line.c_str ()));
    }
  }
  elves.pop_back ();
  return elves;
}

/// \brief Finds the elf carrying the most calories.
/// \param[in] elves The problem input.
/// \return The total calories carried by the elf with the mostest.
int largestSum (const std::vector<std::vector<int>>& elves)
{
  int largest = 0;
  for (const auto& elf : elves)
  {
    int total = 0;
    for (int x : elf)
    {
      total += x;
    }
    largest = std::max (largest, total);
  }
  return largest;
}

/// \brief Finds the three elves carrying the most calories.
/// \param[in] elves The problem input.
/// \return The total calories carried by the three elves with the mostest.
/// This is disgustingly non-general, but I could write it very quickly.
int top3Sums (const std::vector<std::vector<int>>& elves)
{
  int largest = 0;
  int second = 0;
  int third = 0;
  for (const auto& elf : elves)
  {
    int total = 0;
    for (int x : elf)
    {
      total += x;
    }
    if (total > largest)
    {
      third = second;
      second = largest;
      largest = total;
    }
    else if (total > second)
    {
      third = second;
      second = total;
    }
    else if (total > third)
    {
      third = total;
    }
  }
  return largest + second + third;
}


/// \brief Runs the program.
/// \return Always 0.
int main () {
  std::vector<std::vector<int>> elves = getInput ();
  std::cout << largestSum (elves) << "\n";
  std::cout << top3Sums (elves) << "\n";
  return 0;
}
