/// \file 2022Day16.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-16.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>
#include <set>

struct Valve
{
  std::string m_name;
  unsigned m_flowRate;
  bool m_opened;
  std::set<std::string> m_tunnels;
};

struct Problem
{
  std::map<std::string, struct Valve> m_valves;
  unsigned m_minutesLeft;
  unsigned long m_pressureReleased;
};

Problem
getInput ()
{
  Problem prob;
  prob.m_minutesLeft = 30;
  prob.m_pressureReleased = 0;
  std::string line;
  std::string part1 = "Valve ";
  std::string part2 = " has flow rate=";
  std::string part3a = "; tunnels lead to valves ";
  std::string part3b = "; tunnel leads to valve ";
  std::string part4 = ", ";
  while (std::getline (std::cin, line))
  {
    if (line.empty ()) { break; }
    Valve current;
    current.m_opened = false;
    std::size_t part1Loc = line.find (part1);
    assert (part1Loc == 0);
    std::size_t part2Loc = line.find (part2);
    assert (part2Loc == part1.size () + 2);
    current.m_name = line.substr (part1.size (), 2);
    current.m_flowRate = atoi (line.substr (part1.size () + 2 + part2.size ()).c_str ());
    std::size_t part3aLoc = line.find (part3a);
    std::size_t part3bLoc = line.find (part3b);
    assert (part3aLoc != std::string::npos || part3bLoc != std::string::npos);
    if (part3bLoc != std::string::npos)
    {
      assert (part3bLoc + part3b.size () + 2 == line.size ());
      current.m_tunnels.insert (line.substr (part3bLoc + part3b.size ()));
    }
    else
    {
      std::size_t now = part3aLoc + part3a.size ();
      while (now < line.size ())
      {
        current.m_tunnels.insert (line.substr (now, 2));
        now += 2;
        if (now < line.size ())
        {
          assert (line.substr (now, 2) == part4);
          now += 2;
        }
      }
    }
    prob.m_valves.insert ({current.m_name, current});
  }
  return prob;
}

int main () {
  Problem prob = getInput ();

  return 0;
}
