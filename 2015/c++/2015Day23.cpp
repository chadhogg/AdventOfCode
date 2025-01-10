/// \file 2015Day23.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-23.

#include <iostream>
#include <string>
#include <cassert>
#include <set>
#include <vector>
#include <map>

const std::string HALF = "hlf";
const std::string TRIPLE = "tpl";
const std::string INCREMENT = "inc";
const std::string JUMP = "jmp";
const std::string JUMP_EVEN = "jie";
const std::string JUMP_ONE = "jio";
const std::set<std::string> opcodes = {HALF, TRIPLE, INCREMENT, JUMP, JUMP_EVEN, JUMP_ONE};

const std::string REG_A = "a";
const std::string REG_B = "b";
const std::set<std::string> registers = {REG_A, REG_B};

struct Instruction
{
  std::string opcode;
  std::string reg;
  int offset;
};

using Program = std::vector<Instruction>;

struct VM
{
  VM () : ip (0), registers ({{REG_A, 0}, {REG_B, 0}}) {}
  int ip;
  std::map<std::string, long long> registers;
};

Program
readInput ()
{
  Program prog;
  std::string line;
  while (std::getline (std::cin, line)) {
    assert (line.size () >= 5);
    std::string opcode = line.substr (0, 3);
    std::string rest = line.substr (4);
    if (opcode == HALF || opcode == TRIPLE || opcode == INCREMENT) {
      assert (registers.contains (rest));
      prog.push_back ({opcode, rest, 0});
    }
    else if (opcode == JUMP) {
      prog.push_back ({opcode, "", atoi (rest.c_str ())});
    }
    else {
      assert (opcode == JUMP_EVEN || opcode == JUMP_ONE);
      std::string reg = rest.substr (0, 1);
      assert (registers.contains (reg));
      assert (rest.substr (3, 1) == "+" || rest.substr (3, 1) == "-");
      prog.push_back ({opcode, reg, atoi (rest.substr (3).c_str ())});
    }
  }
  return prog;
}

VM
run (const Program& prog, long long aStart = 0) {
  VM vm;
  vm.registers[REG_A] = aStart;
  while (vm.ip >= 0 && vm.ip < (int)prog.size ()) {
    const Instruction& current = prog.at (vm.ip);
    if (current.opcode == HALF) {
      vm.registers[current.reg] /= 2;
      ++vm.ip;
    }
    else if (current.opcode == TRIPLE) {
      vm.registers[current.reg] *= 3;
      ++vm.ip;
    }
    else if (current.opcode == INCREMENT) {
      ++vm.registers[current.reg];
      ++vm.ip;
    }
    else if (current.opcode == JUMP) {
      vm.ip += current.offset;
    }
    else if (current.opcode == JUMP_EVEN) {
      if (vm.registers[current.reg] % 2 == 0) {
        vm.ip += current.offset;
      }
      else {
        ++vm.ip;
      }
    }
    else if (current.opcode == JUMP_ONE) {
      if (vm.registers[current.reg] == 1) {
        vm.ip += current.offset;
      }
      else {
        ++vm.ip;
      }
    }
    else {
      throw "Impossible";
    }
  }
  return vm;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Program prog = readInput ();
  std::cout << run (prog, 0).registers[REG_B] << "\n";
  std::cout << run (prog, 1).registers[REG_B] << "\n";
  return 0;
}
