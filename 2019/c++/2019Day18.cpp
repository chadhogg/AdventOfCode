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

#include "utilities.hpp"

constexpr char PLAYER = '@';
constexpr char WALL = '#';
constexpr char PASSAGE = '.';
constexpr char OFF_MAP = '?';

template<typename T>
using Grid = std::vector<std::vector<T>>;
using Vault = Grid<char>;

struct GameState {
    Vault picture;
    std::map<char, Coordinate> keyLocations;
    std::map<char, Coordinate> doorLocations;
    std::set<char> foundKeys;
    Coordinate playerLoc;
    unsigned int movedSoFar;

    char getUnderlyingSymbol (Coordinate const& where) const {
        if (where.row < 0 || where.col < 0 || where.row >= (int)picture.size () || where.col >= (int)picture[where.row].size ()) { return OFF_MAP; }
        else {
            for (std::pair<char, Coordinate> key : keyLocations) {
                if (key.second == where) {
                    return key.first;
                }
            }
            for (std::pair<char, Coordinate> door : doorLocations) {
                if (door.second == where) {
                    return door.first;
                }
            }
            return picture[where.row][where.col];
        }

    }

    char getSymbol (Coordinate const& where) const {
        if (where.row < 0 || where.col < 0 || where.row >= (int)picture.size () || where.col >= (int)picture[where.row].size ()) { return OFF_MAP; }
        else {
            if (playerLoc == where) { return PLAYER; }
            for (std::pair<char, Coordinate> key : keyLocations) {
                if (key.second == where && foundKeys.count (key.first) == 0) {
                    return key.first;
                }
            }
            for (std::pair<char, Coordinate> door : doorLocations) {
                if (door.second == where && foundKeys.count (std::tolower (door.first)) == 0) {
                    return door.first;
                }
            }
            return picture[where.row][where.col];
        }
    }
};

std::ostream& operator<< (std::ostream & out, GameState const& state) {
    for (unsigned int row {0U}; row < state.picture.size (); ++row) {
        for (unsigned int col {0U}; col < state.picture[row].size (); ++col) {
            out << state.getSymbol ({(int)row, (int)col});
        }
        out << "\n";
    }
    out << "Found keys: ";
    for (char key {'a'}; key <= 'z'; ++key) {
        if (state.foundKeys.count (key) == 1) { out << key; }
        else { out << " "; }
    }
    out << "\nMoves so far: " << state.movedSoFar << "\n";
    return out;
}

GameState getInput () {
    GameState state;
    std::string line;
    int row {0};
    while (std::cin >> line) {
        state.picture.push_back ({});
        for (int col {0}; col < (int)line.size (); ++col) {
            char symbol = line[col];
            assert (symbol == PASSAGE || symbol == WALL || symbol == PLAYER || std::islower (symbol) || std::isupper (symbol));
            if (std::islower (symbol)) {
                assert (state.keyLocations.count (symbol) == 0);
                state.keyLocations[symbol] = {row, col};
                state.picture.back ().push_back (PASSAGE);
            }
            else if (std::isupper (symbol)) {
                assert (state.doorLocations.count (symbol) == 0);
                state.doorLocations[symbol] = {row, col};
                state.picture.back ().push_back (PASSAGE);
            }
            else if (symbol == PLAYER) {
                state.playerLoc = {row, col};
                state.picture.back ().push_back (PASSAGE);
            }
            else {
                state.picture.back ().push_back (symbol);
            }
        }
        ++row;
    }
    state.movedSoFar = {0U};
    return state;
}

std::vector<Coordinate> getNeighbors (Coordinate c) {
    return { {c.row - 1, c.col}, {c.row + 1, c.col}, {c.row, c.col - 1}, {c.row, c.col + 1} };
}
/*
class ReachablilityCache {
public:

    bool isKnown (char source, char destination) const {
        return internal.count (source) == 1 && internal.at (source).count (destination) == 1;
    }

    void store (char source, char destination, std::set<char> const& neededKeys, unsigned int distance) {
        if (internal.count (source) == 0) { internal.insert ({source, {}}); }
        assert (internal[source].count (destination) == 0);
        internal[source][destination] = {neededKeys, distance};
    }

    std::set<char> const& getNeededKeys (char source, char destination) const {
        assert (isKnown (source, destination));
        return internal.at (source).at (destination).first;
    }

    unsigned int getDistance (char source, char destination) const {
        assert (isKnown (source, destination));
        return internal.at (source).at (destination).second;
    }

private:
    std::map<char, std::map<char, std::pair<std::set<char>, unsigned int>>> internal;
};

std::vector<std::pair<Coordinate, unsigned int>> getReachableKeys (GameState const& state) {
    static ReachablilityCache cache;
    std::vector<std::pair<Coordinate, unsigned int>> result;
    // todo
    return result;
}
*/

