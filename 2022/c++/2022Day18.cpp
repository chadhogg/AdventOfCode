/// \file 2022Day18.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-18.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>

struct Coord3D
{
  int m_x;
  int m_y;
  int m_z;
};

// First index = x-coord, second = y-coord, members of set are z-coords.
using Map = std::map<int, std::map<int, std::set<int>>>;

std::vector<Coord3D>
getInput ()
{
  std::vector<Coord3D> cubes;
  int x, y, z;
  while (scanf ("%d,%d,%d", &x, &y, &z) == 3)
  {
    cubes.push_back ({x, y, z});
  }
  return cubes;
}

Map
buildMap (std::vector<Coord3D>& cubes)
{
  Map map;
  for (const Coord3D& cube : cubes)
  {
    if (map.count (cube.m_x) == 0) { map[cube.m_x] = {}; }
    if (map[cube.m_x].count (cube.m_y) == 0) { map[cube.m_x][cube.m_y] = {}; }
    assert (map[cube.m_x][cube.m_y].count (cube.m_z) == 0);
    map[cube.m_x][cube.m_y].insert (cube.m_z);
  }
  return map;
}

bool
exists (const Coord3D& cube, const Map& map)
{
  return map.count (cube.m_x) == 1
    && map.at (cube.m_x).count (cube.m_y) == 1
    && map.at (cube.m_x).at (cube.m_y).count (cube.m_z) == 1;
}

unsigned long
computeSurfaceArea (const std::vector<Coord3D>& cubes, const Map& map)
{
  unsigned long result = 0;
  for (const Coord3D& cube : cubes)
  {
    if (!exists ({cube.m_x, cube.m_y, cube.m_z - 1}, map)) { ++result; }
    if (!exists ({cube.m_x, cube.m_y, cube.m_z + 1}, map)) { ++result; }
    if (!exists ({cube.m_x, cube.m_y - 1, cube.m_z}, map)) { ++result; }
    if (!exists ({cube.m_x, cube.m_y + 1, cube.m_z}, map)) { ++result; }
    if (!exists ({cube.m_x - 1, cube.m_y, cube.m_z}, map)) { ++result; }
    if (!exists ({cube.m_x + 1, cube.m_y, cube.m_z}, map)) { ++result; }
  }
  return result;
}

int main () {
  std::vector<Coord3D> cubes = getInput ();
  Map map = buildMap (cubes);
  std::cout << computeSurfaceArea (cubes, map) << "\n";
  return 0;
}

// My answers:
// Part 1: 4302