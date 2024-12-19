/// \file 2024Day17.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-17.

//// Disassembly of my input program ////
// <redacted>

//// Analysis ////
// Observation 01: One number gets printed each time through the loop.
// Observation 02: I am trying to print 16 numbers.
// Conclusion 03 (from Obs 01 and Obs 02): I need the loop to execute 16 times.
// Observation 04: The values of the B and C registers at the beginning of a loop are irrelevant.
// Observation 05: Each time through the loop shifts A right by 3.
// Observation 06: The program ends when A is 0.
// Conclusion 07 (from C03, O05, O06): The initial value of A must be between 36 and 38 bits long.
// Observation 08: The three highest-order bits of A entirely control the final output.
// Conclusion 09 (from experiments): A must start with one of <redacted> to get final output.
// Hypothesis 10: A must start with one of those + <redacted> to get final two outputs, etc.


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

using Number = long long;

const int OPCODE_ADV = 0;
const int OPCODE_BXL = 1;
const int OPCODE_BST = 2;
const int OPCODE_JNZ = 3;
const int OPCODE_BXC = 4;
const int OPCODE_OUT = 5;
const int OPCODE_BDV = 6;
const int OPCODE_CDV = 7;

const int REG_A = 0;
const int REG_B = 1;
const int REG_C = 2;

bool
isPrefix (const std::vector<int>& a, const std::vector<int>& b)
{
  if (a.size () > b.size ()) { return false; }
  for (unsigned int i = 0; i < a.size (); ++i) {
    if (a.at (i) != b.at (i)) { return false; }
  }
  return true;
}

bool
isSuffix (const std::vector<int>& a, const std::vector<int>& b)
{
  if (a.size () > b.size ()) { return false; }
  for (unsigned int i = 0; i < a.size (); ++i) {
    if (a.at (a.size() - 1 - i) != b.at (b.size () - 1 - i)) { return false; }
  }
  return true;
}

std::ostream&
operator<< (std::ostream& out, const std::vector<int>& numbers) {
  if (!numbers.empty ()) {
    out << numbers.front ();
    for (unsigned int x = 1; x < numbers.size (); ++x) {
      out << "," << numbers.at (x);
    }
  }
  else {
    out << "(empty list)";
  }
  out << "\n";
  return out;
}


struct VM
{
  Number registers[3];
  std::vector<int> instructions;
  int ip;
  std::vector<int> outputs;

  Number
  combo (int operand) {
    switch (operand) {
      case 0: return 0;
      case 1: return 1;
      case 2: return 2;
      case 3: return 3;
      case 4: return registers[REG_A];
      case 5: return registers[REG_B];
      case 6: return registers[REG_C];
      default: throw "Should not happen.\n";
    }
  }

  bool
  run (const std::vector<int>& desiredOutput = {}) {
    while (ip < (int)instructions.size ()) {
      if (!desiredOutput.empty () && !isPrefix (outputs, desiredOutput)) { return false; } // early exit if we are seeking a particular output and have already generated something incompatible with it.
      int opcode = instructions.at (ip);
      Number operand = instructions.at (ip + 1);

      if (opcode == OPCODE_ADV || opcode == OPCODE_BDV || opcode == OPCODE_CDV) {
        operand = combo (operand);
        if (operand == 0) { return false; } // Invalid program.
        Number numerator = registers[REG_A];
        Number denominator = 1 << operand;
        int regNum = (opcode == OPCODE_ADV ? REG_A : (opcode == OPCODE_BDV ? REG_B : REG_C));
        registers[regNum] = numerator / denominator;
      }
      else if (opcode == OPCODE_BXL) {
        registers[REG_B] = registers[REG_B] ^ operand;
      }
      else if (opcode == OPCODE_BST) {
        operand = combo (operand);
        registers[REG_B] = operand % 8;
      }
      else if (opcode == OPCODE_JNZ) {
        if (registers[REG_A] != 0) {
          ip = operand - 2;
        }
      }
      else if (opcode == OPCODE_BXC) {
        registers[REG_B] = registers[REG_B] ^ registers[REG_C];
      }
      else if (opcode == OPCODE_OUT) {
        operand = combo (operand);
        outputs.push_back (operand % 8);
      }
      ip += 2;
    }
    return true;
  }
};

VM
readInput ()
{
  VM vm;
  int foo, bar;
  scanf ("Register A: %lld\n", &vm.registers[0]);
  scanf ("Register B: %lld\n", &vm.registers[1]);
  scanf ("Register C: %lld\n", &vm.registers[2]);
  scanf ("\nProgram: %d,%d", &foo, &bar);
  assert (foo >= 0 && foo <= 7);
  assert (bar >= 0 && bar <= 7);
  vm.instructions.push_back (foo);
  vm.instructions.push_back (bar);
  while (scanf (",%d,%d", &foo, &bar) == 2) {
    assert (foo >= 0 && foo <= 7);
    assert (bar >= 0 && bar <= 7);
    vm.instructions.push_back (foo);
    vm.instructions.push_back (bar);
  }
  vm.ip = 0;
  return vm;
}

// Note: This brute force approach is woefully insufficient.
Number
tryAValues (const VM& vm)
{
  for (Number a = 0; ; ++a) {
    if (a % 10000000 == 0) { std::cout << "Trying " << a << "\n"; }
    VM copy = vm;
    copy.registers[REG_A] = a;
    if (copy.run (copy.outputs)) {
      if (copy.outputs == copy.instructions) {
        return a;
      }
    }
  }
  throw "Impossible!";
}

std::vector<Number>
trySpecificAValues (const VM& vm, const std::vector<Number>& values, const std::vector<int>& desiredOutputs)
{
  std::vector<Number> worked;
  for (const Number& num : values) {
    VM copy = vm;
    copy.registers[REG_A] = num;
    if (copy.run (desiredOutputs) && !copy.outputs.empty () && copy.outputs == desiredOutputs) {
      worked.push_back (num);
    }
  }
  return worked;
}

std::vector<Number>
workBackwards (const VM& vm) {
  std::vector<Number> allTribits = {0, 1, 2, 3, 4, 5, 6, 7};
  std::vector<Number> partialSolutions = {0};
  for (unsigned int outCount = 0; outCount < vm.instructions.size (); ++outCount) {
    std::vector<Number> potential;
    for (Number& old : partialSolutions) {
      for (Number & added : allTribits) {
        potential.push_back ((old << 3) + added);
      }
    }
    partialSolutions = trySpecificAValues (vm, potential, {vm.instructions.begin () + vm.instructions.size () - outCount - 1, vm.instructions.end ()});
    //std::cout << "The following inputs produce output " << std::vector<int> {vm.instructions.begin () + vm.instructions.size () - outCount - 1, vm.instructions.end ()};
    //for (Number num : partialSolutions) {
    //  std::cout << std::oct << num << ", ";
    //}
    //std::cout << "\n";
  }
  return partialSolutions;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  VM vm = readInput ();
  VM copy = vm;
  copy.run ({});
  std::cout << "Outputs with original config: " << copy.outputs << "\n";

  // Nice try!
  //std::cout << tryAValues (vm) << "\n";

  std::vector<Number> quineStarters = workBackwards (vm);
  std::sort (quineStarters.begin (), quineStarters.end ());
  if (quineStarters.empty ()) { std::cout << "No solutions found.\n"; }
  else { std::cout << "Smallest A value that makes it a quine: " << std::dec << quineStarters.front () << "\n"; }

  return 0;
}