template<typename T>
bool isSubset (std::set<T> const& whole, std::set<T> const& part) {
    for (T const& obj : part) {
        if (whole.count (obj) == 0) { return false; }
    }
    return true;
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

std::vector<std::pair<Coordinate, unsigned int>> getReachableKeys (GameState const& state) {
    static std::unordered_map<Coordinate, std::vector<std::pair<Coordinate, std::pair<std::set<char>, unsigned int>>>> cache;
    if (cache.count (state.playerLoc) == 0) {
        cache[state.playerLoc] = {};
        std::unordered_map<Coordinate, unsigned int> finished;
        std::unordered_map<Coordinate, unsigned int> unfinished;
        std::unordered_map<Coordinate, std::set<char>> keysNeeded;
        unfinished.insert ({state.playerLoc, 0U});
        keysNeeded.insert ({state.playerLoc, {}});
        while (cache[state.playerLoc].size () < state.keyLocations.size ()) {
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
                            if (std::isupper (symbol)) { keysNeeded[neighbor].insert (std::tolower (symbol)); }
                        }
                    }
                }
            }
            finished[currentLoc] = currentDist;
            if (std::islower (state.getUnderlyingSymbol (currentLoc))) {
                cache[state.playerLoc].push_back ({currentLoc, {keysNeeded[currentLoc], currentDist}});
            }
        }
    }
    /* actually slowed things down, despite finding that there was lots of repetition
    static std::unordered_map<Coordinate, std::map<std::set<char>, std::vector<std::pair<Coordinate, unsigned int>>>> cache2;
    if (cache2.count (state.playerLoc) == 0) { cache2[state.playerLoc] = {}; }
    if (cache2[state.playerLoc].count (state.foundKeys) == 0) {
    */
    std::vector<std::pair<Coordinate, unsigned int>> result;
    for (std::pair<Coordinate, std::pair<std::set<char>, unsigned int>> const& pair : cache[state.playerLoc]) {
        if (state.foundKeys.count (state.getUnderlyingSymbol (pair.first)) == 0) {
            if (isSubset (state.foundKeys, pair.second.first)) {
                result.push_back ({pair.first, pair.second.second});
            }
        }
    }
    /*
    cache2[state.playerLoc][state.foundKeys] = result;
    }
    return cache2[state.playerLoc][state.foundKeys];
    */
    //std::cout << "Reachable from " << state.playerLoc << " is " << result << "\n";
    return result;
}

