/// \file 2019Day20.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/20.

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

constexpr char WALL = '#';
constexpr char PASSAGE = '.';
constexpr char OFF_MAP = ' ';
const Coordinate NOWHERE {-1, -1};

struct Board {
    std::vector<std::vector<char>> picture;
    std::unordered_map<Coordinate, Coordinate> portals;
    Coordinate start;
    Coordinate goal;
};

std::vector<Coordinate> getNeighbors (Coordinate c) {
    return { {c.row - 1, c.col}, {c.row + 1, c.col}, {c.row, c.col - 1}, {c.row, c.col + 1} };
}

Coordinate getNeighboringLetter (Board const& board, Coordinate const& where) {
    for (Coordinate c : getNeighbors ({where.row, where.col})) {
        if (std::isupper (board.picture[c.row][c.col])) { return c; }
    }
    return NOWHERE;
}

Board getInput () {
    Board board;
    std::string line;
    while (std::getline (std::cin, line)) {
        board.picture.push_back ({});
        for (char c : line) {
            board.picture.back ().push_back (c);
        }
    }
    std::unordered_map<std::string, Coordinate> unmatched;
    for (int row {0}; row < (int)board.picture.size (); ++row) {
        for (int col {0}; col < (int)board.picture[row].size (); ++col) {
            if (board.picture[row][col] == PASSAGE) {
                Coordinate here {row, col};
                Coordinate neighbor = getNeighboringLetter (board, here);
                if (! (neighbor == NOWHERE)) {
                    Coordinate otherNeighbor = getNeighboringLetter (board, neighbor);
                    char letter1 = board.picture[neighbor.row][neighbor.col];
                    char letter2 = board.picture[otherNeighbor.row][otherNeighbor.col];
                    std::string letters = std::string("") + std::min (letter1, letter2) + "" + std::max (letter1, letter2);
                    if (letters == "AA") {
                        board.start = here;
                    }
                    else if (letters == "ZZ") {
                        board.goal = here;
                    }
                    else if (unmatched.count (letters) == 0) {
                        unmatched.insert ({letters, here});
                    }
                    else {
                        Coordinate match = unmatched[letters];
                        unmatched.erase (letters);
                        board.portals[match] = here;
                        board.portals[here] = match;
                    }
                }
            }
        }
    }
    return board;
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

unsigned int getShortestPathLength (Board const& board) {
    std::unordered_map<Coordinate, unsigned int> finished;
    std::unordered_map<Coordinate, unsigned int> unfinished;
    unfinished.insert ({board.start, 0U});
    while (finished.count (board.goal) == 0) {
        assert (!unfinished.empty ());
        Coordinate currentLoc = findClosest (unfinished);
        unsigned int currentDist = unfinished[currentLoc];
        unfinished.erase (currentLoc);
        std::vector<Coordinate> neighbors = getNeighbors (currentLoc);
        if (board.portals.count (currentLoc) == 1) {
            neighbors.push_back (board.portals.at (currentLoc));
        }
        for (Coordinate neighbor : neighbors) {
            if (finished.count (neighbor) == 0) {
                if (board.picture[neighbor.row][neighbor.col] == PASSAGE) {
                    if (unfinished.count (neighbor) == 0 || unfinished[neighbor] > currentDist + 1) {
                        unfinished[neighbor] = currentDist + 1;
                    }
                }
            }
        }
        finished[currentLoc] = currentDist;
    }
    return finished[board.goal];
}


int main () {
    Board board = getInput ();
    std::cout << "Shortest path start to goal: " << getShortestPathLength (board) << "\n";
    return 0;
}
