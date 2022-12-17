/// \file 2022Day15.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-15.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>

using Number = long;
// x first, then y
using Point = std::pair<Number, Number>;

struct Sensor
{
  Point m_sensorLoc;
  Point m_beaconLoc;
};

std::vector<Sensor>
getInput ()
{
  std::vector<Sensor> input;
  std::string line;
  while (std::getline (std::cin, line))
  {
    if (line.empty ()) { break; }
    Number a, b, c, d;
    int count = sscanf (line.c_str (), "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld", &a, &b, &c, &d);
    assert (count == 4);
    input.push_back ({ {a, b}, {c, d}});
  }
  return input;
}

Number mhDist (const Point& a, const Point& b)
{
  return abs (a.first - b.first) + abs (a.second - b.second);
}

bool
couldHaveHiddenBeacon (const std::vector<Sensor>& input, const Point& loc)
{
  for (const Sensor& sensor : input)
  {
    if (sensor.m_beaconLoc.first == loc.first && sensor.m_beaconLoc.second == loc.second) { return false; }
    Number distToBeacon = mhDist (sensor.m_sensorLoc, sensor.m_beaconLoc);
    Number distToLoc = mhDist (sensor.m_sensorLoc, loc);
    if (distToLoc <= distToBeacon) { return false; }
  }
  return true;
}

Number
minX (const std::vector<Sensor>& input)
{
  Number min = std::min (input[0].m_sensorLoc.first, input[0].m_beaconLoc.first);
  for (std::size_t index = 1; index < input.size (); ++index)
  {
    min = std::min (min, input[index].m_sensorLoc.first);
    min = std::min (min, input[index].m_beaconLoc.first);
  }
  return min;
}

Number
maxX (const std::vector<Sensor>& input)
{
  Number max = std::max (input[0].m_sensorLoc.first, input[0].m_beaconLoc.first);
  for (std::size_t index = 1; index < input.size (); ++index)
  {
    max = std::max (max, input[index].m_sensorLoc.first);
    max = std::max (max, input[index].m_beaconLoc.first);
  }
  return max;
}

Number
countCantBeBeaconSpots (const std::vector<Sensor>& input, Number row)
{
  int min = minX (input);
  int max = maxX (input);
  Number count = 0;
  int x = min;
  while (!couldHaveHiddenBeacon (input, {x, row}))
  {
    ++count;
    --x;
  }
  x = max;
  while (!couldHaveHiddenBeacon (input, {x, row}))
  {
    ++count;
    ++x;
  }
  x = min + 1;
  while (x < max)
  {
    if (!couldHaveHiddenBeacon (input, {x, row})) { ++count; }
    ++x;
  }
  return count;
}

Number
part2 (const std::vector<Sensor>& input)
{
  for (Number x = 0; x <= 4000000; ++x)
  {
    for (Number y = 0; y <= 4000000; ++y)
    {
      if (couldHaveHiddenBeacon (input, {x, y}))
      {
        return y * 4000000 * x + y;
      }
    }
    std::cout << ".";
    std::cout.flush ();
  }
  return 0;
}

/* Notes to figure out a general solution.
Scenario 1: checking a spot closer than the beacon above and to left
  Specifics: Sensor at (20, 40); Checking (15, 35); beacon 12 units away
    Last covered spot in column 15 should be at row 47
Senario 2: checking a spot closer than the beacon below and to left
  Specifics: Sensor at (30, 50); Checking (26, 52); beacon 8 units away
    Last covered spot in column 26 should be 54
*/

Number
part2Again (const std::vector<Sensor>& input)
{
  //const Number MAX = 20;
  const Number MAX = 4000000;
  for (Number x = 0; x <= MAX; ++x)
  {
    for (Number y = 0; y <= MAX; )
    {
      Number yPlus = 0;
      for (const Sensor& sense : input)
      {
        Number distToCurrent = mhDist (sense.m_sensorLoc, {x, y});
        Number distToBeacon = mhDist (sense.m_sensorLoc, sense.m_beaconLoc);
        //std::cout << "Trying (" << x << ", " << y << ")\n";
        //std::cout << "Sensor at (" << sense.m_sensorLoc.first << ", " << sense.m_sensorLoc.second << ") is " << distToCurrent << " away.\n";
        //std::cout << "Beacon at (" << sense.m_beaconLoc.first << ", " << sense.m_beaconLoc.second << ") is " << distToBeacon << " away from it\n";
        if (distToCurrent <= distToBeacon)
        {
          // This sensor is proof that this location will not work.
          // How far over we are from the sensor.
          Number xDistToSensor = (Number)abs (sense.m_sensorLoc.first - x);
          // How far we want to be down from the sensor.
          Number desiredYDiff = distToBeacon - xDistToSensor;
          // Where that would be
          Number desiredYLoc = sense.m_sensorLoc.second + desiredYDiff;
          // How much y needs to change to get there.
          yPlus = std::max (yPlus, (desiredYLoc - y) + 1);
          //std::cout << "New yPlus is " << yPlus << "\n";
        }
      }
      if (yPlus == 0)
      {
        //std::cout << x << " " << y << "\n";
        return 4000000 * x + y;
      }
      else { y += yPlus; }
    }
    std::cout << "Finished " << x * 100.0 / MAX << "% of search.\n";
  }
  return 0;
}

int main () {
  std::vector<Sensor> input = getInput ();
  //std::cout << countCantBeBeaconSpots (input, 2000000) << "\n";
  std::cout << part2Again (input) << "\n";
  return 0;
}
