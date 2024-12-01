/// \file 2024Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-01.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using List = std::vector<int>;
using Problem = std::pair<List, List>;

/// @brief Reads the input into a usable form.
/// @return A pair of equally-sized lists of numbers.
Problem
getInput ()
{
  Problem p;
  int x, y;
  std::cin >> x >> y;
  while (std::cin) {
    p.first.push_back (x);
    p.second.push_back (y);
    std::cin >> x >> y;
  }
  return p;
}

/// @brief Sums how far the n-th smallest element in one list is from the n-th smallest in the other.
/// @param p The problem.
/// @return The difference between the lists.
long
distanceBetweenLists (const Problem& p)
{
  List l1 = p.first;
  List l2 = p.second;
  long diff = 0;
  std::sort (l1.begin (), l1.end ());
  std::sort (l2.begin (), l2.end ());
  for (unsigned int i = 0; i < l1.size (); ++i) {
    diff += std::abs (l1.at (i) - l2.at (i));
  }
  return diff;
}

/// @brief Sums the product of each element in left list with count of times it appears in right list.
/// @param p The problem.
/// @return The lists' similarity score.
long
similarityScoreOfLists (const Problem& p)
{
  List l1 = p.first;
  List l2 = p.second;
  long score = 0;
  for (unsigned int i = 0; i < l1.size (); ++i) {
    int val = l1.at (i);
    int count = 0;
    // Premature optimization is the root of not getting done before other people.
    for (unsigned int j = 0; j < l2.size (); ++j) {
      if (l2.at (j) == val) { ++count; }
    }
    score += val * count;
  }
  return score;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Problem p = getInput ();
  std::cout << distanceBetweenLists (p) << "\n";
  std::cout << similarityScoreOfLists (p) << "\n";
  return 0;
}
