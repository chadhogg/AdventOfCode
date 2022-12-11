/// \file 2022Day11.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-11.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>

using Worry = unsigned long;

struct Monkey
{
  Monkey (std::string o, std::string op, int t, int tt, int ft) : items {}, oper (o), operand (op), test (t), trueThrow (tt), falseThrow (ft) { }
  std::list<Worry> items;
  std::string oper;
  std::string operand;
  int test;
  int trueThrow;
  int falseThrow;
};

std::vector<Monkey>
getInput ()
{
  std::vector<Monkey> monkeys;
  std::string headerLine, itemsLine, operationLine, testLine, trueLine, falseLine, blankLine;
  while (true)
  {
    std::getline (std::cin, headerLine);
    if (std::cin.fail ()) { break; }
    std::getline (std::cin, itemsLine);
    assert (std::cin);
    std::getline (std::cin, operationLine);
    assert (std::cin);
    std::getline (std::cin, testLine);
    assert (std::cin);
    std::getline (std::cin, trueLine);
    assert (std::cin);
    std::getline (std::cin, falseLine);
    assert (std::cin);
    
    assert (headerLine.substr (0, 7) == "Monkey ");
    std::size_t colonLoc = headerLine.find (':');
    assert (colonLoc != std::string::npos);
    assert (atoi (headerLine.substr (7, colonLoc - 7).c_str ()) == (int)monkeys.size ());
    assert (headerLine.size () == colonLoc + 1);

    // Read starting items line.
    std::vector<int> items;
    assert (itemsLine.substr (0, 18) == "  Starting items: ");
    std::size_t start = 18;
    std::size_t commaLoc = itemsLine.find (',');
    while (commaLoc != std::string::npos)
    {
      items.push_back (atoi (itemsLine.substr (start, commaLoc - start).c_str ()));
      start = commaLoc + 2;
      commaLoc = itemsLine.find (',', start);
    }
    //if (start == 18)
    {
      items.push_back (atoi (itemsLine.substr (start).c_str ()));
    }

    // Read operation line
    std::string rater, operand;
    assert (operationLine.substr (0, 23) == "  Operation: new = old ");
    rater = operationLine.substr (23, 1);
    assert (rater == "+" || rater == "*");
    operand = operationLine.substr (25);

    // Read test line
    int test;
    assert (testLine.substr (0, 21) == "  Test: divisible by ");
    test = atoi (testLine.substr (21).c_str ());

    // Read true line
    int trueThrow;
    assert (trueLine.substr (0, 29) == "    If true: throw to monkey ");
    trueThrow = atoi (trueLine.substr (29).c_str ());

    // Read false line
    int falseThrow;
    assert (falseLine.substr (0, 30) == "    If false: throw to monkey ");
    falseThrow = atoi (falseLine.substr (30).c_str ());

    std::getline (std::cin, blankLine);
    assert (blankLine.empty ());

    monkeys.push_back ( {rater, operand, test, trueThrow, falseThrow});
    for (int x : items)
    {
      monkeys.back ().items.push_back (x);
    }

  }
  return monkeys;
}

Worry
applyOperation (Worry old, const std::string& rater, const std::string& operand)
{
  assert (rater == "+" || rater == "*");
  if (rater == "+")
  {
    if (operand == "old") { return old + old; }
    else { return old + atoi (operand.c_str ()); }
  }
  else
  {
    if (operand == "old") { return old * old; }
    else { return old * atoi (operand.c_str ()); }
  }
}

std::vector<unsigned long>
doRound (std::vector<Monkey>& monkeys, const Worry RELIEF, const Worry DIVISOR_PROD)
{
  std::vector<unsigned long> amountInspected;
  for (std::size_t activeIndex = 0; activeIndex < monkeys.size (); ++activeIndex)
  {
    Monkey& monk = monkeys[activeIndex];
    amountInspected.push_back (monk.items.size ());
    while (!monk.items.empty ())
    {
      Worry item = monk.items.front ();
      monk.items.pop_front ();
      item = applyOperation (item, monk.oper, monk.operand);
      item /= RELIEF;
      item %= DIVISOR_PROD;
      if (item % monk.test == 0)
      {
        monkeys[monk.trueThrow].items.push_back (item);
      }
      else
      {
        monkeys[monk.falseThrow].items.push_back (item);
      }
    }
  }
  return amountInspected;
}

Worry
solve (std::vector<Monkey> monkeys, const int RELIEF, const int ROUNDS)
{
  // Explanation:
  // What matters to determine where an item goes is whether or not it is divisible
  //   by each monkey's test.  Modding by the product of each of those won't change
  //   the answer.  This prevents worry levels from growing infinitely.
  Worry divisorProd = 1;
  for (const Monkey& monk : monkeys) { divisorProd *= monk.test; }
  
  std::vector<unsigned long> activities (monkeys.size (), 0);
  for (int i = 1; i <= ROUNDS; ++i)
  {
    std::vector<unsigned long> amountInspected = doRound (monkeys, RELIEF, divisorProd);
    for (std::size_t index = 0; index < monkeys.size (); ++index)
    {
      activities[index] += amountInspected[index];
    }
    //std::cout << "After round " << i << "\n";
    //for (std::size_t index = 0; index < monkeys.size (); ++index)
    //{
    //  std::cout << "Monkey " << index << " has ";
    //  for (const auto& x : monkeys[index].items) { std::cout << x << " "; }
    //  std::cout << "\n";
    //}
    //for (auto x : activities) { std::cout << x << " "; } std::cout << "\n";
  }
  //for (auto x : activities) { std::cout << x << " "; } std::cout << "\n";
  std::sort (activities.begin (), activities.end ());
  return activities[monkeys.size () - 2] * activities[monkeys.size () - 1];
}

int main () {
  std::vector<Monkey> monkeys = getInput ();
  std::cout << solve (monkeys, 3, 20) << "\n";
  std::cout << solve (monkeys, 1, 10000) << "\n";
  return 0;
}
