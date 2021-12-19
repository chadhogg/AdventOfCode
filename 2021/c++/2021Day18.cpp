/// \file 2021Day18.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/18.

#include <iostream>
#include <string>
#include <cassert>
#include <deque>

#include "utilities.hpp"

/// If a pair is nested inside this many other pairs, it explodes.
constexpr unsigned int EXPLODE_DEPTH = 4U;
/// If a literal gets this large, it splits.
constexpr unsigned int SPLIT_LEVEL = 10U;

// Forward declarations.
class Snailfish;
class SnailfishLiteral;
class SnailfishPair;

/// Some type of snailfish number.
class Snailfish {
public:
    /// \brief Constructs a snailfish number.
    /// \param[in] parent A pointer to the parent of this number, or NULL.
    Snailfish (SnailfishPair* parent) : m_parent (parent) {}

    /// \brief Destructs a snailfish number.
    virtual ~Snailfish () {}

    Snailfish (Snailfish const& other) = delete;
    Snailfish& operator= (Snailfish const& other) = delete;
    Snailfish (Snailfish const&& other) = delete;
    Snailfish& operator= (Snailfish const&& other) = delete;

    /// \brief Generates a string containing a representation of this snailfish number.
    /// \return That string.
    virtual std::string toString () const = 0;

    /// \brief Counts how many pairs this node is nested inside of.
    /// \return The number of ancestors it has in the tree.
    unsigned int depth () const;

    /// \brief Finds the next literal value to the left of this part of the tree.
    /// \return That literal value.
    virtual SnailfishLiteral* findNextNumberToLeft () = 0;

    /// \brief Finds the next literal value to the right of this part of the tree.
    /// \return That literal value.
    virtual SnailfishLiteral* findNextNumberToRight () = 0;

    /// \brief Descends the tree leftward until reaching a leaf.
    /// \return That leaf.
    virtual SnailfishLiteral* descendLeft () = 0;

    /// \brief Descends the tree rightward until reaching a leaf.
    /// \return That leaf.
    virtual SnailfishLiteral* descendRight () = 0;

    /// Computes the magnitude of this number.
    /// Recursivesly, 3 * left child + 2 * right child.
    virtual unsigned long magnitude () const = 0;
public:

    /// A (weak) pointer to the parent of this tree node.  Needed for exploding.
    SnailfishPair* m_parent;
};

/// \brief Inserts a snailfish number into an output stream.
/// \param[inout] out The stream.
/// \param[in] snail The snailfish number.
/// \return The stream.
/// \post A representation of the snailfish number was inserted into the stream.
std::ostream& operator<< (std::ostream& out, Snailfish const& snail) {
    out << snail.toString ();
    return out;
}



/// A leaf node in s snailfish number tree.
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
    /// The literal value stored in this leaf.
    unsigned int m_value;
};



/// An internal node in a snailfish number tree.
class SnailfishPair : public Snailfish {
public:
    SnailfishPair (SnailfishPair* parent, Snailfish* first, Snailfish* second) : Snailfish (parent), m_left (first), m_right (second) {}
    virtual ~SnailfishPair () { delete m_left; delete m_right; }
    SnailfishPair (SnailfishPair const& other) = delete;
    SnailfishPair& operator= (SnailfishPair const& other) = delete;
    SnailfishPair (SnailfishPair const&& other) = delete;
    SnailfishPair& operator= (SnailfishPair const&& other) = delete;

    virtual std::string toString () const { return "[" + m_left->toString () + "," + m_right->toString () + "]"; }
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
    virtual unsigned long magnitude () const { return 3 * m_left->magnitude () + 2 * m_right->magnitude (); }
    SnailfishPair* clone (SnailfishPair* newParent) const {
        SnailfishPair* copy = new SnailfishPair (newParent, NULL, NULL);
        if (dynamic_cast<SnailfishPair*> (m_left)) {
            copy->m_left = dynamic_cast<SnailfishPair*> (m_left)->clone (copy);
        }
        else {
            copy->m_left = new SnailfishLiteral (copy, dynamic_cast<SnailfishLiteral*> (m_left)->m_value);
        }
        if (dynamic_cast<SnailfishPair*> (m_right)) {
            copy->m_right = dynamic_cast<SnailfishPair*> (m_right)->clone (copy);
        }
        else {
            copy->m_right = new SnailfishLiteral (copy, dynamic_cast<SnailfishLiteral*> (m_right)->m_value);
        }
        return copy;
    }

    /// \brief Reduces this tree as much as possible.
    /// \post This tree has been reduced until there are no more eligible explodes or splits.
    /// \note This should only be called on the root node.
    void fullReduce () {
        while (singleReduce ()) {
            /* intentionally empty */
        }
    }

