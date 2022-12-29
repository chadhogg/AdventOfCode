/// \file 2022Day21.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-21.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

using Number = long;
struct Expr;
using Map = std::map<std::string, Expr*>;

struct Expr
{
  virtual Number
  eval (const Map& map) const = 0;
};

struct LiteralExpr : public Expr
{
  LiteralExpr (Number v) : value (v) {}
  Number value;

  virtual Number
  eval (const Map& map) const
  {
    return value;
  }
};

struct OperExpr : public Expr
{
  OperExpr (const std::string& l, const std::string& o, const std::string& r) : left (l), oper (o), right (r), cachedValue (0), cacheValid (false) {}
  std::string left, oper, right;
  mutable Number cachedValue;
  mutable bool cacheValid;

  virtual Number
  eval (const Map& map) const
  {
    if (!cacheValid)
    {
      if (oper == "+")
      {
        cachedValue = map.at (left)->eval (map) + map.at (right)->eval (map);
      }
      else if (oper == "-")
      {
        cachedValue = map.at (left)->eval (map) - map.at (right)->eval (map);
      }
      else if (oper == "*")
      {
        cachedValue = map.at (left)->eval (map) * map.at (right)->eval (map);
      }
      else if (oper == "/")
      {
        cachedValue = map.at (left)->eval (map) / map.at (right)->eval (map);
      }
      else
      {
        assert (false);
      }
      cacheValid = true;
    }
    return cachedValue;
  }
};

Map
getInput ()
{
  Map map;
  std::string line;
  std::getline (std::cin, line);
  while (std::cin)
  {
    assert (line.size () >= 7);
    assert (line[4] == ':');
    std::string name = line.substr (0, 4);
    if (isdigit (line[6]))
    {
      map[name] = new LiteralExpr (atoi (line.substr (6).c_str ()));
    }
    else
    {
      assert (line.size () == 17);
      map[name] = new OperExpr (line.substr (6, 4), line.substr (11, 1), line.substr (13));
    }
    std::getline (std::cin, line);
  }
  return map;
}

int main ()
{
  Map map = getInput ();
  std::cout << map.at ("root")->eval (map) << "\n";
  return 0;
}

// My answers:
// Part 1: 159591692827554
// Part 2: 
