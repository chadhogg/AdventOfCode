/// \file 2024Day24.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-24.


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

using Wire = std::string;
using Type = std::string;
const Type TYPE_AND = "AND";
const Type TYPE_OR = "OR";
const Type TYPE_XOR = "XOR";

struct Gate
{
  Type type;
  Wire inputA;
  Wire inputB;
  Wire output;

  bool
  compute (bool a, bool b)
  {
    if (type == TYPE_AND) { return a && b; }
    else if (type == TYPE_OR) { return a || b; }
    else if (type == TYPE_XOR) { return a != b; }
    else { throw "Impossible"; }
  }
};

struct Problem
{
  std::map<Wire, bool> initialValues;
  std::list<Gate> gates;
};

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::size_t colonPos = line.find (':');
    if (colonPos != std::string::npos) {
      assert (isalpha (line[0]) /*&& isdigit (line[1]) && isdigit (line[2])*/);
      assert (line[5] == '1' || line[5] == '0');
      prob.initialValues[line.substr (0, 3)] = atoi (line.substr (5, 1).c_str ());
    }
    else if (line != "") {
      Gate gate;
      if (line.find ("AND") != std::string::npos || line.find ("XOR") != std::string::npos) {
        gate.inputA = line.substr (0, 3);
        gate.type = line.substr (4, 3);
        gate.inputB = line.substr (8, 3);
        gate.output = line.substr (15, 3);
      }
      else {
        gate.inputA = line.substr (0, 3);
        gate.type = line.substr (4, 2);
        gate.inputB = line.substr (7, 3);
        gate.output = line.substr (14, 3);
      }
      assert (gate.type == TYPE_AND || gate.type == TYPE_OR || gate.type == TYPE_XOR);
      assert (isalpha (gate.inputA[0]) && isalpha (gate.inputB[0]) && isalpha (gate.output[0]));
      //assert (isdigit (gate.inputA[1]) && isdigit (gate.inputB[1]) && isdigit (gate.output[1]));
      //assert (isdigit (gate.inputA[2]) && isdigit (gate.inputB[2]) && isdigit (gate.output[2]));
      assert (gate.inputA != gate.inputB && gate.inputB != gate.output && gate.inputA != gate.output);
      prob.gates.push_back (gate);
    }
  }
  return prob;
}

std::map<Wire, bool>
findFinalValues (const std::map<Wire, bool>& initialValues, const std::list<Gate>& gates)
{
  std::map<Wire, bool> finalValues = initialValues;
  std::list<Gate> unfinishedGates = gates;
  while (!unfinishedGates.empty ()) {
    for (std::list<Gate>::iterator it = unfinishedGates.begin (); it != unfinishedGates.end (); ) {
      if (finalValues.contains (it->output)) {
        it = unfinishedGates.erase (it);
      }
      if (finalValues.contains (it->inputA) && finalValues.contains (it->inputB)) {
        finalValues[it->output] = it->compute (finalValues[it->inputA], finalValues[it->inputB]);
        it = unfinishedGates.erase (it);
      }
      else {
        ++it;
      }
    }
  }
  return finalValues;
}

long long
part1 (const Problem& prob)
{
  std::map<Wire, bool> finalValues = findFinalValues (prob.initialValues, prob.gates);
  long long number = 0;
  for (std::map<Wire, bool>::const_iterator it = finalValues.begin (); it != finalValues.end (); ++it) {
    if (it->first[0] == 'z') {
      long long part = it->second;
      number += part << atoi (it->first.substr (1).c_str ());
    }
  }
  return number;
}

std::set<Wire>
findHalfAdderSum (const Problem& prob, Wire inA, Wire inB)
{
  assert (prob.initialValues.contains (inA));
  assert (prob.initialValues.contains (inB));
  std::set<Wire> candidates;
  for (const Gate& gate : prob.gates) {
    candidates.insert (gate.output);
  }

  std::map<Wire, bool> start = prob.initialValues;
  for (bool in1 : {false, true}) {
    for (bool in2: {false, true}) {
      start[inA] = in1;
      start[inB] = in2;
      std::map<Wire, bool> end = findFinalValues (start, prob.gates);
      for (auto it = candidates.begin (); it != candidates.end (); ) {
        if (end[*it] == (in1 != in2)) { ++it; }
        else { it = candidates.erase (it); }
      }
    }
  }
  return candidates;
}

