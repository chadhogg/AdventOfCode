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

constexpr int PART1_LAST_CARD = 10007;
constexpr unsigned long long PART2_NUM_CARDS = 119315717514047ULL;
constexpr unsigned long long TIMES_TO_DO = 101741582076661ULL;

enum ActionType {
    STACK,
    CUT,
    DEAL
};

struct Action {
    ActionType type;
    long long param; 
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


inline unsigned long long getPositionBeforeDealIntoNewStack (unsigned long long after, unsigned long long numCards) {
    return numCards - after - 1;
}

inline unsigned long long getPositionBeforeCutNCards (unsigned long long after, long long n, unsigned long long numCards) {
    if (n > 0) {
        if (after >= numCards - n) { return after - (numCards - n); }
        else { return after + n; }
    }
    else {
        n = std::abs (n);
        if (after < (unsigned int)n) { return after + (numCards - n); }
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

inline unsigned long long getPositionBeforeDealWithIncrementN (unsigned long long after, unsigned long long n, unsigned long long numCards) {
    // Belief: after = (before * n) % numCards
    // Belief: before = (after + factor * numCards) / n for some factor such that (after + factor * numCards) % n == 0
    // Belief: factor = after % n
    // For this problem: after =     63,517,699,051,668
    // For this problem: n =         23,273,712,670,748
    // For this problem: numCards = 119,315,717,514,047
    // For this problem: factor   =  16,970,273,710,172  (according to the factor = after % n belief)
    // I believe that this should mean that (after + remainder * numCards) % n == 0, but it does not.
    // Instead it                 =  15,294,254,074,448
    __int64_t remainder = (__int64_t)after % (__int64_t)n;
    __int64_t quotient = (__int64_t)after / (__int64_t)n;
    __int64_t before = (after + remainder * numCards) / n;
    assert (n * quotient + remainder == after);
    assert ((before * n) % numCards == after);
    //assert ((after + remainder % n * numCards % n) % n == 0);
    return (unsigned long long)((after + remainder * numCards) / n);
    // NOTE: Back to my original equation, but I don't have to guess the factor -- I can calculate it!
    /*
    unsigned long long factor = after % n;
    assert ((after + factor % n * numCards % n) % n == 0);
    return (after + factor * numCards) / n;
    */
    // NOTE: This also seemed to take infinite time
    //for (unsigned long long before {0U}; before <= numCards; ++before) {
    //    unsigned long long result = (before * n) % numCards;
    //    if (result == after) { return before; }
    //}
    //throw std::runtime_error ("Didn't find any value that mapped there.");
    // NOTE: This didn't work due to overflow
    //for (unsigned long long factor {1U}; true; ++factor) {
    //    if ((after + factor * numCards) % n == 0) { return (after + factor * numCards) / n; }
    //    if (factor % 10000 == 0) { std::cout << "Tried factor " << factor << "\n"; }
    //}
}

unsigned long long findPositionBeforeAllSteps (unsigned long long finalPos, std::vector<Action> const& commands, unsigned long long repetitions, unsigned long long numCards) {
    // Note: If a cycle does occur, it isn't within the first 250 million iterations.
    // Note: It will take approximately 5000 days for this to run the full number of iterations.
    //std::unordered_map<unsigned long long, unsigned long long> cache;
    //bool found = false;
    unsigned long long pos = finalPos;
    for (unsigned long long r {0U}; r < repetitions; ++r) {
        //unsigned long long afterThisCycle = pos;
        //if (r % 10000000ULL == 0ULL) {
        //    std::cout << "Finished " << r / (double)repetitions * 100 << "% of repetitions.\n";
        //}
        //if (cache.count (afterThisCycle) == 1) {
        //    pos = cache[afterThisCycle];
        //    if (!found) { std::cout << "Hit a cycle!\n"; }
        //    found = true;
        //    break;
        //}
        for (long index = commands.size () - 1; index >= 0; --index) {
            switch (commands[index].type) {
                case STACK: pos = getPositionBeforeDealIntoNewStack (pos, numCards); break;
                case CUT: pos = getPositionBeforeCutNCards (pos, commands[index].param, numCards); break;
                case DEAL: pos = getPositionBeforeDealWithIncrementN (pos, commands[index].param, numCards); break;
                default: throw std::runtime_error ("Unknown command.");
            }
        }
        //cache[afterThisCycle] = pos;
    }
    return pos;
}

// New idea, inspired by https://www.reddit.com/r/adventofcode/comments/ee56wh/2019_day_22_part_2_so_whats_the_purpose_of_this/fbr0vjb/

std::vector<Action> simplify (std::vector<Action> const& original, unsigned long long numCards) {
    std::vector<Action> actions = original;
    //std::cout << actions << "\n";
    while (true) {
        bool madeAChange = false;
        // Try removing consectutive STACKs
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == STACK && actions[index + 1].type == STACK) {
                actions.erase (actions.begin () + index + 1);
                actions.erase (actions.begin () + index);
                madeAChange = true;
                //std::cout << "Removed consecutive STACKs at positions " << index << " and " << index + 1 << "\n";
            }
        }
        // Try combining consecutive CUTs
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == CUT && actions[index + 1].type == CUT) {
                actions[index].param = (actions[index].param + actions[index + 1].param + numCards) % numCards;
                actions.erase (actions.begin () + index + 1);
                madeAChange = true;
                //std::cout << "Combining consecutive CUTs at positions " << index << " and " << index + 1 << "\n";
            }
        }
        // Try combining consecutive DEALs
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == DEAL && actions[index + 1].type == DEAL) {
                actions[index].param = (actions[index].param * actions[index + 1].param) % numCards;
                actions.erase (actions.begin () + index + 1);
                madeAChange = true;
                //std::cout << "Combining consecutive DEALs at positions " << index << " and " << index + 1 << "\n";
            }
        }
        // Try moving a DEAL up through a STACK
        // 0 1 2 3 4 5 6 7 8 9 A
        // deal into new stack
        // A 9 8 7 6 5 4 3 2 1 0
        // deal with increment 4
        // A 7 4 1 9 6 3 0 8 5 2
        // alternative
        // 0 1 2 3 4 5 6 7 8 9 A
        // deal with increment 4
        // 0 3 6 9 1 4 7 A 2 5 8
        // cut 11 + 1 - 4
        // 2 5 8 0 3 6 9 1 4 7 A
        // deal into new stack
        // A 7 4 1 9 6 3 0 8 5 2
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == STACK && actions[index + 1].type == DEAL) {
                Action oldDeal = actions[index + 1];
                actions[index] = oldDeal;
                actions[index + 1] = {CUT, (long long)numCards + 1 - oldDeal.param};
                actions.insert (actions.begin () + index + 2, {STACK, 0});
                madeAChange = true;
                //std::cout << "Swapped a DEAL at " << index + 1 << " with a STACK at " << index << ", adding a CUT\n";
            }
        }
        // Try moving a DEAL up through a CUT
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == CUT && actions[index + 1].type == DEAL) {
                Action oldCut = actions[index];
                Action oldDeal = actions[index + 1];
                actions[index] = oldDeal;
                actions[index + 1] = {CUT, (long long)(((oldCut.param + numCards) * oldDeal.param) % numCards)};
                madeAChange = true;
                //std::cout << "Swapped a DEAL at " << index + 1 << " with a CUT at " << index << "\n";
            }
        }
        // Try moving a CUT up through a STACK
        for (unsigned int index {0U}; !madeAChange && index < actions.size () - 1; ++index) {
            if (actions[index].type == STACK && actions[index + 1].type == CUT) {
                Action oldCut = actions[index + 1];
                actions[index] = {CUT, ((long long)numCards - oldCut.param) % (long long)numCards};
                actions[index + 1] = {STACK, 0};
                madeAChange = true;
                //std::cout << "Swapped a CUT at " << index + 1 << " with a STACK at " << index << "\n";
            }
        }
        if (!madeAChange || actions.empty ()) { break; }
        //std::cout << actions << "\n";
    }
    return actions;
}

