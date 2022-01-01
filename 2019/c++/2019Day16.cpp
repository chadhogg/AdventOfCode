/// \file 2019Day16.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/16.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
#include <unordered_map>

#include "utilities.hpp"

/* notes
position:       abcdefgh
original input: 12345678
formulas:
a' = a - c + e - g
b' = b + c - f - g
c' = c + d + e
d' = d + e + f + g
e' = e + f + g + h
f' = f + g + h
g' = g + h
h' = h

a'' = a' - c' + e' - g'
b'' = b' + c' - f' - g'
c'' = c' + d' + e'
d'' = d' + e' + f' + g'
e'' = e' + f' + g' + h'
f'' = f' + g' + h'
g'' = g' + h'
h'' = h'

a'' = (a - c + e - g) - (c + d + e) + (e + f + g + h) - (g + h)
b'' = (b + c - f - g) + (c + d + e) - (f + g + h) - (g + h)
c'' = (c + d + e) + (d + e + f + g) + (e + f + g + h)
d'' = (d + e + f + g) + (e + f + g + h) + (f + g + h) + (g + h)
e'' = (e + f + g + h) + (f + g + h) + (g + h) + (h)
f'' = (f + g + h) + (g + h) + (h)
g'' = (g + h) + (h)
h'' = h

a'' = a - 2c - d + e + f - g
b'' = b + 2c + d + e - 2f -3g -2h
c'' = c + 2d + 3e + 2f + 2g + h
d'' = d + 2e + 3f + 4g + 3h
e'' = e + 2f + 3g + 4h
f'' = f + 2g + 3h
g'' = g + 2h
h'' = h

e''' = e'' + f'' + g'' + h'' = e + 3f + 6g + 10h
f''' = f'' + g'' + h'' = f + 3g + 6h
g''' = g'' + h'' = g + 3h
h''' = h'' = h

e'''' = e''' + f''' + g''' + h''' = e + 4f + 10g + 20h
f'''' = f''' + g''' + h''' = f + 4g + 10h
g'''' = g''' + h''' = g + 4h
h'''' = h''' = h

but this is ignoring the modular arithmetic ... maybe that doesn't matter? ... it seems I can just do it at the end
The important thing is, the calculation for term X never uses any term prior to X.

w(5) = e + 5f + 15g + 35h
x(5) = f + 5g + 15h
y(5) = g + 5h
z(5) = h 

w(6) = e + 6f + 21g + 56h
x(6) = f + 6g + 21h
y(6) = g + h6
z(6) = h

w(n) = e + n*f + n(n+1)/2*g + nth tetrahedral number * h
x(n) = f + n*g + n(n+1)/2*h
y(n) = g + n*h
z(n) = h

nth triangular number is (n + 1) choose 2
nth tetrahedral number is (n + 2) choose 3
nth kth number is (n + k - 1) choose k
k starts at 0

formula for any element in second half: sum of ((n + k - 1) choose k) * original term
                                        k=0 for itself, 1 for term after it, 2 for term after that ...
                                        n is the number of phases you are running
*/

using Sequence = std::vector<int>;

const Sequence BASE_PATTERN = {0, 1, 0, -1};

Sequence getPatternForIndex (unsigned int index) {
    static std::vector<Sequence> patterns;
    if (index < patterns.size ()) {
        return patterns.at (index);
    }
    assert (patterns.size () == index);
    Sequence pattern;
    for (int num : BASE_PATTERN) {
        for (unsigned int count {0U}; count <= index; ++count) {
            pattern.push_back (num);
        }
    }
    patterns.push_back (pattern);
    return pattern;
}

Sequence doPhase (Sequence input) {
    Sequence output;
    output.reserve (input.size ());
    for (unsigned int index {0U}; index < input.size (); ++index) {
        Sequence pattern = getPatternForIndex (index);
        unsigned int patternIndex = 1U;
        int sum {0};
        for (int num : input) {
            sum += num * pattern[patternIndex];
            patternIndex = (patternIndex + 1) % pattern.size ();
        }
        output.push_back (std::abs (sum) % 10);
    }
    return output;
}

