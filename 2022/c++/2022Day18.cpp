/// \file 2022Day18.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-18.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>
#include <queue>

struct Coord3D
{
  int m_x;
  int m_y;
  int m_z;
};

// First index = x-coord, second = y-coord, members of set are z-coords.
using Map = std::map<int, std::map<int, std::set<int>>>;
// x-coord, y-coord, z-coord, number of sides touching surface
using MapWithCounts = std::map<int, std::map<int, std::map<int, int>>>;

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

MapWithCounts
findSurfaceCubes (const std::vector<Coord3D>& cubes, const Map& map)
{
  MapWithCounts result;
  for (const Coord3D& cube : cubes)
  {
    std::vector<Coord3D> neighbors = 
      { {cube.m_x, cube.m_y, cube.m_z - 1},
        {cube.m_x, cube.m_y, cube.m_z + 1},
        {cube.m_x, cube.m_y - 1, cube.m_z},
        {cube.m_x, cube.m_y + 1, cube.m_z},
        {cube.m_x - 1, cube.m_y, cube.m_z},
        {cube.m_x + 1, cube.m_y, cube.m_z} };
    for (const Coord3D& neighbor : neighbors)
    {
      if (!exists (neighbor, map))
      {
        if (result.count (neighbor.m_x) == 0) { result[neighbor.m_x] = {}; }
        if (result.at (neighbor.m_x).count (neighbor.m_y) == 0) { result[neighbor.m_x][neighbor.m_y] = {}; }
        if (result.at (neighbor.m_x).at (neighbor.m_y).count (neighbor.m_z) == 0) { result[neighbor.m_x][neighbor.m_y][neighbor.m_z] = 0; }
        ++result.at (neighbor.m_x).at (neighbor.m_y).at (neighbor.m_z);
      }
    }
  }
  return result;
}

unsigned long
countSurfaceTouches (const MapWithCounts& neighbors)
{
  unsigned long result = 0;
  for (const auto& xMap : neighbors)
  {
    for (const auto& yMap : xMap.second)
    {
      for (const auto& zMap : yMap.second)
      {
        result += zMap.second;
      }
    }
  }
  return result;
}

MapWithCounts
keepExternal (const MapWithCounts& neighbors, const Map& map)
{
  const int MIN_COORD = -1;
  const int MAX_COORD = 22;

  for (const auto& xMap : neighbors)
  {
    assert (xMap.first >= MIN_COORD && xMap.first <= MAX_COORD);
    for (const auto& yMap : xMap.second)
    {
      assert (yMap.first >= MIN_COORD && yMap.first <= MAX_COORD);
      for (const auto& zMap : yMap.second)
      {
        assert (zMap.first >= MIN_COORD && zMap.first <= MAX_COORD);
      }
    }
  }
  Map externals;
  std::queue<Coord3D> frontier;
  frontier.push ({MIN_COORD, MIN_COORD, MIN_COORD});
  while (!frontier.empty ())
  {
    Coord3D current = frontier.front ();
    frontier.pop ();
    if (!exists (current, map) && !exists (current, externals))
    {
      if (externals.count (current.m_x) == 0) { externals[current.m_x] = {}; }
      if (externals.at (current.m_x).count (current.m_y) == 0) { externals[current.m_x][current.m_y] = {}; }
      externals[current.m_x][current.m_y].insert (current.m_z);
      std::vector<Coord3D> nearby = 
        { {current.m_x, current.m_y, current.m_z - 1},
          {current.m_x, current.m_y, current.m_z + 1},
          {current.m_x, current.m_y - 1, current.m_z},
          {current.m_x, current.m_y + 1, current.m_z},
          {current.m_x - 1, current.m_y, current.m_z},
          {current.m_x + 1, current.m_y, current.m_z} };
      for (const auto& neighbor : nearby)
      {
        if (neighbor.m_x >= MIN_COORD && neighbor.m_y >= MIN_COORD && neighbor.m_z >= MIN_COORD && neighbor.m_x <= MAX_COORD && neighbor.m_y <= MAX_COORD && neighbor.m_z <= MAX_COORD)
        {
          frontier.push (neighbor);
        }
      }
    }
  }
  MapWithCounts results;
  for (const auto& xMap : neighbors)
  {
    for (const auto& yMap : xMap.second)
    {
      for (const auto& zMap : yMap.second)
      {
        Coord3D potential {xMap.first, yMap.first, zMap.first};
        if (exists (potential, externals))
        {
          if (results.count (potential.m_x) == 0) { results[potential.m_x] = {}; }
          if (results.at (potential.m_x).count (potential.m_y) == 0) { results[potential.m_x][potential.m_y] = {}; }
          results[potential.m_x][potential.m_y][potential.m_z] = zMap.second;
        }
      }
    }
  }
  return results;
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
  MapWithCounts surfaceCubes = findSurfaceCubes (cubes, map);
  std::cout << countSurfaceTouches (surfaceCubes) << "\n";
  MapWithCounts externalSurfaceCubes = keepExternal (surfaceCubes, map);
  std::cout << countSurfaceTouches (externalSurfaceCubes) << "\n";
  return 0;
}

// My answers:
// Part 1: 4302
// Part 2: 2492
// I probably created myself a lot of unnecessary headaches by using this complicated data structure.