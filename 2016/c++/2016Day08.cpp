/// \file 2016Day08.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-08.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <regex>
#include <array>

const std::size_t HEIGHT = 6;
const std::size_t WIDTH = 50;

enum Type {RECT, ROTATEROW, ROTATECOLUMN};

using Screen = std::array<std::array<bool, WIDTH>, HEIGHT>;

struct Instruction
{
  Type type;
  std::size_t firstNum;
  std::size_t secondNum;
};

std::vector<Instruction>
readInput ()
{
  std::vector<Instruction> results;
  std::basic_regex rectEx ("^rect (\\d+)x(\\d+)$");
  std::basic_regex rowEx ("^rotate row y=(\\d+) by (\\d+)$");
  std::basic_regex colEx ("^rotate column x=(\\d+) by (\\d+)$");
  std::string line;
  while (std::getline (std::cin, line)) {
    std::smatch match;
    Type type;
    if (std::regex_match (line, match, rectEx)) {
      type = RECT;
    }
    else if (std::regex_match (line, match, rowEx)) {
      type = ROTATEROW;
    }
    else if (std::regex_match (line, match, colEx)) {
      type = ROTATECOLUMN;
    }
    else {
      throw "Impossible";
    }
    results.push_back ({type, (std::size_t)atoi (match[1].str ().c_str ()), (std::size_t)atoi (match[2].str ().c_str ())});
  }
  return results;
}

void
followInstruction (Screen& screen, const Instruction& inst)
{
  switch (inst.type) {
    case RECT:
    assert (inst.firstNum < WIDTH);
    assert (inst.secondNum < HEIGHT);
    for (std::size_t row = 0; row < inst.secondNum; ++row) {
      for (std::size_t col = 0; col < inst.firstNum; ++col) {
        screen[row][col] = true;
      }
    }
    break;
    case ROTATEROW:
    assert (inst.firstNum < HEIGHT);
    assert (inst.secondNum < WIDTH);
    std::array<bool, WIDTH> newRow;
    for (std::size_t col = 0; col < WIDTH; ++col) {
      newRow[col] = screen[inst.firstNum][(col + WIDTH - inst.secondNum) % WIDTH];
    }
    screen[inst.firstNum] = newRow;
    break;
    case ROTATECOLUMN:
    assert (inst.firstNum < WIDTH);
    assert (inst.secondNum < HEIGHT);
    std::array<bool, HEIGHT> newCol;
    for (std::size_t row = 0; row < HEIGHT; ++row) {
      newCol[row] = screen[(row + HEIGHT - inst.secondNum) % HEIGHT][inst.firstNum];
    }
    for (std::size_t row = 0; row < HEIGHT; ++row) {
      screen[row][inst.firstNum] = newCol[row];
    }
    break;
    default:
    throw "Impossible";
  }
}

Screen
runProgram (const std::vector<Instruction>& instructions)
{
  Screen screen;
  for (std::size_t row = 0; row < HEIGHT; ++row) {
    for (std::size_t col = 0; col < WIDTH; ++col) {
      screen[row][col] = false;
    }
  }
  for (const Instruction& inst : instructions) {
    followInstruction (screen, inst);
  }
  return screen;
}

std::size_t
countOn (const Screen& screen) {
  std::size_t count = 0;
  for (std::size_t row = 0; row < HEIGHT; ++row) {
    for (std::size_t col = 0; col < WIDTH; ++col) {
      if (screen[row][col]) { ++count; }
    }
  }
  return count;
}

void
display (const Screen& screen) {
  for (std::size_t row = 0; row < HEIGHT; ++row) {
    for (std::size_t col = 0; col < WIDTH; ++col) {
      std::cout << (screen[row][col] ? '#' : ' ');
    }
    std::cout << "\n";
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Instruction> input = readInput ();
  Screen screen = runProgram (input);
  std::cout << countOn (screen) << "\n";
  display (screen);
  return 0;
}

