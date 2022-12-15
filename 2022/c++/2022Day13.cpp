/// \file 2022Day13.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-13.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>

struct PacketData
{
  virtual std::string to_string () const = 0;
};

struct PacketInt : public PacketData
{
  int m_value;

  PacketInt (int v = 0) : m_value (v) {}

  virtual std::string to_string () const
  {
    return "" + std::to_string (m_value);
  }
};

struct PacketList : PacketData
{
  std::vector<PacketData*> m_parts;

  virtual std::string to_string () const
  {
    std::string result = "[";
    for (int index = 0; index < (int)m_parts.size () - 1; ++index)
    {
      result += m_parts[index]->to_string () + ",";
    }
    if (!m_parts.empty ())
    {
      result += m_parts[m_parts.size () - 1]->to_string ();
    }
    return result + "]";
  }
};

PacketData*
parse (const std::string& line)
{
  if (line.empty ()) { return nullptr; }
  if (line[0] == '[')
  {
    assert (line.size () > 1);
    assert (line[line.length () - 1] == ']');
    PacketList* result = new PacketList ();
    std::string contents = line.substr (1, line.length () - 2);
    while (!contents.empty ())
    {
      std::string car, cdr;
      if (contents[0] == '[')
      {
        std::size_t depth = 1;
        std::size_t index = 1;
        while (depth > 0)
        {
          if (contents[index] == '[') { ++depth; }
          else if (contents[index] == ']') { --depth; }
          ++index;
        }
        if (index == contents.size ())
        {
          car = contents;
          cdr = "";
        }
        else
        {
          assert (contents[index] == ',');
          car = contents.substr (0, index);
          cdr = contents.substr (index + 1);
        }
      }
      else
      {
        std::size_t comma = contents.find (',');
        if (comma == std::string::npos)
        {
          car = contents;
          cdr = "";
        }
        else
        {
          car = contents.substr (0, comma);
          cdr = contents.substr (comma + 1);
        }
      }
      PacketData* child = parse (car);
      if (child != nullptr) { result->m_parts.push_back (child); }
      contents = cdr;
    }
    return result;
  }
  else
  {
    for (char c : line) { assert (isdigit (c)); }
    return new PacketInt (atoi (line.c_str ()));
  }
}

std::vector<std::pair<PacketData*, PacketData*>>
getInput ()
{
  std::vector<std::pair<PacketData*, PacketData*>> pairs;
  std::string line1, line2, line3;
  while (true)
  {
    std::getline (std::cin, line1);
    std::getline (std::cin, line2);
    if (!std::cin) { break; }
    std::getline (std::cin, line3);
    assert (!line1.empty ());
    assert (line1[0] == '[');
    assert (!line2.empty ());
    assert (line2[0] == '[');
    assert (line3.empty ());

    pairs.push_back ({parse (line1), parse (line2)});
  }
  return pairs;
}

void
print (const std::vector<std::pair<PacketData*, PacketData*>>& pairs)
{
  for (const auto& pair : pairs)
  {
    std::cout << pair.first->to_string () << "\n";
    std::cout << pair.second->to_string () << "\n";
    std::cout << "\n";
  }
}

bool
less (PacketData* const & x, PacketData* const& y)
{
  const PacketInt* const xInt = dynamic_cast<const PacketInt*> (x);
  const PacketInt* const yInt = dynamic_cast<const PacketInt*> (y);
  const PacketList* const xList = dynamic_cast<const PacketList*> (x);
  const PacketList* const yList = dynamic_cast<const PacketList*> (y);
  if (xInt != nullptr && yInt != nullptr)
  {
    return xInt->m_value < yInt->m_value;
  }
  else if (xList != nullptr && yList != nullptr)
  {
    for (std::size_t index = 0; index < std::min (xList->m_parts.size (), yList->m_parts.size ()); ++index)
    {
      if (less (xList->m_parts[index], yList->m_parts[index])) { return true; }
      else if (less (yList->m_parts[index], xList->m_parts[index])) { return false; }
    }
    return xList->m_parts.size () < yList->m_parts.size ();
  }
  else if (xInt != nullptr && yList != nullptr)
  {
    PacketInt pi (xInt->m_value);
    PacketList pl;
    pl.m_parts.push_back (&pi);
    return less (&pl, y);
  }
  else
  {
    assert (xList != nullptr && yInt != nullptr);
    PacketInt pi (yInt->m_value);
    PacketList pl;
    pl.m_parts.push_back (&pi);
    return less (x, &pl);
  }
}

unsigned int
part1 (const std::vector<std::pair<PacketData*, PacketData*>>& pairs)
{
  unsigned int total = 0;
  for (std::size_t index = 0; index < pairs.size (); ++index)
  {
    if (less (pairs[index].first, pairs[index].second))
    {
      total += (index + 1);
    }
  }
  return total;
}

struct Comparator
{
  bool
  operator() (PacketData* const & x, PacketData* const& y)
  {
    return less (x, y);
  }
};

unsigned int
part2 (const std::vector<std::pair<PacketData*, PacketData*>>& pairs)
{
  std::vector<PacketData*> packets;
  for (const auto& pair : pairs)
  {
    packets.push_back (pair.first);
    packets.push_back (pair.second);
  }
  PacketData* divider2 = parse ("[[2]]");
  PacketData* divider6 = parse ("[[6]]");
  packets.push_back (divider2);
  packets.push_back (divider6);

  Comparator comp;
  std::sort (packets.begin (), packets.end (), comp);

  std::size_t index2, index6;
  for (std::size_t index = 0; index < packets.size (); ++index)
  {
    if (packets[index] == divider2) { index2 = index; }
    else if (packets[index] == divider6) { index6 = index; }
  }
  return (index2 + 1) * (index6 + 1);
}

int main () {
  std::vector<std::pair<PacketData*, PacketData*>> pairs = getInput ();
  print (pairs);
  std::cout << part1 (pairs) << "\n";
  std::cout << part2 (pairs) << "\n";
  return 0;
}
