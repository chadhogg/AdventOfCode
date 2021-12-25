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



constexpr Number OPCODE_ADD = 1;
constexpr Number OPCODE_MULT = 2;
constexpr Number OPCODE_HALT = 99;



constexpr unsigned int valuesInInstruction (Number opcode) {
    switch (opcode) {
        case OPCODE_ADD:
        case OPCODE_MULT:
            return 4U;
        case OPCODE_HALT:
            return 1U;
        default:
            throw std::runtime_error ("Unknown opcode " + std::to_string (opcode));
    }
}



inline bool isValidOpcode (Number num) {
    return num == OPCODE_ADD ||
        num == OPCODE_MULT ||
        num == OPCODE_HALT;
}



using ICProgram = std::vector<Number>;

ICProgram parseProgram (std::string const& str) {
    ICProgram prog;
    std::size_t prev = 0U;
    std::size_t curr = str.find (',', prev + 1U);
    while (curr != std::string::npos) {
        std::string part = str.substr (prev, curr - prev);
        prog.push_back (std::stoi (part));
        prev = curr + 1U;
        curr = str.find (',', prev);
    }
    prog.push_back (std::stoi (str.substr (prev)));
    return prog;
}




class ICComputer {
public:
    ICComputer ();
    ICComputer (ICProgram const& prog);
    ICComputer (std::string const& str);

    void loadProgram (ICProgram const& prog);
    void executeNextInstruction ();
    void executeAllInstructions ();

    inline bool isTerminated () const { return m_terminated; }
    inline unsigned int getInstPointer () const { return m_instPointer; }
    inline int getNumber (unsigned int pos) const { return m_memory.at (pos); }
    //inline void setNumber (unsigned int pos, Number value) { m_memory[pos] = value; }

    std::string toString () const;
private:
    std::vector<Number> m_memory;
    unsigned int m_instPointer;
    bool m_terminated;
};




ICComputer::ICComputer ()
: m_memory {}, m_instPointer {0U}, m_terminated {true} {
}

ICComputer::ICComputer (ICProgram const& prog)
: ICComputer {} {
    loadProgram (prog);
}

ICComputer::ICComputer (std::string const& str)
: ICComputer (parseProgram (str)) {
}



void ICComputer::loadProgram (ICProgram const& prog) {
    m_memory = prog;
    m_instPointer = 0U;
    m_terminated = false;
}


void ICComputer::executeNextInstruction () {
    Number opcode = m_memory.at (m_instPointer);
    if (!isValidOpcode (opcode)) { throw std::runtime_error ("Unknown opcode " + std::to_string (opcode)); }
    if (m_terminated) { throw std::runtime_error ("Program has already halted."); }
    switch (opcode) {
        case OPCODE_ADD: {
            m_memory[m_memory.at (m_instPointer + 3)] = m_memory.at (m_memory.at (m_instPointer + 1)) + m_memory.at (m_memory.at (m_instPointer + 2));
            break;
        }
        case OPCODE_MULT: {
            m_memory[m_memory.at (m_instPointer + 3)] = m_memory.at (m_memory.at (m_instPointer + 1)) * m_memory.at (m_memory.at (m_instPointer + 2));
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
    m_instPointer += valuesInInstruction (opcode);
}

void ICComputer::executeAllInstructions () {
    while (!m_terminated) {
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