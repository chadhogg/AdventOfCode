/// \file 2021Day18.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/18.

#include <iostream>
#include <string>
#include <cassert>
#include <deque>

#include "utilities.hpp"

constexpr unsigned int EXPLODE_DEPTH = 4U;
constexpr unsigned int SPLIT_LEVEL = 10U;

class Snailfish;
class SnailfishLiteral;
class SnailfishPair;

class Snailfish {
public:
    Snailfish (SnailfishPair* parent) : m_parent (parent) {}
    virtual ~Snailfish () {}
    Snailfish (Snailfish const& other) = delete;
    Snailfish& operator= (Snailfish const& other) = delete;
    Snailfish (Snailfish const&& other) = delete;
    Snailfish& operator= (Snailfish const&& other) = delete;
    virtual std::string toString () const = 0;
    unsigned int depth () const;
    virtual SnailfishLiteral* findNextNumberToLeft () = 0;
    virtual SnailfishLiteral* findNextNumberToRight () = 0;
    virtual SnailfishLiteral* descendLeft () = 0;
    virtual SnailfishLiteral* descendRight () = 0;
    virtual unsigned long magnitude () const = 0;
public:
    SnailfishPair* m_parent;
};

std::ostream& operator<< (std::ostream& out, Snailfish const& snail) {
    out << snail.toString ();
    return out;
}

class SnailfishLiteral : public Snailfish {
public:
    SnailfishLiteral (SnailfishPair* parent, unsigned int value) : Snailfish (parent), m_value (value) {}
    virtual ~SnailfishLiteral () {}
    SnailfishLiteral (SnailfishLiteral const& other) = delete;
    SnailfishLiteral& operator= (SnailfishLiteral const& other) = delete;
    SnailfishLiteral (SnailfishLiteral const&& other) = delete;
    SnailfishLiteral& operator= (SnailfishLiteral const&& other) = delete;
    virtual std::string toString () const { return std::to_string (m_value); }
    virtual SnailfishLiteral* findNextNumberToLeft () { return this; }
    virtual SnailfishLiteral* findNextNumberToRight () { return this; }
    virtual SnailfishLiteral* descendLeft () { return this; }
    virtual SnailfishLiteral* descendRight () { return this; }
    virtual unsigned long magnitude () const { return m_value; }
public:
    unsigned int m_value;
};

