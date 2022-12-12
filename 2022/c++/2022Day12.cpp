/// \file 2022Day12.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-12.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>

const char START = 'S';
const char END = 'E';
const int INVALID = -1;
const int MAX_HEIGHT_DIFF = 1;

using Height = char;
using HeightMap = std::vector<std::vector<Height>>;

struct Position
{
  int m_row;
  int m_col;
  //friend auto operator<=>(const Position&, const Position&) = default;
};

bool
operator< (const Position& x, const Position& y)
{
  return x.m_row < y.m_row || (x.m_row == y.m_row && x.m_col < y.m_col);
}

bool
operator== (const Position& x, const Position& y)
{
  return (x.m_row == y.m_row && x.m_col == y.m_col);
}

bool
operator!= (const Position& x, const Position& y)
{
  return !(x == y);
}


struct Problem
{
  HeightMap hm;
  Position start;
  Position end;
};

Problem
getInput ()
{
  Problem prob;
  prob.start = prob.end = {INVALID, INVALID};
  std::string line;
  while (std::getline (std::cin, line))
  {
    if (line.empty ()) { break; }
    prob.hm.push_back ({});
    for (char c : line)
    {
      if (c == START)
      {
        assert (prob.start.m_row == INVALID);
        prob.hm.back ().push_back ('a');
        prob.start = {(int)prob.hm.size () - 1, (int)prob.hm.back ().size () - 1};
      }
      else if (c == END)
      {
        assert (prob.end.m_row == INVALID);
        prob.hm.back ().push_back ('z');
        prob.end = {(int)prob.hm.size () - 1, (int)prob.hm.back ().size () - 1};
      }
      else
      {
        assert (islower (c));
        prob.hm.back ().push_back (c);
      }
    }
  }
  assert (prob.start.m_row >= 0 && prob.start.m_row < (int)prob.hm.size ());
  assert (prob.start.m_col >= 0 && prob.start.m_col < (int)prob.hm.front ().size ());
  assert (prob.end.m_row >= 0 && prob.end.m_row < (int)prob.hm.size ());
  assert (prob.end.m_col >= 0 && prob.end.m_col < (int)prob.hm.front ().size ());
  assert (prob.start.m_row != prob.end.m_row || prob.start.m_col != prob.end.m_col);
  return prob;
}

int
part1 (const Problem& prob)
{
  std::list<Position> frontier;
  std::map<Position, Position> parents;
  frontier.push_back (prob.start);
  parents[prob.start] = prob.start;
  while (!frontier.empty ())
  {
    Position current = frontier.front ();
    frontier.pop_front ();

    Position up = {current.m_row - 1, current.m_col};
    if (up.m_row >= 0 && prob.hm[up.m_row][up.m_col] - prob.hm[current.m_row][current.m_col] <= MAX_HEIGHT_DIFF)
    {
      if (parents.count (up) == 0)
      {
        parents[up] = current;
        frontier.push_back (up);
      }
    }

    Position down = {current.m_row + 1, current.m_col};
    if (down.m_row < (int)prob.hm.size () && prob.hm[down.m_row][down.m_col] - prob.hm[current.m_row][current.m_col] <= MAX_HEIGHT_DIFF)
    {
      if (parents.count (down) == 0)
      {
        parents[down] = current;
        frontier.push_back (down);
      }
    }

    Position left = {current.m_row, current.m_col - 1};
    if (left.m_col >= 0 && prob.hm[left.m_row][left.m_col] - prob.hm[current.m_row][current.m_col] <= MAX_HEIGHT_DIFF)
    {
      if (parents.count (left) == 0)
      {
        parents[left] = current;
        frontier.push_back (left);
      }
    }

    Position right = {current.m_row, current.m_col + 1};
    if (right.m_col < (int)prob.hm[0].size () && prob.hm[right.m_row][right.m_col] - prob.hm[current.m_row][current.m_col] <= MAX_HEIGHT_DIFF)
    {
      if (parents.count (right) == 0)
      {
        parents[right] = current;
        frontier.push_back (right);
      }
    }
  }

  // No path is available
  if (parents.count (prob.end) == 0) { return prob.hm.size () * prob.hm[0].size (); }

  int count = 0;
  Position current = prob.end;
  while (parents[current] != current)
  {
    ++count;
    current = parents[current];
  }
  return count;
}

// Sure, it would be much faster to run the search backwards from end to the closest 'a'.
// But we are optimizing for (developer time + cpu time), and N^2 isn't going to hurt the latter
//   as much as re-writing the algorithm would hurt the former.
int
part2 (Problem prob)
{
  int best = prob.hm.size () * prob.hm[0].size ();
  for (std::size_t row = 0; row < prob.hm.size (); ++row)
  {
    for (std::size_t col = 0; col < prob.hm[row].size (); ++col)
    {
      if (prob.hm[row][col] == 'a')
      {
        prob.start = {(int)row, (int)col};
        best = std::min (best, part1 (prob));
      }
    }
  }
  return best;
}

int main () {
  Problem prob = getInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";

  return 0;
}
