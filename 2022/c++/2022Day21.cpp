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

  virtual bool
  uses (const Map& map, const std::string& key) const = 0;
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

  virtual bool
  uses (const Map& map, const std::string& key) const
  {
    return false;
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
      // Turning this off because there don't appear to be duplicates anyway and it will break part 2.
      //cacheValid = true;
    }
    return cachedValue;
  }

  virtual bool
  uses (const Map& map, const std::string& key) const
  {
    return left == key || right == key || map.at (left)->uses (map, key) || map.at (right)->uses (map, key);
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

Number
reverseEngineer (const Map& map, const std::string& rootName, Number target)
{
  OperExpr* root = dynamic_cast<OperExpr*> (map.at (rootName));
  assert (root != nullptr);
  if (root->left == "humn")
  {
    Number subExpr = map.at (root->right)->eval (map);
    // humn + subExpr = target -> humn = target - subExpr
    if (root->oper == "+") { return target - subExpr; }
    // humn - subExpr = target -> humn = target + subExpr
    else if (root->oper == "-") { return target + subExpr; }
    // humn * subExpr = target -> humn = target / subExpr
    else if (root->oper == "*") { return target / subExpr; }
    // humn / subExpr = target -> humn = target * subExpr
    else { return target * subExpr; }
  }
  else if (root->right == "humn")
  {
    Number subExpr = map.at (root->left)->eval (map);
    // subExpr + humn = target -> humn = target - subExpr
    if (root->oper == "+") { return target - subExpr; }
    // subExpr - humn = target -> humn = subExpr - target
    else if (root->oper == "-") { return subExpr - target; }
    // subExpr * humn = target -> humn = target / subExpr
    else if (root->oper == "*") { return target / subExpr; }
    // subExpr / humn = target -> humn = subExpr / target
    else { return subExpr / target; }
  }
  else if (map.at (root->left)->uses (map, "humn"))
  {
    assert (!map.at (root->right)->uses (map, "humn"));
    Number subExpr = map.at (root->right)->eval (map);
    // missing + subExpr = target -> missing = target - subExpr
    if (root->oper == "+") { return reverseEngineer (map, root->left, target - subExpr); }
    // missing - subExpr = target -> missing = target + subExpr
    else if (root->oper == "-") { return reverseEngineer (map, root->left, target + subExpr); }
    // missing * subExpr = target -> missing = target / subExpr
    else if (root->oper == "*") { return reverseEngineer (map, root->left, target / subExpr); }
    // missing / subExpr = target -> missing = target * subExpr
    else { return reverseEngineer (map, root->left, target * subExpr); }
  }
  else
  {
    assert (map.at (root->right)->uses (map, "humn"));
    Number subExpr = map.at (root->left)->eval (map);
    // subExpr + missing = target -> missing = target - subExpr
    if (root->oper == "+") { return reverseEngineer (map, root->right, target - subExpr); }
    // subExpr - missing = target -> missing = subExpr - target
    else if (root->oper == "-") { return reverseEngineer (map, root->right, subExpr - target); }
    // subExpr * missing = target -> missing = target / subExpr
    else if (root->oper == "*") { return reverseEngineer (map, root->right, target / subExpr); }
    // subExpr / missing = target -> missing = subExpr / target
    else { return reverseEngineer (map, root->right, subExpr / target); }
  }
}

Number
part2 (Map map)
{
  OperExpr* root = dynamic_cast<OperExpr*> (map.at ("root"));
  assert (root != nullptr);
  std::string known, unknown;
  if (map.at (root->left)->uses (map, "humn"))
  {
    assert (!map.at (root->right)->uses (map, "humn"));
    known = root->right;
    unknown = root->left;
  }
  else
  {
    assert (map.at (root->right)->uses (map, "humn"));
    known = root->left;
    unknown = root->right;
  }
  Number target = map.at (known)->eval (map);
  return reverseEngineer (map, unknown, target);
}

int main ()
{
  Map map = getInput ();
  std::cout << map.at ("root")->eval (map) << "\n";
  std::cout << part2 (map) << "\n";
  return 0;
}

// My answers:
// Part 1: 159591692827554
// Part 2: 3509819803065
