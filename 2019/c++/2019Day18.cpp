/// \file 2019Day18.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/18.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include <deque>
#include <array>
#include <cctype>

#include "utilities.hpp"

constexpr char PLAYER = '@';
constexpr char WALL = '#';
constexpr char PASSAGE = '.';
constexpr char OFF_MAP = '?';

constexpr std::size_t MAX_KEYS = 26U;

class KeySet {
public:
    KeySet () : set {}, count {0U} {}
    inline void addKey (char key) { set[key - 'a'] = true; ++count; }
    inline bool hasKey (char key) const { return set[key - 'a']; }
    inline std::size_t size () const { return count; }
    bool containsAll (KeySet const& other) const {
        for (std::size_t index {0U}; index < MAX_KEYS; ++index) {
            if (other.set[index] && !set[index]) { return false; }
        }
        return true;
    }
    inline bool operator== (KeySet const& other) const { 
        //std::cout << "Me: " << set << " (" << count << ")\n";
        //std::cout << "It: " << other.set << " (" << other.count << ")\n";
        bool result = (count == other.count && set == other.set);
        //std::cout << "Returning " << result << "\n";
        return result;
    }
    friend std::ostream & operator<< (std::ostream & out, KeySet const& keys) {
        for (std::size_t index {0U}; index < MAX_KEYS; ++index) {
            if (keys.set[index]) { out << (index + 'a'); }
            else { out << ' '; }
        }
        return out;
    }
private:
    std::array<bool, MAX_KEYS> set;
    std::size_t count;
};

inline char isKey (char c) { /*return std::islower (c);*/ return c >= 'a' && c <= 'z'; }
//inline char isDoor (char c) { return std::isupper (c); }
inline char isDoor (char c) { return c >= 'A' && c <= 'Z'; }
inline char doorToKey (char c) { /*return std::tolower (c);*/ return c + ('a' - 'A'); }
inline char keyToDoor (char c) { /*return std::toupper (c);*/ return c - ('a' - 'A'); }

struct Board {
    std::vector<std::vector<char>> picture;
    std::map<char, Coordinate> keyLocations;
    std::map<char, Coordinate> doorLocations;
    std::vector<Coordinate> initialPlayerLocs;
};

struct GameState {
    const Board * const board;
    KeySet foundKeys;
    std::vector<Coordinate> playerLocs;
    unsigned int movedSoFar;

    GameState (const Board * const b) : board {b}, foundKeys {}, playerLocs {b->initialPlayerLocs}, movedSoFar {0U} {}

    char getUnderlyingSymbol (Coordinate const& where) const {
        if (where.row < 0 || where.col < 0 || where.row >= (int)board->picture.size () || where.col >= (int)board->picture[where.row].size ()) { return OFF_MAP; }
        else {
            for (std::pair<const char, Coordinate> const& key : board->keyLocations) {
                if (key.second == where) {
                    return key.first;
                }
            }
            for (std::pair<const char, Coordinate> const& door : board->doorLocations) {
                if (door.second == where) {
                    return door.first;
                }
            }
            return board->picture[where.row][where.col];
        }

    }

    char getSymbol (Coordinate const& where) const {
        if (where.row < 0 || where.col < 0 || where.row >= (int)board->picture.size () || where.col >= (int)board->picture[where.row].size ()) { return OFF_MAP; }
        else {
            for (Coordinate const& c : playerLocs) {
                if (c == where) { return PLAYER; }
            }
            for (std::pair<const char, Coordinate> const& key : board->keyLocations) {
                if (key.second == where && !foundKeys.hasKey (key.first)) {
                    return key.first;
                }
            }
            for (std::pair<const char, Coordinate> const& door : board->doorLocations) {
                if (door.second == where && !foundKeys.hasKey (doorToKey (door.first))) {
                    return door.first;
                }
            }
            return board->picture[where.row][where.col];
        }
    }
};

