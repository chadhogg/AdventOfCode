/// \file 2016Day13.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-13.


#include <iostream>
#include <string>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <list>
#include <map>

using Number = unsigned long long;
Number g_Favorite;

struct Coordinate
{
  Number x;
  Number y;

  auto operator<=> (const Coordinate& other) const = default;

  bool
  isOpenSpace () const
  {
    Number result = x * x + 3 * x + 2 * x * y + y + y * y + g_Favorite;
    std::bitset<32> bs (result);
    return (bs.count () % 2 == 0);
  }

};

std::size_t
stepsTo (const Coordinate& start, const Coordinate& end)
{
  assert (start.isOpenSpace () && end.isOpenSpace ());
  std::list<Coordinate> frontier;
  std::map<Coordinate, std::size_t> steps;
  frontier.push_back (start);
  steps[start] = 0;
  while (!frontier.empty ()) {
    Coordinate current = frontier.front ();
    frontier.pop_front ();
    assert (steps.contains (current));
    //std::cout << "Considering (" << current.x << ", " << current.y << ") [" << steps[current] << "]\n";
    if (current == end) {
      return steps[current];
    }
    std::vector<Coordinate> neighbors;
    if (current.x > 0) {
      neighbors.push_back ({current.x - 1, current.y});
    }
    if (current.y > 0) {
      neighbors.push_back ({current.x, current.y - 1});
    }
    neighbors.push_back ({current.x + 1, current.y});
    neighbors.push_back ({current.x, current.y + 1});
    for (const Coordinate& neighbor : neighbors) {
      if (neighbor.isOpenSpace ()) {
        if (steps.contains (neighbor)) {
          assert (steps[neighbor] <= steps[current] + 1);
        }
        else {
          steps[neighbor] = steps[current] + 1;
          frontier.push_back (neighbor);
        }
      }
    }
  }
  return 0;
}


std::size_t
reachableIn (const Coordinate& start, std::size_t maxSteps)
{
  assert (start.isOpenSpace ());
  std::list<Coordinate> frontier;
  std::map<Coordinate, std::size_t> steps;
  frontier.push_back (start);
  steps[start] = 0;
  while (!frontier.empty ()) {
    Coordinate current = frontier.front ();
    frontier.pop_front ();
    assert (steps.contains (current));
    //std::cout << "Considering (" << current.x << ", " << current.y << ") [" << steps[current] << "]\n";
    if (steps[current] == maxSteps) { continue; }
    std::vector<Coordinate> neighbors;
    if (current.x > 0) {
      neighbors.push_back ({current.x - 1, current.y});
    }
    if (current.y > 0) {
      neighbors.push_back ({current.x, current.y - 1});
    }
    neighbors.push_back ({current.x + 1, current.y});
    neighbors.push_back ({current.x, current.y + 1});
    for (const Coordinate& neighbor : neighbors) {
      if (neighbor.isOpenSpace ()) {
        if (steps.contains (neighbor)) {
          assert (steps[neighbor] <= steps[current] + 1);
        }
        else {
          steps[neighbor] = steps[current] + 1;
          frontier.push_back (neighbor);
        }
      }
    }
  }
  return steps.size ();
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::cin >> g_Favorite;
  //g_Favorite = 10;
  Coordinate start = {1, 1};
  Coordinate end = {31, 39};
  //end = {7, 4};
  std::cout << stepsTo (start, end) << "\n";
  std::cout << reachableIn (start, 50) << "\n";
  return 0;
}

