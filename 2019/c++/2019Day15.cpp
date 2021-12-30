/// \file 2019Day15.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/15.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <unordered_map>

#include "utilities.hpp"
#include "intcode.hpp"

enum Command {
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4
};

enum Status {
    HIT_WALL = 0,
    MOVED = 1,
    GOAL = 2
};

using Path = std::vector<Command>;

void drawPicture (std::unordered_set<Coordinate> const& visited, std::unordered_set<Coordinate> const& walls, Coordinate const& current, Coordinate const& upperLeft, Coordinate const& lowerRight) {
    for (int row {upperLeft.row}; row <= lowerRight.row; ++row) {
        for (int col {upperLeft.col}; col <= lowerRight.col; ++col) {
            Coordinate here {row, col};
            if (row == current.row && col == current.col) { std::cout << "D"; }
            else if (visited.find (here) != visited.end ()) { std::cout << "."; }
            else if (walls.find (here) != walls.end ()) { std::cout << "#"; }
            else { std::cout << " "; }
        }
        std::cout << "\n";
    }
}

Coordinate findClosest (std::unordered_map<Coordinate, Path> const& map) {
    Coordinate chosen = map.begin ()->first;
    for (std::pair<Coordinate const, Path> const& pair : map) {
        if (pair.second.size () < map.at (chosen).size ()) {
            chosen = pair.first;
        }
    }
    return chosen;
}

std::unordered_map<Coordinate, Path> shortestPaths (Coordinate start, std::unordered_set<Coordinate> const& goals, std::unordered_set<Coordinate> const& known) {
    std::unordered_map<Coordinate, Path> result;
    std::unordered_map<Coordinate, Path> finished;
    std::unordered_map<Coordinate, Path> visited;
    std::unordered_set<Coordinate> unvisited;
    visited.insert ({start, {}});
    while (result.size () < goals.size ()) {
        assert (!visited.empty ());
        Coordinate current = findClosest (visited);
        Path soFar = visited.at (current);
        visited.erase (current);

        if (known.count (current) == 1) {
            std::vector<std::pair<Coordinate, Path>> successors;
            successors.push_back ({{current.row - 1, current.col}, soFar});
            successors.push_back ({{current.row + 1, current.col}, soFar});
            successors.push_back ({{current.row, current.col - 1}, soFar});
            successors.push_back ({{current.row, current.col + 1}, soFar});
            successors[0].second.push_back (NORTH);
            successors[1].second.push_back (SOUTH);
            successors[2].second.push_back (WEST);
            successors[3].second.push_back (EAST);
            for (std::pair<Coordinate, Path> const& s : successors) {
                if (finished.count (s.first) == 0) {
                    if (visited.count (s.first) == 0) {
                        visited.insert ({s.first, s.second});
                    }
                    else if(visited.at (s.first).size () > s.second.size ()) {
                        visited[s.first] = s.second;
                    }
                }
            }
        }
        finished[current] = soFar;
        if (goals.count (current) == 1) {
            result[current] = soFar;
        }
    }
    return result;
}


Coordinate exploreManually (NumbersList & prog) {
    Coordinate current {0, 0};
    Coordinate upperLeft {0, 0};
    Coordinate lowerRight {0, 0};
    std::unordered_set<Coordinate> visited;
    visited.insert (current);
    std::unordered_set<Coordinate> walls;
    ICComputer comp;
    comp.loadProgram (prog, {});
    while (true) {
        drawPicture (visited, walls, current, upperLeft, lowerRight);
        std::string human;
        while (human != "w" && human != "a" && human != "s" && human != "d") {
            human = read<std::string> ();
        }
        int input = 0;
        switch (human[0]) {
            case 'w': input = NORTH; break;
            case 's': input = SOUTH; break;
            case 'a': input = WEST; break;
            case 'd': input = EAST; break;
        }
        comp.addInput (input);
        comp.executeUntilMissingInput ();
        Number output = comp.getOutputs ().back ();
        if (output == HIT_WALL) {
            switch (input) {
                case NORTH: walls.insert ({current.row - 1, current.col}); break;
                case SOUTH: walls.insert ({current.row + 1, current.col}); break;
                case WEST: walls.insert ({current.row, current.col - 1}); break;
                case EAST: walls.insert ({current.row, current.col + 1}); break;
            }
        }
        else {
            switch (input) {
                case NORTH: --current.row; break;
                case SOUTH: ++current.row; break;
                case WEST: --current.col; break;
                case EAST: ++current.col; break;
            }
            visited.insert (current);
            if (output == GOAL) { return current; }
        }
        switch (input) {
            case NORTH: upperLeft.row = std::min (upperLeft.row, current.row - 1); break;
            case SOUTH: lowerRight.row = std::max (lowerRight.row, current.row + 1); break;
            case WEST: upperLeft.col = std::min (upperLeft.col, current.col - 1); break;
            case EAST: lowerRight.col = std::max (lowerRight.col, current.col + 1); break;
        }
    }
}

