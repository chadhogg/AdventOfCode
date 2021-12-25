/// \file 2021Day24.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/24.

#define NDEBUG

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_set>

#include "utilities.hpp"

inline bool isRegister (std::string const& str) { 
    return str == "w" || str == "x" || str == "y" || str == "z";
}

struct VMState {
    short w, y;
    int x, z;
    //int w, x, y, z;
    std::string input;
    unsigned short consumed;
    VMState () : w {0}, x {0}, y {0}, z {0}, input {}, consumed {0U} {};
    VMState (int ww, int xx, int yy, int zz, std::string i, unsigned int c)
        : w (ww), x (xx), y (yy), z (zz), input {i}, consumed (c) {}
};

// NOTE: We very deliberately don't consider the input as relevant, because we are goin to be building it iteratively and only want to keep largest.
bool operator== (VMState const& first, VMState const& second) {
    return first.w == second.w && first.x == second.x && first.y == second.y && first.z == second.z;
}

template<>
struct std::hash<VMState> {
    std::size_t operator() (VMState const& state) const {
        std::size_t hash = 17;
        hash = hash * 31 + state.w;
        hash = hash * 31 + state.x;
        hash = hash * 31 + state.y;
        hash = hash * 31 + state.z;
        return hash;
    }
};

struct Instruction {
    std::string opcode;
    std::string op1;
    std::string op2;

    void execute (VMState & state) const {
        if (opcode == "inp") { executeInput (state); }
        else if (opcode == "add") { executeAdd (state); }
        else if (opcode == "mul") { executeMul (state); }
        else if (opcode == "div") { executeDiv (state); }
        else if (opcode == "mod") { executeMod (state); }
        else if (opcode == "eql") { executeEql (state); }
        else { throw std::runtime_error ("Unknown opcode \"" + opcode + "\""); }
    }

    void executeInput (VMState & state) const {
        assert (state.consumed < state.input.size ());
        int value = state.input.at (state.consumed) - '0';
        ++state.consumed;
        assert (value >= 1 && value <= 9);
        setValue (op1, value, state);
    }

    void executeAdd (VMState & state) const {
        assert (isRegister (op1));
        int operand1 = getValue (op1, state);
        int operand2 = getValue (op2, state);
        setValue (op1, operand1 + operand2, state);
    }

    void executeMul (VMState & state) const {
        assert (isRegister (op1));
        int operand1 = getValue (op1, state);
        int operand2 = getValue (op2, state);
        setValue (op1, operand1 * operand2, state);
    }

    void executeDiv (VMState & state) const {
        assert (isRegister (op1));
        int operand1 = getValue (op1, state);
        int operand2 = getValue (op2, state);
        setValue (op1, operand1 / operand2, state);
    }

    void executeMod (VMState & state) const {
        assert (isRegister (op1));
        int operand1 = getValue (op1, state);
        int operand2 = getValue (op2, state);
        setValue (op1, operand1 % operand2, state);
    }

    void executeEql (VMState & state) const {
        assert (isRegister (op1));
        int operand1 = getValue (op1, state);
        int operand2 = getValue (op2, state);
        setValue (op1, operand1 == operand2, state);
    }

    inline int getValue (std::string const& operand, VMState const& state) const {
        if (operand == "w") { return state.w; }
        else if (operand == "x") { return state.x; }
        else if (operand == "y") { return state.y; }
        else if (operand == "z") { return state.z; }
        else { return std::stoi (operand); }
    }

    inline void setValue (std::string const& operand, int value, VMState & state) const {
        if (operand == "w") { state.w = value; }
        else if (operand == "x") { state.x = value; }
        else if (operand == "y") { state.y = value; }
        else if (operand == "z") { state.z = value; }
        else { throw std::runtime_error ("Unknown register name \"" + operand + "\""); }
    }
};

using Program = std::vector<Instruction>;