std::ostream& operator<< (std::ostream & out, GameState const& state) {
    for (unsigned int row {0U}; row < state.board->picture.size (); ++row) {
        for (unsigned int col {0U}; col < state.board->picture[row].size (); ++col) {
            out << state.getSymbol ({(int)row, (int)col});
        }
        out << "\n";
    }
    out << "Found keys: " << state.foundKeys << "\n";
    out << "Moves so far: " << state.movedSoFar << "\n";
    return out;
}

Board getInput () {
    Board originalBoard;
    std::string line;
    int row {0};
    while (std::cin >> line) {
        originalBoard.picture.push_back ({});
        for (int col {0}; col < (int)line.size (); ++col) {
            char symbol = line[col];
            assert (symbol == PASSAGE || symbol == WALL || symbol == PLAYER || std::islower (symbol) || std::isupper (symbol));
            if (std::islower (symbol)) {
                assert (originalBoard.keyLocations.count (symbol) == 0);
                originalBoard.keyLocations[symbol] = {row, col};
                originalBoard.picture.back ().push_back (PASSAGE);
            }
            else if (std::isupper (symbol)) {
                assert (originalBoard.doorLocations.count (symbol) == 0);
                originalBoard.doorLocations[symbol] = {row, col};
                originalBoard.picture.back ().push_back (PASSAGE);
            }
            else if (symbol == PLAYER) {
                originalBoard.initialPlayerLocs.push_back ({row, col});
                originalBoard.picture.back ().push_back (PASSAGE);
            }
            else {
                originalBoard.picture.back ().push_back (symbol);
            }
        }
        ++row;
    }
    return originalBoard;
}

std::vector<Coordinate> getNeighbors (Coordinate c) {
    return { {c.row - 1, c.col}, {c.row + 1, c.col}, {c.row, c.col - 1}, {c.row, c.col + 1} };
}

Coordinate findClosest (std::unordered_map<Coordinate, unsigned int> const& map) {
    Coordinate chosen = map.begin ()->first;
    for (std::pair<Coordinate const, unsigned int> const& pair : map) {
        if (pair.second < map.at (chosen)) {
            chosen = pair.first;
        }
    }
    return chosen;
}

std::vector<std::pair<Coordinate, unsigned int>> getReachableKeys (GameState const& state, unsigned int playerIndex) {
    static std::unordered_map<Coordinate, std::vector<std::pair<Coordinate, std::pair<KeySet, unsigned int>>>> cache;
    if (cache.count (state.playerLocs[playerIndex]) == 0) {
        cache[state.playerLocs[playerIndex]] = {};
        std::unordered_map<Coordinate, unsigned int> finished;
        std::unordered_map<Coordinate, unsigned int> unfinished;
        std::unordered_map<Coordinate, KeySet> keysNeeded;
        unfinished.insert ({state.playerLocs[playerIndex], 0U});
        keysNeeded.insert ({state.playerLocs[playerIndex], {}});
        while (!unfinished.empty ()) {
            Coordinate currentLoc = findClosest (unfinished);
            unsigned int currentDist = unfinished[currentLoc];
            unfinished.erase (currentLoc);
            std::vector<Coordinate> neighbors = getNeighbors (currentLoc);
            for (Coordinate neighbor : neighbors) {
                if (finished.count (neighbor) == 0) {
                    char symbol = state.getUnderlyingSymbol (neighbor);
                    if (symbol != WALL) {
                        if (unfinished.count (neighbor) == 0 || unfinished[neighbor] > currentDist + 1) {
                            unfinished[neighbor] = currentDist + 1;
                            keysNeeded[neighbor] = keysNeeded[currentLoc];
                            if (isDoor (symbol)) { keysNeeded[neighbor].addKey (doorToKey (symbol)); }
                        }
                    }
                }
            }
            finished[currentLoc] = currentDist;
            if (std::islower (state.getUnderlyingSymbol (currentLoc))) {
                cache[state.playerLocs[playerIndex]].push_back ({currentLoc, {keysNeeded[currentLoc], currentDist}});
            }
        }
    }
    std::vector<std::pair<Coordinate, unsigned int>> result;
    for (std::pair<Coordinate, std::pair<KeySet, unsigned int>> const& pair : cache[state.playerLocs[playerIndex]]) {
        if (!state.foundKeys.hasKey (state.getUnderlyingSymbol (pair.first))) {
            if (state.foundKeys.containsAll (pair.second.first)) {
                result.push_back ({pair.first, pair.second.second});
            }
        }
    }
    return result;
}