std::set<Wire>
findHalfAdderCarry (const Problem& prob, Wire inA, Wire inB)
{
  assert (prob.initialValues.contains (inA));
  assert (prob.initialValues.contains (inB));
  std::set<Wire> candidates;
  for (const Gate& gate : prob.gates) {
    candidates.insert (gate.output);
  }

  std::map<Wire, bool> start = prob.initialValues;
  for (bool in1 : {false, true}) {
    for (bool in2: {false, true}) {
      start[inA] = in1;
      start[inB] = in2;
      std::map<Wire, bool> end = findFinalValues (start, prob.gates);
      for (auto it = candidates.begin (); it != candidates.end (); ) {
        if (end[*it] == (in1 && in2)) { ++it; }
        else { it = candidates.erase (it); }
      }
    }
  }
  return candidates;
}

std::set<Wire>
findFullAdderSum (const Problem& prob, Wire inA, Wire inB, Wire inC)
{
  assert (prob.initialValues.contains (inA));
  assert (prob.initialValues.contains (inB));
  std::set<Wire> candidates;
  for (const Gate& gate : prob.gates) {
    candidates.insert (gate.output);
  }

  std::map<Wire, bool> start = prob.initialValues;
  for (bool in1 : {false, true}) {
    for (bool in2: {false, true}) {
      for (bool in3: {false, true}) {
        start[inA] = in1;
        start[inB] = in2;
        start[inC] = in3;
        std::map<Wire, bool> end = findFinalValues (start, prob.gates);
        for (auto it = candidates.begin (); it != candidates.end (); ) {
          if (end[*it] == ((in1 != in2) != in3)) { ++it; }
          else { it = candidates.erase (it); }
        }
      }
    }
  }
  return candidates;
}

std::set<Wire>
findFullAdderCarry (const Problem& prob, Wire inA, Wire inB, Wire inC)
{
  assert (prob.initialValues.contains (inA));
  assert (prob.initialValues.contains (inB));
  std::set<Wire> candidates;
  for (const Gate& gate : prob.gates) {
    candidates.insert (gate.output);
  }

  std::map<Wire, bool> start = prob.initialValues;
  for (bool in1 : {false, true}) {
    for (bool in2: {false, true}) {
      for (bool in3: {false, true}) {
        start[inA] = in1;
        start[inB] = in2;
        start[inC] = in3;
        std::map<Wire, bool> end = findFinalValues (start, prob.gates);
        for (auto it = candidates.begin (); it != candidates.end (); ) {
          if (end[*it] == ((in1 && in2) || (in3 && (in1 != in2)))) { ++it; }
          else { it = candidates.erase (it); }
        }
      }
    }
  }
  return candidates;
}

std::set<Wire>
findContributingSignals (const Problem& prob, Wire out) {
  std::set<Wire> frontier = {out};
  std::set<Wire> answer;
  while (!frontier.empty ()) {
    Wire current = *frontier.begin ();
    frontier.erase (current);
    if (prob.initialValues.contains (current)) { answer.insert (current); }
    else {
      for (const Gate& gate : prob.gates) {
        if (gate.output == current) {
          frontier.insert (gate.inputA);
          frontier.insert (gate.inputB);
          break;
        }
      }
    }
  }
  return answer;
}


template<typename T>
std::ostream&
operator<<(std::ostream& out, const std::set<T>& container) {
  out << "{ ";
  for (const T& thing: container) {
    out << thing << " ";
  }
  out << "}";
  return out;
}

