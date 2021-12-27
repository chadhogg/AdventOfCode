#ifndef INTCODE_HPP
#define INTCODE_HPP
/// \file intcode.hpp
/// \author Chad Hogg
/// \brief A header-only library for working with intcode programs.

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

using Number = int;

inline Number extractOpcode (Number instruction) {
    return instruction % 100;
}

inline bool firstParamIsImmediate (Number instruction) {
    return instruction % 1000 / 100;
}

inline bool secondParamIsImmediate (Number instruction) {
    return instruction % 10000 / 1000;
}

constexpr Number FIRST_PARAM_POSITION = 0;
constexpr Number FIRST_PARAM_IMMEDIATE = 100;
constexpr Number SECOND_PARAM_POSITION = 0;
constexpr Number SECOND_PARAM_IMMEDIATE = 1000;


constexpr Number OPCODE_ADD = 1;
constexpr Number OPCODE_MULT = 2;
constexpr Number OPCODE_INPUT = 3;
constexpr Number OPCODE_OUTPUT = 4;
constexpr Number OPCODE_JTRUE = 5;
constexpr Number OPCODE_JFALSE = 6;
constexpr Number OPCODE_LT = 7;
constexpr Number OPCODE_EQ = 8;
constexpr Number OPCODE_HALT = 99;



inline unsigned int valuesInInstruction (Number opcode) {
    switch (opcode) {
        case OPCODE_ADD:
        case OPCODE_MULT:
        case OPCODE_LT:
        case OPCODE_EQ:
            return 4U;
        case OPCODE_JTRUE:
        case OPCODE_JFALSE:
            return 3U;
        case OPCODE_INPUT:
        case OPCODE_OUTPUT:
            return 2U;
        case OPCODE_HALT:
            return 1U;
        default:
            throw std::runtime_error ("Unknown opcode " + std::to_string (opcode));
    }
}



inline bool isValidOpcode (Number num) {
    return num == OPCODE_ADD ||
        num == OPCODE_MULT ||
        num == OPCODE_HALT ||
        num == OPCODE_INPUT ||
        num == OPCODE_OUTPUT ||
        num == OPCODE_JTRUE ||
        num == OPCODE_JFALSE ||
        num == OPCODE_LT ||
        num == OPCODE_EQ;
}



using NumbersList = std::vector<Number>;

NumbersList parseNumbersList (std::string const& str) {
    NumbersList prog;
    if (str != "") {
        std::size_t prev = 0U;
        std::size_t curr = str.find (',', prev + 1U);
        while (curr != std::string::npos) {
            std::string part = str.substr (prev, curr - prev);
            prog.push_back (std::stoi (part));
            prev = curr + 1U;
            curr = str.find (',', prev);
        }
        prog.push_back (std::stoi (str.substr (prev)));
    }
    return prog;
}




class ICComputer {
public:
    ICComputer ();
    ICComputer (NumbersList const& prog, NumbersList const& inputs = {});
    ICComputer (std::string const& prog, std::string const& inputs = "");

    void loadProgram (NumbersList const& prog, NumbersList const& inputs = {});
    void executeNextInstruction ();
    void executeAllInstructions ();
    void executeUntilMissingInput ();

    inline void addInput (Number input) { m_inputs.push_back (input); }

    inline bool isTerminated () const { return m_terminated; }
    inline unsigned int getInstPointer () const { return m_instPointer; }
    inline int getNumber (unsigned int pos) const { return m_memory.at (pos); }
    inline NumbersList getOutputs () const { return m_outputs; }

    std::string toString () const;
private:
    int readFirstOperand (Number instruction) const;
    int readSecondOperand (Number instruction) const;

    NumbersList m_memory;
    unsigned int m_instPointer;
    bool m_terminated;
    NumbersList m_inputs;
    unsigned int m_inputPointer;
    NumbersList m_outputs;
};




ICComputer::ICComputer ()
: m_memory {}, m_instPointer {0U}, m_terminated {true}, m_inputs {}, m_inputPointer {0U}, m_outputs {} {
}