void checkCommands (std::vector<Action> const& commands) {
    std::vector<Action> simplified = simplify (commands, PART1_LAST_CARD);
    Deck d1 = getFactoryOrder (PART1_LAST_CARD);
    Deck d2 = getFactoryOrder (PART1_LAST_CARD);
    shuffle (commands, d1);
    shuffle (simplified, d2);
    assert (d1 == d2);
}

void test () {
    // Test two STACKs cancelling each other
    checkCommands ({{STACK, 0}, {STACK, 0}});

    // Test two CUTs combining with each other
    checkCommands ({{CUT, 4}, {CUT, -7}});

    // Test two DEALs cancelling with each other
    checkCommands ({{DEAL, 4}, {DEAL, 12}});

    // Test swapping a DEAL up through a STACK
    checkCommands ({{STACK, 0}, {DEAL, 7}});

    // Test swapping a DEAL up through a CUT
    checkCommands ({{CUT, -8}, {DEAL, 13}});

    // Test swapping a CUT up through a STACK
    checkCommands ({{STACK, 0}, {CUT, -4}});
}

std::vector<Action> simplify (std::vector<Action> const& commands, unsigned long long numCards, unsigned long long repetitions) {
    if (repetitions == 0) { return {}; }
    else if (repetitions == 1) { return commands; }
    else {
        std::vector<Action> doubled;
        doubled.insert (doubled.end (), commands.begin (), commands.end ());
        doubled.insert (doubled.end (), commands.begin (), commands.end ());
        doubled = simplify (doubled, numCards);
        std::vector<Action> result = simplify (doubled, numCards, repetitions / 2);
        if (repetitions % 2 == 1) {
            result.insert (result.end (), commands.begin (), commands.end ());
        }
        return simplify (result, numCards);
    }
}

int main () {
    std::vector<Action> commands = getInput ();
    Deck deck = getFactoryOrder (PART1_LAST_CARD);
    shuffle (commands, deck);
    std::cout << findCard (deck, 2019) << "\n";

    std::vector<Action> simplified = simplify (commands, PART2_NUM_CARDS, TIMES_TO_DO);
    std::cout << simplified << "\n";
    std::cout << findPositionBeforeAllSteps (2020, simplified, 1, PART2_NUM_CARDS) << "\n";
    //std::cout << findPositionBeforeAllSteps (2020, commands, TIMES_TO_DO) << "\n";
    return 0;
}