unsigned int exploreAutomatically (NumbersList & prog) {
    Coordinate current {0, 0};
    Coordinate upperLeft {0, 0};
    Coordinate lowerRight {0, 0};
    std::unordered_set<Coordinate> visited;
    std::unordered_set<Coordinate> frontier;
    visited.insert (current);
    frontier.insert ({current.row - 1, current.col});
    frontier.insert ({current.row + 1, current.col});
    frontier.insert ({current.row, current.col - 1});
    frontier.insert ({current.row, current.col + 1});
    std::unordered_set<Coordinate> walls;
    std::unordered_set<Coordinate> goals;
    ICComputer comp;
    comp.loadProgram (prog, {});
    std::unordered_map<Coordinate, Path> paths = shortestPaths (current, frontier, visited);
    Coordinate goal = findClosest (paths);
    Path path = paths[goal];
    while (!frontier.empty ()) {
        drawPicture (visited, walls, current, upperLeft, lowerRight);
        if (path.empty ()) {
            paths = shortestPaths (current, frontier, visited);
            goal = findClosest (paths);
            path = paths[goal];
        }
        int input = path[0];
        path.erase (path.begin ());
        comp.addInput (input);
        comp.executeUntilMissingInput ();
        Number output = comp.getOutputs ().back ();
        if (output == HIT_WALL) {
            switch (input) {
                case NORTH: {
                    walls.insert ({current.row - 1, current.col});
                    frontier.erase ({current.row - 1, current.col});
                    break;
                }
                case SOUTH: {
                    walls.insert ({current.row + 1, current.col});
                    frontier.erase ({current.row + 1, current.col});
                    break;
                }
                case WEST: {
                    walls.insert ({current.row, current.col - 1});
                    frontier.erase ({current.row, current.col - 1});
                    break;
                }
                case EAST: {
                    walls.insert ({current.row, current.col + 1});
                    frontier.erase ({current.row, current.col + 1});
                    break;
                }
            }
        }
        else {
            switch (input) {
                case NORTH: { --current.row; break; }
                case SOUTH: { ++current.row; break; }
                case WEST: { --current.col; break; }
                case EAST: { ++current.col; break; }
            }
            visited.insert (current);
            frontier.erase (current);
            std::vector<Coordinate> neighbors { {current.row - 1, current.col}, {current.row + 1, current.col}, {current.row, current.col - 1}, {current.row, current.col + 1} };
            for (Coordinate const& neighbor : neighbors) {
                if (walls.count (neighbor) == 0 && visited.count (neighbor) == 0) {
                    frontier.insert (neighbor);
                }
            }
            if (output == GOAL) { goals.insert (current); }
        }
        switch (input) {
            case NORTH: { upperLeft.row = std::min (upperLeft.row, current.row - 1); break; }
            case SOUTH: { lowerRight.row = std::max (lowerRight.row, current.row + 1); break; }
            case WEST: { upperLeft.col = std::min (upperLeft.col, current.col - 1); break; }
            case EAST: { lowerRight.col = std::max (lowerRight.col, current.col + 1); break; }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds (50));   
    }
    paths = shortestPaths ({0, 0}, goals, visited);
    Coordinate reached = findClosest (paths);
    return paths.at (reached).size ();
}


