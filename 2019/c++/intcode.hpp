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

using Number = long;

enum ParamMode {
    ABSOLUTE = 0, 
    IMMEDIATE = 1, 
    RELATIVE = 2
};

enum Opcode {
    OPCODE_ADD = 1,
    OPCODE_MULT = 2,
    OPCODE_INPUT = 3,
    OPCODE_OUTPUT = 4,
    OPCODE_JTRUE = 5,
    OPCODE_JFALSE = 6,
    OPCODE_LT = 7,
    OPCODE_EQ = 8,
    OPCODE_RELBASE = 9,
    OPCODE_HALT = 99
};



inline Number extractOpcode (Number instruction) {
    return instruction % 100;
}


inline ParamMode extractParamMode (Number instruction, int param) {
    int value;
    switch (param) {
        case 1: value = instruction % 1000 / 100; break;
        case 2: value = instruction % 10000 / 1000; break;
        case 3: value = instruction % 100000 / 10000; break;
        default: throw std::runtime_error ("Unknown parameter index " + std::to_string (param));
    }
    switch (value) {
        case 0: return ABSOLUTE;
        case 1: return IMMEDIATE;
        case 2: return RELATIVE;
        default: throw std::runtime_error ("Unknown parameter type " + std::to_string (value));
    }
}


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
        case OPCODE_RELBASE:
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
        num == OPCODE_EQ ||
        num == OPCODE_RELBASE;
}



using NumbersList = std::vector<Number>;

NumbersList parseNumbersList (std::string const& str) {
    NumbersList prog;
    if (str != "") {
        std::size_t prev = 0U;
        std::size_t curr = str.find (',', prev + 1U);
        while (curr != std::string::npos) {
            std::string part = str.substr (prev, curr - prev);
            prog.push_back (std::stol (part));
            prev = curr + 1U;
            curr = str.find (',', prev);
        }
        prog.push_back (std::stol (str.substr (prev)));
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
    inline int getNumber (unsigned int pos) const { return readMemoryAddress (pos); }
    inline NumbersList getOutputs () const { return m_outputs; }

    std::string toString () const;
private:
    Number readParameter (Number instruction, unsigned int index) const;
    Number readMemoryAddress (Number address) const;
    void writeMemoryAddress (Number address, Number value);
    inline Number computeWriteAddress (Number instruction, unsigned int index) const {return readMemoryAddress (m_instPointer + index) + (extractParamMode (instruction, index) == RELATIVE ? m_relativeBase : 0); }

    NumbersList m_memory;
    unsigned int m_instPointer;
    bool m_terminated;
    NumbersList m_inputs;
    unsigned int m_inputPointer;
    NumbersList m_outputs;
    Number m_relativeBase;
};




ICComputer::ICComputer ()
: m_memory {}, m_instPointer {0U}, m_terminated {true}, m_inputs {}, m_inputPointer {0U}, m_outputs {}, m_relativeBase {0U} {
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
    m_relativeBase = 0U;
}

Number ICComputer::readParameter (Number instruction, unsigned int index) const {
    Number operand = readMemoryAddress (m_instPointer + index);
    switch (extractParamMode (readMemoryAddress (m_instPointer), index)) {
        case ABSOLUTE: {
            operand = readMemoryAddress (operand);
            break;
        }
        case IMMEDIATE: {
            break;
        }
        case RELATIVE: {
            operand = readMemoryAddress (m_relativeBase + operand);
            break;
        }
        default: {
            throw std::runtime_error ("Unknown parameter mode " + std::to_string (extractParamMode (readMemoryAddress (m_instPointer), index)));
        }
    }
    return operand;
}

Number ICComputer::readMemoryAddress (Number address) const {
    if ((unsigned Number)address < m_memory.size ()) { return m_memory.at (address); }
    else { return 0; }
}

void ICComputer::writeMemoryAddress (Number address, Number value) {
    while (m_memory.size () <= (unsigned Number)address) { m_memory.push_back (0); }
    m_memory[address] = value;
}


void ICComputer::executeNextInstruction () {
    Number instruction = m_memory.at (m_instPointer);
    Number opcode = extractOpcode (instruction);
    bool modifiedIP = false;
    if (!isValidOpcode (opcode)) { throw std::runtime_error ("Unknown opcode " + std::to_string (opcode)); }
    if (m_terminated) { throw std::runtime_error ("Program has already halted."); }
    switch (opcode) {
        case OPCODE_ADD: {
            writeMemoryAddress (computeWriteAddress (instruction, 3), readParameter (instruction, 1) + readParameter (instruction, 2));
            break;
        }
        case OPCODE_MULT: {
            writeMemoryAddress (computeWriteAddress (instruction, 3), readParameter (instruction, 1) * readParameter (instruction, 2));
            break;
        }
        case OPCODE_INPUT: {
            writeMemoryAddress (computeWriteAddress (instruction, 1), m_inputs.at (m_inputPointer));
            ++m_inputPointer;
            break;
        }
        case OPCODE_OUTPUT: {
            m_outputs.push_back (readParameter (instruction, 1));
            break;
        }
        case OPCODE_JTRUE: {
            if (readParameter (instruction, 1) != 0) {
                m_instPointer = readParameter (instruction, 2);
                modifiedIP = true;
            }
            break;
        }
        case OPCODE_JFALSE: {
            if (readParameter (instruction, 1) == 0) {
                m_instPointer = readParameter (instruction, 2);
                modifiedIP = true;
            }
            break;
        }
        case OPCODE_LT: {
            writeMemoryAddress (computeWriteAddress (instruction, 3), readParameter (instruction, 1) < readParameter (instruction, 2));
            break;
        }
        case OPCODE_EQ: {
            writeMemoryAddress (computeWriteAddress (instruction, 3), readParameter (instruction, 1) == readParameter (instruction, 2));
            break;
        }
        case OPCODE_RELBASE: {
            m_relativeBase += readParameter (instruction, 1);
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