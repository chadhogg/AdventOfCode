/// \file 2021Day02.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/2

#include <iostream>
#include <vector>
#include <string>

/// \brief An instruction, which can be interpreted one way in part 1 and a different way in part 2.
struct Instruction { std::string command; int amount; };
/// \brief The position of the submarine in horizontal-depth space.
struct Position { int horiz; int depth; };
/// \brief The position of the submarine in horizontal-depth-aim space.
struct PosWithAim { Position pos; int aim; };


/// \brief Reads in a series of instructions from standard input.
/// \return A vector of instructions.
std::vector<Instruction> getInput () {
    std::vector<Instruction> instructions;
    std::string s;
    int n;
    while (std::cin >> s >> n) {
        instructions.push_back ({s, n});
    }
    return instructions;
}

/// \brief Applies instructions to a horizontal-depth position.
/// \param[in] instructions The instructions to run.
/// \param[inout] pos The position that should be updated.
void followInstructions (std::vector<Instruction> const& instructions, Position & pos) {
    for (Instruction const& inst : instructions) {
        if(inst.command == "forward") {
            pos.horiz += inst.amount;
        }
        else if(inst.command == "down") {
            pos.depth += inst.amount;
        }
        else if(inst.command == "up") {
            pos.depth -= inst.amount;
        }
        else {
            throw std::runtime_error ("Unknown command type \"" + inst.command + "\"\n");
        }
    }
}

/// \brief Applies instructions to a horizontal-depth-aim position.
/// \param[in] instructions The instructions to run.
/// \param[inout] pos The position that should be updated.
void followInstructions2 (std::vector<Instruction> const& instructions, PosWithAim & pos) {
    for (Instruction const& inst : instructions) {
        if (inst.command == "forward") {
            pos.pos.horiz += inst.amount;
            pos.pos.depth += pos.aim * inst.amount;
        }
        else if (inst.command == "down") {
            pos.aim += inst.amount;
        }
        else if (inst.command == "up") {
            pos.aim -= inst.amount;
        }
        else {
            throw std::runtime_error ("Unknown command type \"" + inst.command + "\"\n");
        }
    }
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    std::vector<Instruction> instructions = getInput ();
    Position pos {0, 0};
    followInstructions (instructions, pos);
    std::cout << pos.horiz * pos.depth << std::endl;
    PosWithAim pos2 {Position {0, 0}, 0};
    followInstructions2 (instructions, pos2);
    std::cout << pos2.pos.horiz * pos2.pos.depth << std::endl;
    return 0;
}