ICComputer::ICComputer (NumbersList const& prog, NumbersList const& inputs)
: ICComputer {} {
    loadProgram (prog, inputs);
}

ICComputer::ICComputer (std::string const& prog, std::string const& inputs)
: ICComputer (parseNumbersList (prog), parseNumbersList (inputs)) {
}



void ICComputer::loadProgram (NumbersList const& prog, NumbersList const& inputs) {
    m_memory = prog;
    m_instPointer = 0U;
    m_terminated = false;
    m_inputs = inputs;
    m_outputs.clear ();
    m_inputPointer = 0U;
}

int ICComputer::readFirstOperand (Number instruction) const {
    Number firstOperand = m_memory.at (m_instPointer + 1);
    if (!firstParamIsImmediate (instruction)) { firstOperand = m_memory.at (firstOperand); }
    return firstOperand;
}

int ICComputer::readSecondOperand (Number instruction) const {
    Number secondOperand = m_memory.at (m_instPointer + 2);
    if (!secondParamIsImmediate (instruction)) { secondOperand = m_memory.at (secondOperand); }
    return secondOperand;
}


void ICComputer::executeNextInstruction () {
    Number instruction = m_memory.at (m_instPointer);
    Number opcode = extractOpcode (instruction);
    bool modifiedIP = false;
    if (!isValidOpcode (opcode)) { throw std::runtime_error ("Unknown opcode " + std::to_string (opcode)); }
    if (m_terminated) { throw std::runtime_error ("Program has already halted."); }
    switch (opcode) {
        case OPCODE_ADD: {
            m_memory[m_memory.at (m_instPointer + 3)] = readFirstOperand (instruction) + readSecondOperand (instruction);
            break;
        }
        case OPCODE_MULT: {
            m_memory[m_memory.at (m_instPointer + 3)] = readFirstOperand (instruction) * readSecondOperand (instruction);
            break;
        }
        case OPCODE_INPUT: {
            m_memory[m_memory.at (m_instPointer + 1)] = m_inputs.at (m_inputPointer);
            ++m_inputPointer;
            break;
        }
        case OPCODE_OUTPUT: {
            m_outputs.push_back (readFirstOperand (instruction));
            break;
        }
        case OPCODE_JTRUE: {
            if (readFirstOperand (instruction) != 0) {
                m_instPointer = readSecondOperand (instruction);
                modifiedIP = true;
            }
            break;
        }
        case OPCODE_JFALSE: {
            if (readFirstOperand (instruction) == 0) {
                m_instPointer = readSecondOperand (instruction);
                modifiedIP = true;
            }
            break;
        }
        case OPCODE_LT: {
            m_memory[m_memory.at (m_instPointer + 3)] = (readFirstOperand (instruction) < readSecondOperand (instruction));
            break;
        }
        case OPCODE_EQ: {
            m_memory[m_memory.at (m_instPointer + 3)] = (readFirstOperand (instruction) == readSecondOperand (instruction));
            break;
        }
        case OPCODE_HALT: {
            m_terminated = true;
           break;
        }
        default: {
            throw std::runtime_error ("I haven't implemented opcode " + std::to_string (opcode) + " yet.");
        }
    }
    if (!modifiedIP) { m_instPointer += valuesInInstruction (opcode); }
}

void ICComputer::executeAllInstructions () {
    while (!m_terminated) {
        executeNextInstruction ();
    }
}

void ICComputer::executeUntilMissingInput () {
    while (!m_terminated) {
        if (extractOpcode (m_memory[m_instPointer]) == OPCODE_INPUT && m_inputPointer == m_inputs.size ()) {
            return;
        }
        executeNextInstruction ();
    }
}

std::string ICComputer::toString () const {
    std::stringstream stream;
    stream << m_memory.at (0U);
    unsigned int pos = 1U;
    while (pos < m_memory.size ()) {
        stream << "," << m_memory.at (pos);
        ++pos;
    }
    return stream.str ();
}

#endif//INTCODE_HPP