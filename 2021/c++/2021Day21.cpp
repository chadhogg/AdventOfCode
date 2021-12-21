/// \file 2021Day21.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/21.



#include <iostream>
#include <cstdio>
#include <cassert>
#include <unordered_map>

#include "utilities.hpp"

using Positions = std::pair<unsigned int, unsigned int>;
using Scores = std::pair<unsigned int, unsigned int>;
using WinCounts = std::pair<unsigned long long, unsigned long long>;

struct GameState {
    Positions positions;
    Scores scores;
    bool p1Turn;

    bool operator== (GameState const& other) const {
        return positions == other.positions && scores == other.scores && p1Turn == other.p1Turn;
    }
};

template<>
struct std::hash<GameState> {
    std::size_t operator() (GameState const& state) const {
        return std::hash<Positions>() (state.positions) + 7 * std::hash<Scores>() (state.scores) + 11 * std::hash<bool>() (state.p1Turn);
    }
};

WinCounts operator+ (WinCounts const& a, WinCounts const& b) {
    return WinCounts {a.first + b.first, a.second + b.second};
}

WinCounts operator* (WinCounts const& a, unsigned int x) {
    return WinCounts {a.first * x, a.second * x};
}

constexpr unsigned int BOARD_MIN = 1U;
constexpr unsigned int BOARD_MAX = 10U;
constexpr unsigned int DETERMINISTIC_DIE_MIN = 1U;
constexpr unsigned int DETERMINISTIC_DIE_MAX = 100U;
constexpr unsigned int DIE_ROLLS = 3U;
constexpr unsigned int PART1_TARGET = 1000U;
constexpr unsigned int DIRAC_DIE_MIN = 1U;
constexpr unsigned int DIRAC_DIE_MAX = 3U;
constexpr unsigned int PART2_TARGET = 21U;

Positions getInput () {
    Positions pos;
    int result;
    result = scanf ("Player 1 starting position: %u\n", &pos.first);
    assert (result == 1);
    result = scanf ("Player 2 starting position: %u\n", &pos.second);
    assert (result == 1);
    return pos;
}

GameState nextState (GameState const& current, unsigned int rollTotal) {
    GameState next {current};
    if (current.p1Turn) {
        next.positions.first = (next.positions.first + rollTotal - BOARD_MIN) % BOARD_MAX + BOARD_MIN;
        next.scores.first += next.positions.first;
    }
    else {
        next.positions.second = (next.positions.second + rollTotal - BOARD_MIN) % BOARD_MAX + BOARD_MIN;
        next.scores.second += next.positions.second;
    }
    next.p1Turn = !next.p1Turn;
    return next;
}

int playUntilWin (Positions const& initial, unsigned int winThreshold) {
    unsigned int rolls {0U};
    GameState current {initial, {0U, 0U}, true};
    while (current.scores.first < winThreshold && current.scores.second < winThreshold) {
        unsigned int moves {0U};
        for (unsigned int count {0U}; count < DIE_ROLLS; ++count) {
            ++rolls;
            moves += ((rolls - DETERMINISTIC_DIE_MIN) % DETERMINISTIC_DIE_MAX + DETERMINISTIC_DIE_MIN);
        }
       current = nextState (current, moves);
    }
    if (current.scores.first >= winThreshold) {
        return current.scores.second * rolls;
    }
    else if (current.scores.second >= winThreshold) {
        return current.scores.first * rolls;
    }
    else {
        assert (false && "impossible");
    }
}


WinCounts continueGame (GameState const& current) {
    static std::unordered_map<GameState, WinCounts> memo;
    auto const iter = memo.find (current);
    if (iter != memo.end ()) {
        return (*iter).second;
    }
    else if (current.scores.first >= PART2_TARGET) {
        return WinCounts {1ULL, 0ULL};
    }
    else if (current.scores.second >= PART2_TARGET) {
        return WinCounts {0ULL, 1ULL};
    }
    else {
        WinCounts counts {0ULL, 0ULL};
        // (1, 1, 1)
        counts = counts + continueGame (nextState (current, 3U)) * 1U;
        // (2, 1, 1) or (1, 2, 1) or (1, 1, 2)
        counts = counts + continueGame (nextState (current, 4U)) * 3U;
        // (3, 1, 1) or (1, 3, 1) or (1, 1, 3), or (2, 2, 1) or (2, 1, 2) or (1, 2, 2)
        counts = counts + continueGame (nextState (current, 5U)) * 6U;
        // (1, 2, 3) or (1, 3, 2) or (2, 1, 3) or (2, 3, 1) or (3, 1, 2) or (3, 2, 1) or (2, 2, 2)
        counts = counts + continueGame (nextState (current, 6U)) * 7U;
        // (3, 3, 1) or (3, 1, 3) or (1, 3, 3) or (3, 2, 2) or (2, 3, 2) or (2, 2, 3)
        counts = counts + continueGame (nextState (current, 7U)) * 6U;
        // (3, 3, 2) or (3, 2, 3) or (2, 3, 3)
        counts = counts + continueGame (nextState (current, 8U)) * 3U;
        // (3, 3, 3)
        counts = counts + continueGame (nextState (current, 9U)) * 1U;
        memo.insert ({current, counts});
        return counts;
    }
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    Positions initial = getInput ();
    std::cout << playUntilWin (initial, PART1_TARGET) << "\n";
    GameState current {initial, {0U, 0U}, true};
    WinCounts counts = continueGame (current);
    if (counts.first > counts.second) {
        std::cout << counts.first << "\n";
    }
    else {
        std::cout << counts.second << "\n";
    }
    return 0;
}