Sequence doMultiplePhases (Sequence const& input, unsigned int toDo) {
    Sequence output = input;
    for (unsigned int count {0U}; count < toDo; ++count) {
        output = doPhase (output);
    }
    return output;
}

Sequence getInput () {
    std::string line = read<std::string> ();
    Sequence result;
    for (char c : line) {
        result.push_back (c - '0');
    }
    return result;
}

unsigned long sublist (Sequence const& sequence, unsigned int offset, unsigned int length) {
    unsigned long result = 0;
    for (unsigned int count {0U}; count < length; ++count) {
        result *= 10;
        result += sequence.at (offset + count);
    }
    return result;
}

std::string strlist (Sequence const& sequence, unsigned int offset, unsigned int length) {
    std::string str = "";
    for (unsigned int count {0U}; count < length; ++count) {
        str += std::to_string (sequence.at (offset + count));
    }
    return str;
}

Sequence expand (Sequence const& original, unsigned int duplications) {
    Sequence result;
    result.reserve (duplications * original.size ());
    for (unsigned int count {0U}; count < duplications; ++count) {
        for (int i : original) {
            result.push_back (i);
        }
    }
    return result;
}

long long choose (long long n, long long k) {
    if (k == 0) { return 1; }
    return (n * choose (n - 1, k - 1)) / k;
}

using Type = int;
using Index = int;

constexpr long MIN_TYPE = -1;
constexpr long MAX_TYPE = 3250000;
constexpr long MIN_NUMBER = 1;
constexpr long MAX_NUMBER = 100;
constexpr char INVALID = 10;
constexpr long ENTRY_LENGTH = MAX_NUMBER - MIN_NUMBER + 1;

struct TriangularNumberLikeThingMemo {
    std::vector<char> answers;

    TriangularNumberLikeThingMemo () {
        answers.resize ((MAX_TYPE - MIN_TYPE + 1) * (MAX_NUMBER - MIN_NUMBER + 1), INVALID);
    }
    
    bool contains (Type type, Index number) const {
        return answers[type * ENTRY_LENGTH + number] != INVALID;
    }

    int get (Type type, Index number) const {
        return answers[type * ENTRY_LENGTH + number];
    }

    void set (Type type, Index number, char value) {
        answers[type * ENTRY_LENGTH + number] = value;
    }
};

int getTriangularNumberLikeThing2 (Type type, Index number) {
    static TriangularNumberLikeThingMemo memo;
    assert (number > 0);
    if (!memo.contains (type, number)) {
        if (type == -1) { memo.set (type, number, 1); }
        else if (type == 0) { memo.set (type, number, number % 10); }
        else if( type == 1) { memo.set (type, number, (number * (number + 1) / 2) % 10); }
        else if (number == 1) { memo.set (type, number, 1); }
        else { 
            memo.set (type, number, (getTriangularNumberLikeThing2 (type, number - 1) + getTriangularNumberLikeThing2 (type - 1, number)) % 10);
        }
    }
    return memo.get (type, number);
}

int getTriangularNumberLikeThingOld (Type type, Index number) {
    static std::unordered_map<Type, std::unordered_map<Index, char>> memo;
    assert (number > 0);
    if (memo.count (type) == 0) { memo.insert ({type, {}}); }
    if (memo[type].count (number) == 0) {
        if (type == -1) { memo[type][number] = 1; }
        else if (type == 0) { memo[type][number] = number % 10; }
        else if( type == 1) { memo[type][number] = (number * (number + 1) / 2) % 10; }
        else if (number == 1) { memo[type][number] = 1; }
        else { 
            memo[type][number] = (getTriangularNumberLikeThing2 (type, number - 1) + getTriangularNumberLikeThing2 (type - 1, number)) % 10; 
        }
    }
    return memo[type][number];
}