class SnailfishPair : public Snailfish {
public:
    SnailfishPair (SnailfishPair* parent, Snailfish* first, Snailfish* second) : Snailfish (parent), m_left (first), m_right (second) {}
    virtual ~SnailfishPair () { delete m_left; delete m_right; }
    SnailfishPair (SnailfishPair const& other) = delete;
    SnailfishPair& operator= (SnailfishPair const& other) = delete;
    SnailfishPair (SnailfishPair const&& other) = delete;
    SnailfishPair& operator= (SnailfishPair const&& other) = delete;
    virtual std::string toString () const { return "[" + m_left->toString () + "," + m_right->toString () + "]"; }
    void fullReduce () {
        while (singleReduce ()) {
            /* intentionally empty */
        }
    }
    bool singleReduce () {
        bool change = explodeFirstEligible ();
        if (!change) {
            change = splitFirstEligible ();
        }
        return change;
    }
    virtual SnailfishLiteral* findNextNumberToLeft () {
        if (m_parent == NULL) {
            return NULL;
        }
        else if (m_parent->m_right == this) {
            return m_parent->m_left->descendRight ();
        }
        else {
            return m_parent->findNextNumberToLeft ();
        }
    }
    virtual SnailfishLiteral* findNextNumberToRight () {
        if (m_parent == NULL) {
            return NULL;
        }
        else if (m_parent->m_left == this) {
            return m_parent->m_right->descendLeft ();
        }
        else {
            return m_parent->findNextNumberToRight ();
        }
    }
    virtual SnailfishLiteral* descendLeft () { return m_left->descendLeft (); }
    virtual SnailfishLiteral* descendRight () { return m_right->descendRight (); }
    void explode () {
        assert (depth () == EXPLODE_DEPTH);
        SnailfishLiteral* numToLeft = findNextNumberToLeft ();
        if (numToLeft != NULL) {
            numToLeft->m_value += ((SnailfishLiteral*)m_left)->m_value;
        }
        SnailfishLiteral* numToRight = findNextNumberToRight ();
        if (numToRight != NULL) {
            numToRight->m_value += ((SnailfishLiteral*)m_right)->m_value;
        }
    }
    bool explodeFirstEligible () {
        if (dynamic_cast<SnailfishPair*> (m_left)) {
            if (m_left->depth () == EXPLODE_DEPTH) {
                dynamic_cast<SnailfishPair*> (m_left)->explode ();
                delete m_left;
                m_left = new SnailfishLiteral (this, 0);
                return true;
            }
            else {
                if (dynamic_cast<SnailfishPair*>(m_left)->explodeFirstEligible ()) {
                    return true;
                }
            }
        }
        if (dynamic_cast<SnailfishPair*> (m_right)) {
            if (m_right->depth () == EXPLODE_DEPTH) {
                dynamic_cast<SnailfishPair*> (m_right)->explode ();
                delete m_right;
                m_right = new SnailfishLiteral (this, 0);
                return true;
            }
            else {
                if (dynamic_cast<SnailfishPair*>(m_right)->explodeFirstEligible ()) {
                    return true;
                }
            }
        }
        return false;
    }
    bool splitFirstEligible () {
        if (dynamic_cast<SnailfishLiteral*> (m_left)) {
            unsigned int value = dynamic_cast<SnailfishLiteral*> (m_left)->m_value;
            if (value >= SPLIT_LEVEL) {
                delete m_left;
                m_left = new SnailfishPair (this, NULL, NULL);
                SnailfishPair* child = dynamic_cast<SnailfishPair*> (m_left);
                child->m_left = new SnailfishLiteral (child, value / 2);
                child->m_right = new SnailfishLiteral (child, (value + 1) / 2);
                return true;
            }
        }
        else {
            if (dynamic_cast<SnailfishPair*>(m_left)->splitFirstEligible ()) {
                return true;
            }
        }
        if (dynamic_cast<SnailfishLiteral*> (m_right)) {
            unsigned int value = dynamic_cast<SnailfishLiteral*> (m_right)->m_value;
            if (value >= SPLIT_LEVEL) {
                delete m_right;
                m_right = new SnailfishPair (this, NULL, NULL);
                SnailfishPair* child = dynamic_cast<SnailfishPair*> (m_right);
                child->m_left = new SnailfishLiteral (child, value / 2);
                child->m_right = new SnailfishLiteral (child, (value + 1) / 2);
                return true;
            }
        }
        else {
            if (dynamic_cast<SnailfishPair*>(m_right)->splitFirstEligible ()) {
                return true;
            }
        }
        return false;
    }
    virtual unsigned long magnitude () const { return 3 * m_left->magnitude () + 2 * m_right->magnitude (); }

public:
    Snailfish* m_left;
    Snailfish* m_right;
};

unsigned int Snailfish::depth () const {
        if (m_parent == NULL) { return 0; }
        else { return 1U + m_parent->depth (); }
}

SnailfishPair* parse (std::deque<char> & line, SnailfishPair* parent) {
    char next = line.front ();
    line.pop_front ();
    assert (next == '[');
    SnailfishPair* pair = new SnailfishPair (parent, NULL, NULL);
    if (std::isdigit (line.front ())) {
        pair->m_left = new SnailfishLiteral (pair, line.front () - '0');
        line.pop_front ();
    }
    else {
        pair->m_left = parse (line, pair);
    }
    next = line.front ();
    line.pop_front ();
    assert (next == ',');
    if (std::isdigit (line.front ())) {
        pair->m_right = new SnailfishLiteral (pair, line.front () - '0');
        line.pop_front ();
    }
    else {
        pair->m_right = parse (line, pair);
    }
    next = line.front ();
    line.pop_front ();
    assert (next == ']');
    return pair;
}

std::vector<SnailfishPair*> getInput () {
    std::vector<SnailfishPair*> numbers;
    std::string line;
    while (std::cin >> line) {
        std::deque<char> deque (line.begin (), line.end ());
        numbers.push_back (parse (deque, NULL));
    }
    return numbers;
}

SnailfishPair* addAndReduce (SnailfishPair* first, SnailfishPair* second) {
    SnailfishPair* result = new SnailfishPair (NULL, first, second);
    first->m_parent = result;
    second->m_parent = result;
    result->fullReduce ();
    return result;
}

SnailfishPair* sumNumbers (std::vector<SnailfishPair*> & numbers) {
    SnailfishPair* sum = numbers.front ();
    for (unsigned int index {1U}; index < numbers.size (); ++index) {
        sum = addAndReduce (sum, numbers.at (index));
    }
    return sum;
}