Wire
analyze (const Problem& prob, int bitNum, Wire carryIn)
{
  Wire x = bitNum > 9 ? "x" + std::to_string (bitNum) : "x0" + std::to_string (bitNum);
  Wire y = bitNum > 9 ? "y" + std::to_string (bitNum) : "y0" + std::to_string (bitNum);
  Wire z = bitNum > 9 ? "z" + std::to_string (bitNum) : "z0" + std::to_string (bitNum);
  if (bitNum == 0) {
    std::set<Wire> sum = findHalfAdderSum (prob, x, y);
    std::set<Wire> carry = findHalfAdderCarry (prob, x, y);
    if (sum == std::set<Wire> {z}) {
      std::cout << "Sum of bit " << bitNum << " is correct.\n";
    }
    else {
      std::cout << "\n!!!!!!!!\n" << "Sum of bit " << bitNum << " is " << sum << "\n\n";
    }
    for (const Wire& possible: carry) {
      std::set<Wire> components = findContributingSignals (prob, possible);
      if (components == std::set<Wire> {x, y}) {
        std::cout << "Carry-out of " << bitNum << " appears to be " << possible << "\n";
        return possible;
      }
    }
    std::cout << "\n!!!!!!!!\n" << "Carry-out of bit " << bitNum << " does not appear to exist.\n\n";
    return "";
  }
  else {
    std::set<Wire> sum = findFullAdderSum (prob, x, y, carryIn);
    std::set<Wire> carry = findFullAdderCarry (prob, x, y, carryIn);
    if (sum == std::set<Wire> {z}) {
      std::cout << "Sum of bit " << bitNum << " is correct.\n";
    }
    else {
      std::cout << "\n!!!!!!!!\n" << "Sum of bit " << bitNum << " is " << sum << "\n\n";
    }
    for (const Wire& possible: carry) {
      std::set<Wire> components = findContributingSignals (prob, possible);
      std::set<Wire> shouldBe = {};
      for (int i = 0; i <= bitNum; ++i) {
        if (i > 9) {
          shouldBe.insert ("x" + std::to_string (i));
          shouldBe.insert ("y" + std::to_string (i));
        }
        else {
          shouldBe.insert ("x0" + std::to_string (i));
          shouldBe.insert ("y0" + std::to_string (i));
        }
      }
      if (components == shouldBe) {
        std::cout << "Carry-out of " << bitNum << " appears to be " << possible << "\n";
        return possible;
      }
    }
    std::cout << "\n!!!!!!!!\n" << "Carry-out of bit " << bitNum << " does not appear to exist.\n\n";
    return "";
  }
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << part1 (prob) << "\n";
  //std::cout << findHalfAdderSum (prob, "x00", "y00") << "\n";
  // Confirmed: z00 is sum of x00 and y00
  //std::cout << findHalfAdderCarry (prob, "x00", "y00") << "\n";
  // Confirmed: rpj is carry-out of x00 and y00
  //std::cout << findFullAdderSum (prob, "x01", "y01", "rpj") << "\n";
  // Confirmed: z01 is sum of x01 and y01 and rpj (carry-out of x00 and y00)
  //std::cout << findFullAdderCarry (prob, "x01", "y01", "rpj") << "\n";
  // Hmm, the carry-out of x01 and y01 could be gsh or vbm or whn.
  //std::cout << findContributingSignals (prob, "gsh") << "\n";
  // Confirmed: gsh is the carry-out of x01 and y01
  //std::cout << findFullAdderSum (prob, "x02", "y02", "gsh") << "\n";
  // Confirmed: z02 is the sum of x02 and y02 and gsh (carry-out of x01 and y01)
  //std::cout << findFullAdderCarry (prob, "x02", "y02", "gsh") << "\n";
  // Confirmed: whn is the carry-out of x02 and y02
  //std::cout << findFullAdderSum (prob, "x03", "y03", "whn") << "\n";
  // Confirmed: z03 is the sum of x03 and y03 and whn (carry-out of x02 and y02)
  //std::cout << findFullAdderCarry (prob, "x03", "y03", "whn") << "\n";
  // Hmm, the carry-out of x03 and y03 and whn could be ggh or hpv or jst or rkg or shw or tkj or vbk or vvg

  // Fixes:
  for (Gate& gate : prob.gates) {
    if (gate.output == "z05") { gate.output = "jst"; }
    else if (gate.output == "jst") { gate.output = "z05"; }
    else if (gate.output == "gdf") { gate.output = "mcm"; }
    else if (gate.output == "mcm") { gate.output = "gdf"; }
    else if (gate.output == "z15") { gate.output = "dnt"; }
    else if (gate.output == "dnt") { gate.output = "z15"; }
    else if (gate.output == "z30") { gate.output = "gwc"; }
    else if (gate.output == "gwc") { gate.output = "z30"; }
  }
  // dnt,gdf,gwc,jst,mcm,z05,z15,z30

  Wire carryIn = "";
  for (int i = 0; i < 45; ++i) {
    carryIn = analyze (prob, i, carryIn);
  }

  //std::cout << findContributingSignals (prob, "dnt") << "\n";

  return 0;
}

// Discoveries:
// - First problem is at bit 05.
//   The gate that should be connected to z05 is instead connected to jst.
//   The gate that should be producing bit 5's carry out (jst?) is instead connected to z05.
// - Next problem is at bit 10.
//   No gate appears to produce what should be connected to z10.
//   The gate connected to z10 does depend on the correct inputs, so the problem is probably somewhere inside it.
//   z10 = mcm XOR tdw
//       = (x10 AND y10) XOR (carry-out of bit 9)
//   z09 = cgq XOR mnk
//       = (carry-out of bit 8) XOR (x09 XOR y09)
//   If every bit (when fixed) is computed the same way, there should be a gate that computes (x10 XOR y10).
//      It is gdf!
//   So let's swap gdf and mcm and hope for the best.
// - Next problem is at bit 15.
//   The gate connected to dnt appears as though it should be connected to z15 instead.
//   dnt = vhr XOR dvj
//       = (carry-out of bit 14) XOR (x15 XOR y15)
//   So vice versa as well.
// - Next problem is at bit 30.
//   The gate connected to gwc appears as though it should be connected to z30 instead.
//   Presumably, the opposite is also true.