Sequence onlySolveSecondHalf (Sequence const& original, unsigned int phases) {
    Sequence result;
    while (result.size () * 2 < original.size ()) {
        // we don't care about these values, so don't bother calculating what they are
        result.push_back (0);
    }
    std::cout << "Minimum offset = -1, maximum offset = " << (original.size () - result.size ()) << "\n";

    for (unsigned int index = result.size (); result.size () < original.size (); ++index) {
        int sum {0};
        for (int offset = 0; (int)result.size () + offset < (int)original.size (); ++offset) {
            sum = (sum + getTriangularNumberLikeThing2 (offset - 1, phases) * original[index + offset]) % 10;
        }
        result.push_back (sum);
    }
    return result;
}

void preCalculateTriangularNumberThings (int max_type, int max_number) {
    for (int i = 0; i <= max_type; ++i) {
        for (int j = 1; j <= max_number; ++j) {
            getTriangularNumberLikeThing2 (i, j);
        }
    }
}

std::string justGiveUsPartWeCareAbout (Sequence const& original, unsigned int phases, unsigned int startIndex, unsigned int length) {
    std::string result = "";
    preCalculateTriangularNumberThings (original.size () / 2, phases);
    for (unsigned int index = startIndex; index < startIndex + length; ++index) {
        int sum {0};
        for (unsigned int offset = 0; index + offset < original.size (); ++offset) {
            sum = (sum + getTriangularNumberLikeThing2 (offset - 1, phases) * original[index + offset]) % 10;
        }
        result += ('0' + sum);
    }
    return result;
}


std::string justGiveUsPartWeCareAboutOld (Sequence const& original, unsigned int phases, unsigned int startIndex, unsigned int length) {
    std::string result = "";
    preCalculateTriangularNumberThings (original.size () / 2, phases);
    for (unsigned int index = startIndex; index < startIndex + length; ++index) {
        int sum {0};
        for (unsigned int offset = index - original.size () / 2; offset < original.size () / 2; ++offset) {
            sum = (sum + getTriangularNumberLikeThing2 (offset - 1, phases) * original[original.size () / 2 + offset]) % 10;
        }
        result += ('0' + sum);
    }
    return result;
}

constexpr unsigned int PHASES = 100U;

int main () {
    Sequence input = getInput ();
    Sequence after100 = doMultiplePhases (input, PHASES);
    std::cout << sublist (after100, 0U, 8U) << "\n";
    Sequence expanded = expand (input, 10000U);
    //Sequence expandedAfter100 = onlySolveSecondHalf (expanded, PHASES);
    //std::cout << sublist (expandedAfter100, sublist (input, 0U, 7U), 8U) << "\n";
    //Sequence expandedAfter100 = doMultiplePhases (expanded, 100U);
    //std::cout << sublist (expandedAfter100, sublist (input, 0U, 7U), 8U) << "\n";
    //Sequence otherAfter100 = onlySolveSecondHalf (input, PHASES);
    //std::cout << strlist (after100, after100.size () / 2, after100.size () / 2) << "\n";
    //std::cout << strlist (otherAfter100, otherAfter100.size () / 2, otherAfter100.size () / 2) << "\n";
    std::cout << justGiveUsPartWeCareAbout (expanded, PHASES, sublist (input, 0U, 7U), 8U) << "\n";
    //std::cout << justGiveUsPartWeCareAbout (input, PHASES, 4, 4) << "\n";
/*
    for (int i = 1; i <= 10; ++i) {
        std::cout << getTriangularNumberLikeThing2 (-1, i) << "\t";
    }
    std::cout << "\n";
    for (int i = 1; i <= 10; ++i) {
        std::cout << getTriangularNumberLikeThing2 (0, i) << "\t";
    }
    std::cout << "\n";
    for (int i = 1; i <= 10; ++i) {
        std::cout << getTriangularNumberLikeThing2 (1, i) << "\t";
    }
    std::cout << "\n";
    for (int i = 1; i <= 10; ++i) {
        std::cout << getTriangularNumberLikeThing2 (2, i) << "\t";
    }
    std::cout << "\n";
    for (int i = 1; i <= 10; ++i) {
        std::cout << getTriangularNumberLikeThing2 (3, i) << "\t";
    }
    std::cout << "\n";
    */
    return 0;
}