bool singleReduceTest (std::string input, std::string expected) {
    std::deque<char> chars (input.begin (), input.end ());
    SnailfishPair* number = parse (chars, NULL);
    number->singleReduce ();
    std::string result = number->toString ();
    if (result == expected) {
        return true;
    }
    else {
        std::cout << "Input:    " << input << "\n";
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Actual:   " << result << "\n";
        return false;
    }
}

bool multiReduceTest (std::string input, std::string expected) {
    std::deque<char> chars (input.begin (), input.end ());
    SnailfishPair* number = parse (chars, NULL);
    number->fullReduce ();
    std::string result = number->toString ();
    delete number;
    if (result == expected) {
        return true;
    }
    else {
        std::cout << "Input:    " << input << "\n";
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Actual:   " << result << "\n";
        return false;
    }
}

bool additionTest (std::string firstString, std::string secondString, std::string expected) {
    std::deque<char> firstChars (firstString.begin (), firstString.end ());
    SnailfishPair* firstNumber = parse (firstChars, NULL);
    std::deque<char> secondChars (secondString.begin (), secondString.end ());
    SnailfishPair* secondNumber = parse (secondChars, NULL);
    SnailfishPair* sum = addAndReduce (firstNumber, secondNumber);
    std::string result = sum->toString ();
    delete sum;
    if (result == expected) {
        return true;
    }
    else {
        std::cout << "1st Input: " << firstString << "\n";
        std::cout << "2nd Input: " << secondString << "\n";
        std::cout << "Expected:  " << expected << "\n";
        std::cout << "Actual:    " << result << "\n";
        return false;
    }
}

bool magnitudeTest (std::string input, unsigned long expected) {
    std::deque<char> chars (input.begin (), input.end ());
    SnailfishPair* number = parse (chars, NULL);
    unsigned long result = number->magnitude ();
    delete number;
    if (result == expected) {
        return true;
    }
    else {
        std::cout << "Input:    " << input << "\n";
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Actual:   " << result << "\n";
        return false;
    }
}

void runTests () {
    unsigned int failures = 0U;
    // The examples given for single explosions.
    if (!singleReduceTest ("[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]")) { ++failures; }
    if (!singleReduceTest ("[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]")) { ++failures; }
    if (!singleReduceTest ("[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]")) { ++failures; }
    if (!singleReduceTest ("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]")) { ++failures; }
    if (!singleReduceTest ("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[7,0]]]]")) { ++failures; }
    // The example given for doing several reductions.
    if (!multiReduceTest ("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]", "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")) { ++failures; }
    // An example of addition.
    if (!additionTest ("[[[[4,3],4],4],[7,[[8,4],9]]]", "[1,1]", "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")) { ++failures; }
    if (!additionTest ("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]", "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]", "[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]")) { ++failures; }
    if (!additionTest ("[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]", "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]", "[[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]]")) { ++failures; }
    if (!additionTest ("[[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]]", "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]", "[[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]]")) { ++failures; }
    if (!additionTest ("[[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]]", "[7,[5,[[3,8],[1,4]]]]", "[[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]]")) { ++failures; }
    if (!additionTest ("[[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]]", "[[2,[2,2]],[8,[8,1]]]", "[[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]]")) { ++failures; }
    if (!additionTest ("[[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]]", "[2,9]", "[[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]]")) { ++failures; }
    if (!additionTest ("[[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]]", "[1,[[[9,3],9],[[9,0],[0,7]]]]", "[[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]]")) { ++failures; }
    if (!additionTest ("[[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]]", "[[[5,[7,4]],7],1]", "[[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]]")) { ++failures; }
    if (!additionTest ("[[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]]", "[[[[4,2],2],6],[8,7]]", "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]")) { ++failures; }

    if (!magnitudeTest ("[9,1]", 29UL)) { ++failures; }
    if (!magnitudeTest ("[1,9]", 21UL)) { ++failures; }

    if (failures == 0U){ 
        std::cout << "All tests passed!\n";
    }
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    std::vector<SnailfishPair*> numbers = getInput ();
    SnailfishPair* sum = sumNumbers (numbers);
    numbers.clear ();
    std::cout << *sum << "\n";
    std::cout << sum->magnitude () << "\n";
    delete sum;
    runTests ();
    return 0;
}