Program getInput () {
    Program prog;
    std::string opcode;
    while (std::cin >> opcode) {
        if (opcode == "inp") {
            std::string op1 = read<std::string> ();
            assert (isRegister (op1));
            prog.push_back (Instruction {opcode, op1, ""});
        } else if (opcode == "add" || opcode == "mul" || opcode == "div" || opcode == "mod" || opcode == "eql") {
            std::string op1 = read<std::string> ();
            std::string op2 = read<std::string> ();
            assert (isRegister (op1));
            prog.push_back (Instruction {opcode, op1, op2});
        } else {
            throw std::runtime_error ("Unknown opcode \"" + opcode + "\"");
        }
    }
    return prog;
}

bool valid (Program const& prog, VMState const& state) {
    VMState current {state};
    for (Instruction const& inst : prog) {
        inst.execute (current);
    }
    return current.z == 0;
}

bool validateBelief (Program const& prog, unsigned int startInstruction, std::vector<VMState> const& shouldBeValid, std::vector<VMState> const& shouldBeInvalid) {
    for (VMState state : shouldBeValid) {
        for (unsigned int index {startInstruction}; index < prog.size (); ++index) {
            prog.at (index).execute (state);
        }
        if (state.z != 0) {
            assert (false);
            return false;
        }
    }
    for (VMState state : shouldBeInvalid) {
        for (unsigned int index {startInstruction}; index < prog.size (); ++index) {
            prog.at (index).execute (state);
        }
        if (state.z == 0) {
            assert (false);
            return false;
        }
    }
    return true;
}

bool validateAllBeliefs (Program const& prog) {
    unsigned int index = prog.size ();
    int a {0}, b {0}, c {0}, d {0}, e {0}, f {0}, g {0}, h {0}, i {0}, j {0}, k {0}, l {0}, m {0}, n {0}, w {0}, x {0}, y {0}, z {0};
    // After all instructions, states with z=0 are valid and others are invalid.
    w = 0; x = 0; y = 0; z = 0;
    assert (z == 0);
    w = 0; x = 0; y = 0; z = 1;
    assert (z != 0);
    if (!validateBelief (prog, index,
        {
            {0, 0, 0, 0, "", 0U}
        },
        {
            {0, 0, 0, 1, "", 0U}
        }
    )) { return false; }
    --index;

    // Before instruction 252, we need z == -y 
    w = 0; x = 0; y = 0; z = 0;
    assert (z == -y);
    w = 0; x = 0; y = 5; z = -5;
    assert (z == -y);
    w = 1; x = 3; y = -2; z = 2;
    assert (z == -y);
    w = 0; x = 1, y = 2; z = 3;
    assert (z != -y);
    w = 4; x = 3; y = 2; z = 0;
    assert (z != -y);
    w = 1; x = -1; y = 0; z = 3;
    assert (z != y);
    if (!validateBelief (prog, index,
        {
            {0, 0, 0, 0, "", 0U},
            {0, 0, 5, -5, "", 0U},
            {1, 3, -2, 2, "", 0U}
        },
        {
            {0, 1, 2, 3, "", 0U},
            {4, 3, 2, 0, "", 0U},
            {1, -1, 0, 3, "", 0U}
        }
    )) { return false; }
    --index;

    // Before instruction 251, we need z == -(y * x)
    w = 0; x = 0; y = 0; z = 0;
    assert (z == -(y * x));
    w = 0; x = -1; y = 5; z = 5;
    assert (z == -(y * x));
    w = 0; x = 2; y = 2; z = -4;
    assert (z == -(y * x));
    w = 1; x = 2; y = 3; z = 4;
    assert (z != -(y * x));
    w = 1; x = 2; y = 2; z = 4;
    assert (z != -(y * x));
    w = 1; x = -2; y = -2; z = 4;
    assert (z != -(y * x));
    if (!validateBelief (prog, index,
        {
            {0, 0, 0, 0, "", 0U},
            {0, -1, 5, 5, "", 0U},
            {0, 2, 2, -4, "", 0U}
        },
        {
            {1, 2, 3, 4, "", 0U},
            {1, 2, 2, 4, "", 0U},
            {1, -2, -2, 4, "", 0U}
        }
    )) { return false; }
    --index;

    // Before instruction 250 we need z == -((y + 11) * x)
    w = 0; x = 0; y = 11; z = 0;
    assert (z == -((y + 11) * x));
    w = 20; x = 5; y = -1; z = -50;
    assert (z == -((y + 11) * x));
    w = 0; x = -3; y = -7; z = 12;
    assert (z == -((y + 11) * x));
    w = 0; x = 1; y = 2; z = 0;
    assert (z != -((y + 11) * x));
    w = 1; x = 2; y = 3; z = 4;
    assert (z != -((y + 11) * x));
    w = 0; x = 2; y = 2; z = -4;
    assert (z != -((y + 11) * x));
    if (!validateBelief (prog, index,
        {
            {0, 0, 11, 0, "", 0U},
            {20, 5, -1, -50, "", 0U},
            {0, -3, -7, 12, "", 0U}
        },
        {
            {0, 1, 2, 0, "", 0U},
            {1, 2, 3, 4, "", 0U},
            {0, 2, 2, -4, "", 0U}
        }
    )) { return false; }
    --index;

    std::cout << a << b << c << d << e << f << g << h << i << j << k << l << m << n << w << x << y << z << "\n";
    std::cout << "All beliefs appear to be true!\n";
    return true;
}