template<typename T>
bool same (std::vector<T> const& x, std::vector<T> const& y) {
    if (x.size () != y.size ()) { return false; }
    for (unsigned int index {0U}; index < x.size (); ++index) {
        if (!(x[index] == y[index])) { return false; }
    }
    return true;
}

unsigned int findAllKeysBF (Board const& board) {
    std::deque<GameState *> frontier;
    unsigned int bestFinished {UINT_MAX};
    frontier.push_back (new GameState (&board));
    unsigned int count {0U};
    unsigned int skipped {0U};
    unsigned int keys = 0;
    while (!frontier.empty ()) {
        ++count;
        GameState * state = frontier.front ();
        frontier.pop_front ();
        if (state->foundKeys.size () > keys) {
            std::cout << "Finished exploring paths with " << keys << " keys found, frontier has " << frontier.size () << " states, skipped " << skipped << " duplicates.\n";
            ++keys;
            skipped = 0U;
        }
        if (state->movedSoFar >= bestFinished) { delete state; }
        else if (state->foundKeys.size () == state->board->keyLocations.size ()) {
            if (state->movedSoFar < bestFinished) { bestFinished = state->movedSoFar; }
            delete state;
        }
        else {
            for (unsigned int playerIndex {0U}; playerIndex < state->playerLocs.size (); ++playerIndex) {
                for (std::pair<Coordinate, unsigned int> const& move : getReachableKeys (*state, playerIndex)) {
                    GameState * successor = new GameState (*state);
                    successor->playerLocs[playerIndex] = move.first;
                    successor->movedSoFar += move.second;
                    successor->foundKeys.addKey (successor->getUnderlyingSymbol (move.first));
                    bool found = false;
                    for (GameState* existing : frontier) {
                        if (/*existing->playerLocs[0] == successor->playerLocs[0]*/ same (existing->playerLocs, successor->playerLocs) /*existing->playerLocs == successor->playerLocs*/ && existing->foundKeys == successor->foundKeys) {
                            found = true;
                            existing->movedSoFar = std::min (existing->movedSoFar, successor->movedSoFar);
                            ++skipped;
                            break;
                        }
                    }
                    if (found) { delete successor; }
                    else { frontier.push_back (successor); }
                }
            }
            delete state;
        }
    }
    std::cout << "Evaluated " << count << " states.\n";
    return bestFinished;
}

Board divideMap (Board const& original) {
    Board result (original);
    Coordinate center = result.initialPlayerLocs[0];
    result.picture[center.row][center.col] = WALL;
    result.picture[center.row - 1][center.col] = WALL;
    result.picture[center.row + 1][center.col] = WALL;
    result.picture[center.row][center.col - 1] = WALL;
    result.picture[center.row][center.col + 1] = WALL;
    result.initialPlayerLocs[0] = {center.row - 1, center.col - 1};
    result.initialPlayerLocs.push_back ({center.row - 1, center.col + 1});
    result.initialPlayerLocs.push_back ({center.row + 1, center.col - 1});
    result.initialPlayerLocs.push_back ({center.row + 1, center.col + 1});
    return result;
}

int main () {
    assert (isDoor ('B'));
    Board original = getInput ();
    //std::cout << initial << "\n";
    //std::cout << findAllKeysBF (initial) << "\n";
    Board revised = divideMap (original);
    std::cout << findAllKeysBF (revised) << "\n";
    return 0;
}
