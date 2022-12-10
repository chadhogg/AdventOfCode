/// \file 2022Day09.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-09.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <set>

struct Move
{
  char m_dir;
  int m_amount;
};

struct Position
{
  int m_x;
  int m_y;
};

bool
operator< (const Position& p1, const Position& p2)
{
  if (p1.m_x < p2.m_x) { return true; }
  if (p1.m_x == p2.m_x && p1.m_y < p2.m_y) { return true; }
  return false;
}

std::vector<Move>
getInput ()
{
  std::vector<Move> moves;
  while (std::cin)
  {
    char d;
    int a;
    std::cin >> d >> a;
    assert (d == 'R' || d == 'U' || d == 'L' || d == 'D');
    assert (a > 0);
    moves.push_back ({d, a});
    //std::cout << d << " " << a << "\n";
  }
  moves.pop_back ();
  return moves;
}

void
applyMove (char dir, std::vector<Position>& knots)
{
  switch (dir)
  {
    case 'D': --knots[0].m_y; break;
    case 'U': ++knots[0].m_y; break;
    case 'L': --knots[0].m_x; break;
    case 'R': ++knots[0].m_x; break;
    default: assert (false);
  }

  for (std::size_t index = 1; index < knots.size (); ++index)
  {
    Position& current = knots[index];
    Position& prev = knots[index - 1];    
    if (prev.m_x == current.m_x)
    {
      if (prev.m_y - current.m_y > 1) { ++current.m_y; }
      else if (current.m_y - prev.m_y > 1) { --current.m_y; }
    }
    else if (prev.m_y == current.m_y)
    {
      if (prev.m_x - current.m_x > 1) { ++current.m_x; }
      else if (current.m_x - prev.m_x > 1) { --current.m_x; }
    }
    else if (abs (prev.m_x - current.m_x) > 1 || abs (prev.m_y - current.m_y) > 1)
    {
      if (prev.m_y > current.m_y) { ++current.m_y; }
      else { --current.m_y; }
      if (prev.m_x > current.m_x) { ++current.m_x; }
      else { --current.m_x; }
    }
  }

}

std::size_t
countUniqueEndPositions (const std::vector<Move>& moves, std::size_t numKnots)
{
  std::vector<Position> knots (numKnots, {0, 0});
  std::set<Position> tailLog;
  tailLog.insert (knots.back ());
  for (const Move& move : moves)
  {
    for (int count = 0; count < move.m_amount; ++count)
    {
      applyMove (move.m_dir, knots);
      tailLog.insert (knots.back ());
    }
  }
  return tailLog.size ();
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  std::vector<Move> moves = getInput ();
  std::cout << countUniqueEndPositions (moves, 2) << "\n";
  std::cout << countUniqueEndPositions (moves, 10) << "\n";
  return 0;
}
