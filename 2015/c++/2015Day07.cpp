/// \file 2015Day07.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-07.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <array>
#include <cassert>
#include <cstdint>
#include <map>

using Value = uint16_t;
using Wire = std::string;

struct Shifter
{
  Wire input;
  bool isLeft;
  int amount;
  Wire output;

  Shifter () {}
  Shifter (const std::string& str)
  {
    assert (str.find ("SHIFT") != std::string::npos);
    std::size_t spaces[4];
    spaces[0] = str.find (' ');
    spaces[1] = str.find (' ', spaces[0] + 1);
    spaces[2] = str.find (' ', spaces[1] + 1);
    spaces[3] = str.find (' ', spaces[2] + 1);
    assert (spaces[3] != std::string::npos);
    input = str.substr (0, spaces[0]);
    assert (isalpha (input[0]));
    isLeft = (str[spaces[0] + 1] == 'L');
    amount = atoi (str.substr (spaces[1]).c_str ());
    assert (amount > 0);
    output = str.substr (spaces[3] + 1);
  }

  Value
  get (const Value in1) const {
    if (isLeft) { return in1 << amount; }
    else { return in1 >> amount; }
  }
};

struct Gate
{
  Wire inputA;
  Wire inputB;
  std::string type;
  Wire output;

  Gate () {}
  Gate (const std::string& str)
  {
    if (str.substr (0, 3) == "NOT") {
      type = "NOT";
      inputA = str.substr (4, str.find (" -> ") - 4);
      inputB = inputA;
      assert (isalpha (inputA[0]));
      assert (isalpha (inputB[0]));
      output = str.substr (str.find (" -> ") + 4);
    }
    else {
      std::size_t spaces[4];
      spaces[0] = str.find (' ');
      spaces[1] = str.find (' ', spaces[0] + 1);
      spaces[2] = str.find (' ', spaces[1] + 1);
      spaces[3] = str.find (' ', spaces[2] + 1);
      inputA = str.substr (0, spaces[0]);
      type = str.substr (spaces[0] + 1, spaces[1] - spaces[0] - 1);
      inputB = str.substr (spaces[1] + 1, spaces[2] - spaces[1] - 1);
      output = str.substr (spaces[3] + 1);
    }
  }

  Value
  get (const Value in1, const Value in2) const
  {
    Value v1 = in1;
    if (isdigit (inputA[0])) { v1 = atoi (inputA.c_str ()); }
    Value v2 = in2;
    if (isdigit (inputB[0])) { v2 = atoi (inputB.c_str ()); }

    if (type == "AND") { return v1 & v2; }
    else if (type == "OR") { return v1 | v2; }
    else { return ~v1; }
  }
};

struct Circuit
{
  std::map<Wire, Value> initialValues;
  std::map<Wire, Shifter> shifters;
  std::map<Wire, Gate> gates;
  std::map<Wire, Wire> copiers;
};

Circuit
readInput ()
{
  Circuit circuit;
  std::string line;
  while (std::getline (std::cin, line)) {
    if (line.find ("SHIFT") != std::string::npos) {
      Shifter x (line);
      circuit.shifters[x.output] = x;
    }
    else if (line.find ("NOT") != std::string::npos || line.find ("AND") != std::string::npos || line.find ("OR") != std::string::npos) {
      Gate x (line);
      circuit.gates[x.output] = x;
    }
    else {
      if (isdigit (line[0])) {
        circuit.initialValues[line.substr (line.find (" -> ") + 4)] = atoi (line.c_str ());
      }
      else {
        circuit.copiers[line.substr (line.find (" -> ") + 4)] = line.substr (0, line.find (' '));
      }
    }
  }
  return circuit;
}

std::map<Wire, Value>
compute (const Circuit& circuit)
{
  std::map<Wire, Value> finalValues = circuit.initialValues;
  std::map<Wire, Gate> unfinishedGates = circuit.gates;
  std::map<Wire, Shifter> unfinishedShifters = circuit.shifters;
  std::map<Wire, Wire> unfinishedCopiers = circuit.copiers;

  while (!unfinishedGates.empty () && !unfinishedShifters.empty () && !unfinishedCopiers.empty ()) {
    for (auto it = unfinishedGates.cbegin (); it != unfinishedGates.cend (); ) {
      if ((finalValues.contains (it->second.inputA) || isdigit (it->second.inputA[0])) && (finalValues.contains (it->second.inputB) || isdigit (it->second.inputB[0]))) {
        finalValues[it->first] = it->second.get (isdigit (it->second.inputA[0]) ? 0 : finalValues[it->second.inputA], isdigit (it->second.inputB[0]) ? 0 : finalValues[it->second.inputB]);
        it = unfinishedGates.erase (it);
      }
      else {
        ++it;
      }
    }
    for (auto it = unfinishedShifters.cbegin (); it != unfinishedShifters.cend (); ) {
      if (finalValues.contains (it->second.input) || isdigit (it->second.input[0])) {
        finalValues[it->first] = isdigit (it->second.input[0]) ? 0 : it->second.get (finalValues[it->second.input]);
        it = unfinishedShifters.erase (it);
      }
      else {
        ++it;
      }
    }
    for (auto it = unfinishedCopiers.cbegin (); it != unfinishedCopiers.cend (); ) {
      if (finalValues.contains (it->second)) {
        finalValues[it->first] = finalValues[it->second];
        it = unfinishedCopiers.erase (it);
      }
      else {
        ++it;
      }
    }
  }
  return finalValues;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Circuit circuit = readInput ();
  std::map<Wire, Value> finalValues = compute (circuit);
  std::cout << finalValues["a"] << "\n";
  circuit.initialValues["b"] = finalValues["a"];
  std::map<Wire, Value> newFinalValues = compute (circuit);
  std::cout << newFinalValues["a"] << "\n";
  return 0;
}