/*
void drawPicture (Picture const& pic) {
    for (unsigned int row {0U}; row < pic.size (); ++row) {
        for (unsigned int col {0U}; col < pic.at (row).size (); ++col) {
            if (pic[row][col] == EMPTY) { std::cout << " "; }
            else { std::cout << pic[row][col]; }
        }
        std::cout << "\n";
    }
}

void playGame (NumbersList const& prog) {
    ICComputer comp;
    comp.loadProgram (prog, {});
    comp.executeAllInstructions ();
    Number maxCol {0}, maxRow {0};
    NumbersList output = comp.getOutputs ();
    assert (output.size () % 3 == 0);
    for (unsigned int index {0U}; index < output.size (); index += 3) {
        assert (output[index] >= 0);
        assert (output[index + 1] >= 0);
        maxCol = std::max (maxCol, output[index]);
        maxRow = std::max (maxRow, output[index + 1]);
        assert (output[index + 2] >= EMPTY && output[index + 2] <= BALL);
    }
    Picture pic (maxRow + 1, std::vector<int> (maxCol + 1, EMPTY));
    for (unsigned int index {0U}; index < output.size (); index += 3) {
        pic[output[index + 1]][output[index]] = output[index + 2];
    }
    drawPicture (pic);
    unsigned int blockCount {0U};
    for (unsigned int row {0U}; row < pic.size (); ++row) {
        for (unsigned int col {0U}; col < pic[row].size (); ++col) {
            if (pic[row][col] == BLOCK) { ++blockCount; }
        }
    }
    std::cout << blockCount << "\n";
}

void playGame2 (NumbersList const& prog) {
    ICComputer comp;
    NumbersList revisedProg = prog;
    revisedProg[0] = 2;
    comp.loadProgram (revisedProg, {});
    Number score {0};
    int ballCol {0};
    int paddleCol {0};
    while (!comp.isTerminated ()) {
        comp.executeUntilMissingInput ();
        Number maxCol {0}, maxRow {0};
        NumbersList output = comp.getOutputs ();
        assert (output.size () % 3 == 0);
        for (unsigned int index {0U}; index < output.size (); index += 3) {
            if (output[index] == -1 && output[index + 1] == 0) {
                score = output[index + 2];
            }
            else {
                assert (output[index] >= 0);
                assert (output[index + 1] >= 0);
                maxCol = std::max (maxCol, output[index]);
                maxRow = std::max (maxRow, output[index + 1]);
                assert (output[index + 2] >= EMPTY && output[index + 2] <= BALL);
            }
        }
        Picture pic;
        for (int row {0}; row <= maxRow; ++row) {
            pic.push_back ({});
            for (int col {0}; col <= maxCol; ++col) {
                pic.back ().push_back (EMPTY);
            }
        }
        for (unsigned int index {0U}; index < output.size (); index += 3) {
            if (output[index] == -1 && output[index + 1] == 0) {
                score = output[index + 2];
            }
            else {
                pic[output[index + 1]][output[index]] = output[index + 2];
                if (output[index + 2] == BALL) {
                    ballCol = output[index];
                }
                else if (output[index + 2] == PADDLE) {
                    paddleCol = output[index];
                }
            }
        }
        drawPicture (pic);
        std::cout << score << "\n";
        
        //std::string action;
        //do {
        //    action = read<std::string> ();
        //} while (action != "q" && action != "w" && action != "e");
        //switch (action[0]) {
        //    case 'q': comp.addInput (-1); break;
        //    case 'w': comp.addInput (0); break;
        //    case 'e': comp.addInput (1); break;
        //}
        
        if (paddleCol < ballCol) { comp.addInput (1); }
        else if (paddleCol > ballCol) { comp.addInput (-1); }
        else { comp.addInput (0); }
        std::this_thread::sleep_for (std::chrono::milliseconds (50));   
    }

}
*/


int main () {
    std::ifstream fin ("../inputs/Day15.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    //exploreManually (prog);
    //playGame (prog);
    //playGame2 (prog);
    std::cout << exploreAutomatically (prog) << "\n";
    return 0;
}
