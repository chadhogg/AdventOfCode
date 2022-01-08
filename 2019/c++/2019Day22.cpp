/// \file 2019Day22.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/22.

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <numeric>
#include <deque>
#include <stack>
#include <unordered_map>

#include "utilities.hpp"

enum ActionType {
    STACK,
    CUT,
    DEAL
};

struct Action {
    ActionType type;
    int param; 
};

using Card = int;
using Deck = std::deque<Card>;

std::ostream & operator<< (std::ostream & out, Deck const& deck) {
    for (Card card : deck) { out << card << " "; }
    return out;
}

Deck getFactoryOrder (int count) {
    Deck numbers (count);
    std::iota (numbers.begin (), numbers.end (), 0);
    return numbers;
}

std::ostream & operator<< (std::ostream & out, Action const& action) {
    switch (action.type) {
        case STACK: out << "deal into new stack"; break;
        case CUT: out << "cut " << action.param; break;
        case DEAL: out << "deal with increment " << action.param; break;
        default: throw std::runtime_error ("Unknown action type: " + std::to_string (action.type));
    }
    return out;
}

std::vector<Action> getInput () {
    std::vector<Action> actions;
    std::string line;
    int quantity;
    while (std::getline (std::cin, line)) {
        if (line == "deal into new stack") {
            actions.push_back ({STACK, 0});
        }
        else if (sscanf (line.c_str (), "cut %d", &quantity) == 1) {
            actions.push_back ({CUT, quantity});
        }
        else if(sscanf (line.c_str (), "deal with increment %d", &quantity) == 1) {
            actions.push_back ({DEAL, quantity});
        }
        else {
            throw std::runtime_error ("Unknown line \"" + line + "\"");
        }
    }
    return actions;
}

void dealIntoNewStack (Deck & cards) {
    std::stack<Card> stack;
    while (!cards.empty ()) { stack.push (cards.front ()); cards.pop_front (); }
    while (!stack.empty ()) { cards.push_back (stack.top ()); stack.pop (); }
}

void cutNCards (Deck & cards, int num) {
    if (num >= 0) {
        std::deque<Card> cutCards;
        while ((int)cutCards.size () < num) { cutCards.push_back (cards.front ()); cards.pop_front (); }
        while (!cutCards.empty ()) { cards.push_back (cutCards.front ()); cutCards.pop_front (); }
    }
    else {
        num = std::abs (num);
        std::deque<Card> cutCards;
        while ((int)cutCards.size () < num) { cutCards.push_front (cards.back ()); cards. pop_back (); }
        while (!cutCards.empty ()) { cards.push_front (cutCards.back ()); cutCards.pop_back (); }
    }
}

void dealWithIncrementN (Deck & cards, int num) {
    std::vector<Card> temp (cards.size ());
    for (std::size_t index {0U}; index < temp.size (); ++index) {
        temp[(index * num) % temp.size ()] = cards.front ();
        cards.pop_front ();
    }
    for (std::size_t index {0U}; index < temp.size (); ++index) {
        cards.push_back (temp[index]);
    }
}

void shuffle (std::vector<Action> const& commands, Deck & cards) {
    for (Action const& action : commands) {
        switch (action.type) {
            case STACK: dealIntoNewStack (cards); break;
            case CUT: cutNCards (cards, action.param); break;
            case DEAL: dealWithIncrementN (cards, action.param); break;
            default: throw std::runtime_error ("Unknown action type.");
        }
    }
}

std::size_t findCard (Deck const& deck, Card card) {
    for (std::size_t index {0U}; index < deck.size (); ++index) {
        if (deck.at (index) == card) { return index; }
    }
    throw std::runtime_error ("Didn't find card.");
}

constexpr unsigned long long NUM_CARDS = 119315717514047ULL;
constexpr unsigned long long TIMES_TO_DO = 101741582076661ULL;

inline unsigned long long getPositionBeforeDealIntoNewStack (unsigned long long after) {
    return NUM_CARDS - after - 1;
}

inline unsigned long long getPositionBeforeCutNCards (unsigned long long after, int n) {
    if (n > 0) {
        if (after >= NUM_CARDS - n) { return after - (NUM_CARDS - n); }
        else { return after + n; }
    }
    else {
        n = std::abs (n);
        if (after < (unsigned int)n) { return after + (NUM_CARDS - n); }
        else { return after - n; }
    }
}

/*
EXAMPLE
size of 10, increment of 3
old new
0   0
1   3
2   6
3   9
4   2
5   5
6   8
7   1
8   4
9   7
if (new % inc == 0) then old = new / inc
if (new + size % inc == 0), then old = new + size / inc
if (new + 2size % inc == 0), then old = new + 2size / inc
*/

inline unsigned long long getPositionBeforeDealWithIncrementN (unsigned long long after, int n) {
    for (unsigned int factor {1U}; true; ++factor) {
        if ((after + factor * NUM_CARDS) % n == 0) { return (after + factor * NUM_CARDS) / n; }
    }
}

unsigned long long findPositionBeforeAllSteps (unsigned long long finalPos, std::vector<Action> const& commands, unsigned long long repetitions) {
    std::unordered_map<unsigned long long, unsigned long long> cache;
    bool found = false;
    unsigned long long pos = finalPos;
    for (unsigned long long r {0U}; r < repetitions; ++r) {
        unsigned long long afterThisCycle = pos;
        if (r % 10000000ULL == 0ULL) {
            std::cout << "Finished " << r / (double)repetitions * 100 << "% of repetitions.\n";
        }
        if (cache.count (afterThisCycle) == 1) {
            pos = cache[afterThisCycle];
            if (!found) { std::cout << "Hit a cycle!\n"; }
            found = true;
            break;
        }
        for (long index = commands.size () - 1; index >= 0; --index) {
            switch (commands[index].type) {
                case STACK: pos = getPositionBeforeDealIntoNewStack (pos); break;
                case CUT: pos = getPositionBeforeCutNCards (pos, commands[index].param); break;
                case DEAL: pos = getPositionBeforeDealWithIncrementN (pos, commands[index].param); break;
                default: throw std::runtime_error ("Unknown command.");
            }
        }
        cache[afterThisCycle] = pos;
    }
    return pos;
}

int main () {
    std::vector<Action> commands = getInput ();
    Deck deck = getFactoryOrder (10007);
    shuffle (commands, deck);
    std::cout << findCard (deck, 2019) << "\n";
    std::cout << findPositionBeforeAllSteps (2020, commands, TIMES_TO_DO) << "\n";
    return 0;
}
