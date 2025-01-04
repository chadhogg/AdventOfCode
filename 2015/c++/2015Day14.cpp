/// \file 2015Day14.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-14.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <regex>
#include <list>
#include <set>
#include <algorithm>

struct Reindeer
{
  std::string name;
  int flySpeed;
  int flyTime;
  int restTime;
  long long distanceTraveled;
};

std::vector<Reindeer>
readInput ()
{
  std::vector<Reindeer> herd;
  std::basic_regex regex ("^(\\w+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\d+) seconds.$");
  std::string line;
  while (std::getline (std::cin, line)) {
    std::smatch match;
    if (std::regex_match (line, match, regex)) {
      assert (match.size () == 5);
      Reindeer rudolph ({match[1].str (), atoi (match[2].str ().c_str ()), atoi (match[3].str ().c_str ()), atoi (match[4].str ().c_str ()), 0LL});
      herd.push_back (rudolph);
    }
    else {
      std::cerr << line << " did not match.\n";
    }
  }
  return herd;
}

std::vector<Reindeer>
after (const std::vector<Reindeer>& pre, int seconds)
{
  std::vector<Reindeer> post = pre;
  for (Reindeer& deer : post) {
    int periodLength = deer.flyTime + deer.restTime;
    int fullPeriods = seconds / periodLength;
    int extraSeconds = seconds % periodLength;
    int extraSecondsFlying = std::min (deer.flyTime, extraSeconds);
    deer.distanceTraveled += (fullPeriods * deer.flyTime + extraSecondsFlying) * deer.flySpeed;
  }
  return post;
}

long long
winner (const std::vector<Reindeer>& herd)
{
  long long best = 0;
  for (const Reindeer& deer : herd) {
    if (deer.distanceTraveled > best) {
      best = deer.distanceTraveled;
    }
  }
  return best;
}

int
maxPoints (const std::vector<Reindeer>& pre, int time)
{
  std::vector<Reindeer> post = pre;
  std::vector<int> points (pre.size (), 0);
  std::vector<bool> flying (pre.size (), true);
  std::vector<int> seconds (pre.size (), 0);
  std::vector<int> distance (pre.size (), 0);
  for (int i = 0; i < time; ++i) {
    std::vector<int> bestIndices;
    for (std::size_t deerIndex = 0; deerIndex < pre.size (); ++deerIndex) {
      ++seconds[deerIndex];
      if (flying[deerIndex] && seconds[deerIndex] <= post[deerIndex].flyTime) {
        distance[deerIndex] += post[deerIndex].flySpeed;
      }
      if (flying[deerIndex] && seconds[deerIndex] == post[deerIndex].flyTime) {
        flying[deerIndex] = false;
        seconds[deerIndex] = 0;
      }
      else if (!flying[deerIndex] && seconds[deerIndex] == post[deerIndex].restTime) {
        flying[deerIndex] = true;
        seconds[deerIndex] = 0;
      }
      if (bestIndices.empty () || distance[deerIndex] == distance[bestIndices.front ()]) {
        bestIndices.push_back (deerIndex);
      }
      else if (distance[deerIndex] > distance[bestIndices.front ()]) {
        bestIndices.clear ();
        bestIndices.push_back (deerIndex);
      }
    }
    for (int deerIndex : bestIndices) {
      ++points[deerIndex];
    }
  }
  return *std::max_element (points.begin (), points.end ());
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Reindeer> herd = readInput ();
  std::vector<Reindeer> after2503 = after (herd, 2503);
  std::cout << winner (after2503) << "\n";
  std::cout << maxPoints (herd, 2503) << "\n";
  return 0;
}