/*
std::unordered_map<Coordinate, unsigned int> getPathLengths (GameState const& state) {
    std::unordered_map<Coordinate, unsigned int> keyDistances;
    std::unordered_map<Coordinate, unsigned int> finished;
    std::unordered_map<Coordinate, unsigned int> unfinished;
    unfinished.insert ({state.playerLoc, 0U});
    while (!unfinished.empty ()) {
        Coordinate currentLoc = findClosest (unfinished);
        unsigned int currentDist = unfinished[currentLoc];
        unfinished.erase (currentLoc);
        std::vector<Coordinate> neighbors = getNeighbors (currentLoc);
        for (Coordinate neighbor : neighbors) {
            if (finished.count (neighbor) == 0) {
                char symbol = state.getSymbol (neighbor);
                if (symbol == PASSAGE || std::islower (symbol)) {
                    if (unfinished.count (neighbor) == 0 || unfinished[neighbor] > currentDist + 1) {
                        unfinished[neighbor] = currentDist + 1;
                    }
                }
            }
        }
        finished[currentLoc] = currentDist;
        if (std::islower (state.getSymbol (currentLoc))) {
            keyDistances[currentLoc] = currentDist;
        }
    }
    return keyDistances;
}

unsigned int findAllKeys (GameState & state, unsigned int cutoff) {
    if (state.foundKeys.size () == state.keyLocations.size ()) { 
        std::cout << state << "\n";
        return state.movedSoFar;
    }
    unsigned int lowestChild {cutoff};
    std::unordered_map<Coordinate, unsigned int> possibleMoves = getPathLengths (state);
    std::vector<std::pair<Coordinate, unsigned int>> sortedMoves;
    for (std::pair<const Coordinate, unsigned int> const& move : possibleMoves) {
        bool inserted {false};
        for (unsigned int index {0U}; index < sortedMoves.size () && !inserted; ++index) {
            if (move.second < sortedMoves[index].second) {
                sortedMoves.insert (sortedMoves.begin () + index, move);
                inserted = true;
            }
        }
        if (!inserted) { sortedMoves.push_back (move); }
    }
    for (std::pair<Coordinate, unsigned int> const& move : sortedMoves) {
        if (state.movedSoFar + move.second < cutoff) {
            Coordinate oldPlayerLoc = state.playerLoc;
            char key = state.getSymbol (move.first);
            assert (std::islower (key) && state.foundKeys.count (key) == 0);
            state.foundKeys.insert (key);
            state.playerLoc = move.first;
            state.movedSoFar += move.second;
            lowestChild = std::min (lowestChild, findAllKeys (state, lowestChild));
            state.movedSoFar -= move.second;
            state.playerLoc = oldPlayerLoc;
            state.foundKeys.erase (key);
        }
    }
    return lowestChild;
}
*/

unsigned int findAllKeys (GameState & state, unsigned int cutoff) {
    if (state.foundKeys.size () == state.keyLocations.size ()) { 
        std::cout << state << "\n";
        return state.movedSoFar;
    }
    unsigned int lowestChild {cutoff};
    std::vector<std::pair<Coordinate, unsigned int>> possibleMoves = getReachableKeys (state);
    for (std::pair<Coordinate, unsigned int> const& move : possibleMoves) {
        if (state.movedSoFar + move.second < cutoff) {
            Coordinate oldPlayerLoc = state.playerLoc;
            char key = state.getSymbol (move.first);
            assert (std::islower (key) && state.foundKeys.count (key) == 0);
            state.foundKeys.insert (key);
            state.playerLoc = move.first;
            state.movedSoFar += move.second;
            lowestChild = std::min (lowestChild, findAllKeys (state, lowestChild));
            state.movedSoFar -= move.second;
            state.playerLoc = oldPlayerLoc;
            state.foundKeys.erase (key);
        }
    }
    return lowestChild;
}

unsigned int findAllKeysBF (GameState const& initial) {
    std::deque<GameState *> frontier;
    unsigned int bestFinished {UINT_MAX};
    frontier.push_back (new GameState (initial));
    while (!frontier.empty ()) {
        GameState * state = frontier.front ();
        frontier.pop_front ();
        if (state->movedSoFar >= bestFinished) { delete state; }
        else if (state->foundKeys.size () == state->keyLocations.size ()) {
            if (state->movedSoFar < bestFinished) { bestFinished = state->movedSoFar; }
            delete state;
        }
        else {
            for (std::pair<Coordinate, unsigned int> const& move : getReachableKeys (*state)) {
                GameState * successor = new GameState (*state);
                successor->playerLoc = move.first;
                successor->movedSoFar += move.second;
                successor->foundKeys.insert (successor->getUnderlyingSymbol (move.first));
                bool found = false;
                for (GameState* existing : frontier) {
                    if (existing->playerLoc == successor->playerLoc && existing->foundKeys == successor->foundKeys) {
                        found = true;
                        existing->movedSoFar = std::min (existing->movedSoFar, successor->movedSoFar);
                        break;
                    }
                }
                if (found) { delete successor; }
                else { frontier.push_back (successor); }
            }
        }
    }
    return bestFinished;
}


int main () {
    GameState initial = getInput ();
    std::cout << initial << "\n";
    //std::cout << findAllKeys (initial, UINT_MAX) << "\n";
    std::cout << findAllKeysBF (initial) << "\n";
    return 0;
}
