/// \file 2016Day12.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-12.


#include <iostream>
#include <string>
#include <cassert>
#include <array>
#include <vector>

using Value = long long;

const char LOWEST_REGISTER = 'a';
const int NUM_REGISTERS = 4;

int
regLetterToIndex (char c) {
  assert (c >= 'a' && c - 'a' < NUM_REGISTERS);
  return c - 'a';
}

char
regIndexToLetter (int index) {
  assert (index >= 0 && index < NUM_REGISTERS);
  return 'a' + index;
}

struct Computer
{
  Computer () : registers ({0, 0, 0, 0}), ip (0) {}
  std::array<Value, NUM_REGISTERS> registers;
  std::size_t ip;
};

struct Instruction
{
  virtual ~Instruction () {}
  virtual void execute (Computer& comp) const = 0;
};

struct CopyRegInst : public Instruction
{
  const char srcReg;
  const char dstReg;
  virtual ~CopyRegInst () {}
  CopyRegInst (char s, char d) : srcReg (s), dstReg (d) {}
  virtual void execute (Computer& comp) const
  {
    comp.registers[regLetterToIndex (dstReg)] = comp.registers[regLetterToIndex (srcReg)];
    ++comp.ip;
  }
};

struct CopyImmInst : public Instruction
{
  const Value srcVal;
  const char dstReg;
  virtual ~CopyImmInst () {}
  CopyImmInst (Value v, char d) : srcVal (v), dstReg (d) {}
  virtual void execute (Computer& comp) const
  {
    comp.registers[regLetterToIndex (dstReg)] = srcVal;
    ++comp.ip;
  }
};

struct IncInst : public Instruction
{
  const char reg;
  virtual ~IncInst () {}
  IncInst (char r) : reg (r) {}
  virtual void execute (Computer& comp) const
  {
    ++comp.registers[regLetterToIndex (reg)];
    ++comp.ip;
  }
};

struct DecInst : public Instruction
{
  const char reg;
  virtual ~DecInst () {}
  DecInst (char r) : reg (r) {}
  virtual void execute (Computer& comp) const
  {
    --comp.registers[regLetterToIndex (reg)];
    ++comp.ip;
  }
};

struct JnzRegInst : public Instruction
{
  const char reg;
  const int amount;
  virtual ~JnzRegInst () {}
  JnzRegInst (char r, int a) : reg (r), amount (a) {}
  virtual void execute (Computer& comp) const
  {
    if (comp.registers[regLetterToIndex (reg)] == 0) {
      ++comp.ip;
    }
    else {
      comp.ip += amount;
    }
  }
};

struct JnzImmInst : public Instruction
{
  const int value;
  const int amount;
  virtual ~JnzImmInst () {}
  JnzImmInst (int v, int a) : value (v), amount (a) {}
  virtual void execute (Computer& comp) const
  {
    if (value == 0) {
      ++comp.ip;
    }
    else {
      comp.ip += amount;
    }
  }
};

struct Program
{
  ~Program () 
  {
    for (auto iter = instructions.begin (); iter != instructions.end (); ++iter) {
      delete *iter;
    }
  }
  std::vector<Instruction*> instructions;
};

Program
readProgram ()
{
  Program program;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::vector<std::string> tokens;
    auto index = line.find (' ');
    while (index != std::string::npos) {
      tokens.push_back (line.substr (0, index));
      line = line.substr (index + 1);
      index = line.find (' ');
    }
    tokens.push_back (line);
    assert (tokens.size () >= 2);
    if (tokens[0] == "cpy") {
      assert (tokens.size () == 3);
      assert (tokens[2].size () == 1 && isalpha (tokens[2][0]));
      if (isalpha (tokens[1][0])) {
        assert (tokens[1].size () == 1);
        program.instructions.push_back (new CopyRegInst (tokens[1][0], tokens[2][0]));
      }
      else {
        program.instructions.push_back (new CopyImmInst (atoi (tokens[1].c_str ()), tokens[2][0]));
      }
    }
    else if (tokens[0] == "inc") {
      assert (tokens.size () == 2);
      assert (tokens[1].size () == 1 && isalpha (tokens[1][0]));
      program.instructions.push_back (new IncInst (tokens[1][0]));
    }
    else if (tokens[0] == "dec") {
      assert (tokens.size () == 2);
      assert (tokens[1].size () == 1 && isalpha (tokens[1][0]));
      program.instructions.push_back (new DecInst (tokens[1][0])); 
    }
    else if (tokens[0] == "jnz") {
      assert (tokens.size () == 3);
      if (isalpha (tokens[1][0])) {
        assert (tokens[1].size () == 1);
        assert (isdigit (tokens[2][0]) || tokens[2][0] == '-');
        program.instructions.push_back (new JnzRegInst (tokens[1][0], atoi (tokens[2].c_str ())));
      }
      else {
        assert (isdigit (tokens[1][0]) || tokens[1][0] == '-');
        assert (isdigit (tokens[2][0]) || tokens[2][0] == '-');
        program.instructions.push_back (new JnzImmInst (atoi (tokens[1].c_str ()), atoi (tokens[2].c_str ())));
      }
    }
    else {
      assert (false);
    }
  }
  return program;
}

void
execute (Computer& comp, const Program& prog)
{
  while (comp.ip < prog.instructions.size ()) {
    prog.instructions[comp.ip]->execute (comp);
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Program program = readProgram ();
  Computer comp;
  execute (comp, program);
  std::cout << comp.registers[regLetterToIndex ('a')] << "\n";
  Computer comp2;
  comp2.registers[regLetterToIndex ('c')] = 1;
  execute (comp2, program);
  std::cout << comp2.registers[regLetterToIndex ('a')] << "\n";
  return 0;
}

