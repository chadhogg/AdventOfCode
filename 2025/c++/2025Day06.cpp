/// \file 2025Day06.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-06.

#include <cstddef>
#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>

using Number = long long;

struct Problem {
  std::vector<std::vector<Number>> operands;
  std::vector<std::string> operators;
};

Problem
getInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::vector<Number> numbers;
    std::vector<std::string> ops;
    Number num;
    std::string op;
    std::stringstream stream (line);
    while (stream >> num) {
      numbers.push_back (num);
    }
    stream.clear ();
    while (stream >> op) {
      assert (op == "*" || op == "+");
      ops.push_back (op);
    }
    assert (numbers.empty () != ops.empty ());
    assert (prob.operators.empty ());
    if (ops.empty ()) {
      if (!prob.operands.empty ()) { assert (prob.operands[0].size () == numbers.size ()); }
      prob.operands.push_back (numbers);
    }
    else {
      assert (!prob.operands.empty ());
      assert (prob.operands[0].size () == ops.size ());
      prob.operators = ops;
    }
  }
  return prob;
}

Number
computeResults (const Problem& prob)
{
  Number total = 0;
  for (std::size_t index = 0; index < prob.operators.size (); ++index) {
    if (prob.operators.at (index) == "+") {
      Number sum = 0;
      for (std::size_t index2 = 0; index2 < prob.operands.size (); ++index2) {
        sum += prob.operands[index2][index];
      }
      total += sum;
    }
    else {
      Number product = 1;
      for (std::size_t index2 = 0; index2 < prob.operands.size (); ++index2) {
        product *= prob.operands[index2][index];
      }
      total += product;
    }
  }
  return total;
}

std::vector<std::string>
getInput2 ()
{
  std::vector<std::string> lines;
  std::string line;
  while (std::getline (std::cin, line)) {
    lines.push_back (line);
  }
  return lines;
}

bool
containsNonSpace (const std::vector<std::string>& lines, std::size_t index)
{
  for (std::size_t lineNum = 0; lineNum < lines.size (); ++lineNum) {
    if (lines.at (lineNum).at (index) != ' ') { return true; }
  }
  return false;
}

Number
computeResults2 (const std::vector<std::string>& lines)
{
  const std::size_t SYMBOL_LINE = lines.size () - 1; 
  Number total = 0;
  std::size_t startIndex = 0;
  std::size_t endIndex = 1;
  while (startIndex < lines.at (0).size ()) {
    while (endIndex < lines.at (0).size () && containsNonSpace (lines, endIndex)) { ++endIndex; }
    std::vector<Number> numbers;
    for (std::size_t currIndex = startIndex; currIndex < endIndex; ++currIndex) {
      std::string str = "";
      for (std::size_t in = 0; in < SYMBOL_LINE; ++in) {
        if (isdigit (lines.at (in).at (currIndex))) { str += lines.at (in).at (currIndex); }
      }
      numbers.push_back (atoi (str.c_str ()));
    }
    if (lines.at (SYMBOL_LINE).substr (startIndex, endIndex - startIndex).find ("+") != std::string::npos) {
      Number sum = 0;
      for (Number num : numbers) { sum += num; }
      total += sum;
    }
    else {
      Number product = 1;
      for (Number num : numbers) { product *= num; }
      total += product;
    }
    startIndex = endIndex + 1;
    endIndex = startIndex + 1;
  }
  return total;
}

int
main (int argc, char* argv[])
{
//  Problem prob = getInput ();
//  std::cout << computeResults (prob) << "\n";
  std::vector<std::string> prob2 = getInput2 ();
  std::cout << computeResults2 (prob2) << "\n";
  return EXIT_SUCCESS;
}