    /// \brief Performs the first available explode, or if none the first available split.
    /// \post If an explode was available, the first one was done.  Otherwise, if a split was available, the first one was done.
    /// \return Whether or not some reduction operation was applied.
    bool singleReduce () {
        bool change = explodeFirstEligible ();
        if (!change) {
            change = splitFirstEligible ();
        }
        return change;
    }

    /// \brief Finds and performs the first explodable node in an in-order traversal.
    /// \post If any node in this subtree was explodable, the first such one has occurred.
    /// \return Whether or not an explodable node was found.
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

    /// \brief Finds and performs the first splittable node in an in-order traversal.
    /// \post If any node in this subtree was splittable, the first such one has occurred.
    /// \return Whether or not a splittable node was found.
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

    /// \brief Explodes this node.
    /// \post The left child has been added to the next literal to the left, and the right child to the next literal to the right.
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

public:
    /// The left compoennt of this pair.
    Snailfish* m_left;
    /// The right component of this pair.
    Snailfish* m_right;
};

// Documented above.
unsigned int Snailfish::depth () const {
        if (m_parent == NULL) { return 0; }
        else { return 1U + m_parent->depth (); }
}



/// \brief Recursively parses (the next part of a) character stream into a snailfish number.
/// \param[inout] line A source of characters.
/// \param[in] parent The part node of the node that will be created.
/// \post The relevant characters have been removed from the line.
/// \note The number will have been dynamically allocated and will need to be deleted.
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


/// \brief Reads the input to the problem.
/// \return A vector of pointers to snailfish numbers.
/// \note The elements of the vector were dynamically allocated and will need to be deleted at some point.
std::vector<SnailfishPair*> getInput () {
    std::vector<SnailfishPair*> numbers;
    std::string line;
    while (std::cin >> line) {
        std::deque<char> deque (line.begin (), line.end ());
        numbers.push_back (parse (deque, NULL));
    }
    return numbers;
}


/// \brief Adds two snailfish numbers and returns the fully reduced sum.
/// \param[in] first The first addend.
/// \param[in] second The second addend.
/// \param[in] clone True to work with copies of the addends, false to use them directly.
/// \return The reduced sum.
/// \note The sum will need to be deleted.  If there was no cloning, it has taken over responsibility for deleting the addends.
SnailfishPair* addAndReduce (SnailfishPair* first, SnailfishPair* second, bool clone) {
    if (clone) {
        SnailfishPair* firstClone = first->clone (NULL);
        SnailfishPair* secondClone = second->clone (NULL);
        SnailfishPair* result = new SnailfishPair (NULL, firstClone, secondClone);
        firstClone->m_parent = result;
        secondClone->m_parent = result;
        result->fullReduce ();
        return result;
    }
    else {
        SnailfishPair* result = new SnailfishPair (NULL, first, second);
        first->m_parent = result;
        second->m_parent = result;
        result->fullReduce ();
        return result;
    }
}

/// \brief Adds all the snailfish numbers in a vector.
/// \param[in] numbers A vector containing all of the addends.
/// \return The sum.
/// \note The sum will need to be deleted.
SnailfishPair* sumNumbers (std::vector<SnailfishPair*> & numbers) {
    SnailfishPair* sum = numbers.front ()->clone (NULL);
    for (unsigned int index {1U}; index < numbers.size (); ++index) {
        sum = addAndReduce (sum, numbers.at (index)->clone (NULL), false);
    }
    return sum;
}

/// \brief Adds all pairs of snailfish numbers in a vector, returning the magnitude of the largest sum.
/// \param[in] numbers A fector containing all of the potential addends.
/// \return The highest magnitude found in any pairwise sum.
unsigned long findLargestSum (std::vector<SnailfishPair*> const& numbers) {
    unsigned long largest = 0UL;
    for (unsigned int a {0U}; a < numbers.size (); ++a) {
        for (unsigned int b {0U}; b < numbers.size (); ++b) {
            if (a != b) {
                SnailfishPair* sum = addAndReduce (numbers.at (a), numbers.at (b), true);
                unsigned long magnitude = sum->magnitude ();
                if (magnitude > largest) { largest = magnitude; }
                delete sum;
            }
        }
    }
    return largest;
}

bool singleReduceTest (std::string input, std::string expected) {
    std::deque<char> chars (input.begin (), input.end ());
    SnailfishPair* number = parse (chars, NULL);
    number->singleReduce ();
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
    SnailfishPair* sum = addAndReduce (firstNumber, secondNumber, false);
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
    //runTests ();
    
    std::vector<SnailfishPair*> numbers = getInput ();
    SnailfishPair* sum = sumNumbers (numbers);
    std::cout << sum->magnitude () << "\n";
    delete sum;

    std::cout << findLargestSum (numbers) << "\n";

    for (SnailfishPair* number : numbers) { delete number; }    

    return 0;
}