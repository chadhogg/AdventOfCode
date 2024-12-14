/// \file 2024Day14.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-14.


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <optional>
#include <chrono>
#include <thread>

using Number = int;

struct Point
{
  Number x;
  Number y;
};

struct Robot
{
  Point position;
  Point velocity;
};

std::ostream&
operator<<(std::ostream& out, const Robot& robot)
{
  out << "p=" << robot.position.x << "," << robot.position.y << " v=" << robot.velocity.x << "," << robot.velocity.y;
  return out;
}

using Problem = std::vector<Robot>;

using Grid = std::vector<std::vector<bool>>;

struct ProbAndGrid
{
  Problem prob;
  Grid grid;
};


void
printProblem (const Problem& prob)
{
  for (const Robot& robot: prob) {
    std::cout << robot << "\n";
  }
  std::cout << "\n";
}

Problem
readInput ()
{
  Problem prob;
  Robot next;
  while (scanf ("p=%d,%d v=%d,%d", 
                &next.position.x, &next.position.y, 
                &next.velocity.x, &next.velocity.y) == 4) {
    prob.push_back (next);
    scanf ("\n");
  }
  return prob;
}

Problem
afterXSeconds (const Problem& prob, int width, int height, int seconds)
{
  Problem result;
  for (const Robot& robot: prob) {
    Robot copy = robot;
    copy.position.x = (copy.position.x + seconds * copy.velocity.x) % width;
    if (copy.position.x < 0) { copy.position.x += width; }
    copy.position.y = (copy.position.y + seconds * copy.velocity.y) % height;
    if (copy.position.y < 0) { copy.position.y += height; }
    result.push_back (copy);
  }
  return result;
}

Number
computeSafetyFactor (const Problem& prob, int width, int height)
{
  Number tl = 0, tr = 0, bl = 0, br = 0;
  for (const Robot& robot: prob) {
    if (robot.position.x < width / 2 && robot.position.y < height / 2) { ++tl; }
    else if (robot.position.x < width / 2 && robot.position.y > height / 2) { ++bl; }
    else if (robot.position.x > width / 2 && robot.position.y < height / 2) { ++tr; }
    else if (robot.position.x > width / 2 && robot.position.y > height / 2) { ++br; }
  }
  return tl * tr * bl * br;
}

bool
containsHorizontalLine2 (const Grid& grid, unsigned int length)
{
  for (unsigned int y = 0; y < grid.size (); ++y) {
    for (unsigned int x = 0; x < grid.at (y).size () - length; ++x) {
      bool good = true;
      for (unsigned int plus = 0; good && plus < length; ++plus) {
        if (!grid.at (y).at(x + plus)) { good = false; }
      }
      if (good) { return true; }
    }
  }
  return false;
}

void
visualize2 (const Grid& grid)
{
  for (unsigned int y = 0; y < grid.size (); ++y) {
    for (unsigned int x = 0; x < grid.at (y).size (); ++x) {
      if (grid.at (y).at (x)) { std::cout << "*"; }
      else { std::cout << " "; }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void
seekTree (const Problem& prob, int width, int height)
{
  int iteration = 0;
  Problem current = prob;
  while (true) {
    current = afterXSeconds (current, width, height, 1);
    ++iteration;
    std::vector<bool> allZeroes;
    for (int i = 0; i < width; ++i) {
      allZeroes.push_back (false);
    }
    Grid grid;
    for (int i = 0; i < height; ++i) {
      grid.push_back (allZeroes);
    }
    for (const Robot& robot: current) {
      grid[robot.position.y][robot.position.x] = true;
    }
    std::cout << "Did " << iteration << " iterations.\n\n";
    if (containsHorizontalLine2 (grid, 8)) {
      visualize2 (grid);
      std::this_thread::sleep_for (std::chrono::seconds (5));
    }
  }
}


#define EXAMPLE false//true
const int WIDTH = (EXAMPLE ? 11 : 101);
const int HEIGHT = (EXAMPLE ? 7 : 103);

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  //printProblem (prob);
  Problem after100 = afterXSeconds (prob, WIDTH, HEIGHT, 100);
  //printProblem (after100);
  std::cout << computeSafetyFactor (after100, WIDTH, HEIGHT) << "\n";
  seekTree (prob, WIDTH, HEIGHT);
  return 0;
}