void addIfMissingReplaceIfMoreExtreme (std::unordered_set<VMState> & set, VMState const& state, bool largest) {
    auto iter = set.find (state);
    if (iter == set.end ()) {
        set.insert (state);
    }
    else {
        VMState const& other = *iter;
        if ((largest && state.input > other.input) || (!largest && state.input < other.input)) {
            set.erase (iter);
            set.insert (state);
        }
    }
}

std::string findMostExtreme (std::unordered_set<VMState> const& set, bool largest) {
    if (largest) {
        std::string best = "11111111111111";
        for (VMState const& state : set) {
            if (state.z != 0) { continue; }
            if (state.input > best) { best = state.input; }
        }
        return best;
    }
    else {
        std::string best = "99999999999999";
        for (VMState const& state : set) {
            if (state.z != 0) { continue; }
            if (state.input < best) { best = state.input; }
        }
        return best;
    }
}

std::string findExtremeValidValue (Program const& prog, bool largest) {
    std::unordered_set<VMState> inputs;
    inputs.insert ({0, 0, 0, 0, "", 0U});
    unsigned int count {0U};
    for (Instruction const& inst : prog) {
        std::unordered_set<VMState> outputs;
        for (VMState state : inputs) {
            if (inst.opcode == "inp") {
                for (char input = '1'; input <= '9'; ++input) {
                    if (state.consumed == 0 && input != '1') { continue; }
                    VMState successor {state};
                    successor.input += input;
                    inst.execute (successor);
                    addIfMissingReplaceIfMoreExtreme (outputs, successor, largest);
                }
            }
            else {
                inst.execute (state);
                addIfMissingReplaceIfMoreExtreme (outputs, state, largest);
            }
        }
        std::cout << "After instruction " << count << " there are " << outputs.size () << " distinct possible states.\n";
        inputs = outputs;
        ++count;
    }
    return findMostExtreme (inputs, largest);
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    Program prog = getInput ();
    //std::cout << prog.size () << "\n";
    //std::cout << valid (prog, {0, 0, 0, 0, "99998239999696", 0U}) << "\n";
    //validateAllBeliefs (prog);
    //std::cout << findExtremeValidValue (prog, true) << "\n";
    std::cout << findExtremeValidValue (prog, false) << "\n";
    return 0;
}




/*

inp w
mul x 0
add x z
mod x 26
div z 1
add x 14
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 12
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 15
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 7
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 1
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 2
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -5
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 4
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 14
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 15
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 15
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 11
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -13
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 5
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -16
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 3
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -8
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 9
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 15
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 2
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -8
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 3
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x 0
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 3
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -4
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w

    Requirements:
        z == -((y - 11) * x)
    Notes:

250 add y 11

    Requirements:
        z == -(y * x)
    Notes:

251 mul y x

    Requirements:
        z == -y
    Notes:

252 add z y

    Requirements:
        z == 0
    Notes:

a b c d e f g h i j k l m n
*/