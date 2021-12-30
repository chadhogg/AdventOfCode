/// \file 2019Day13.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/13.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "utilities.hpp"
#include "intcode.hpp"

enum Tile {
    EMPTY = 0,
    WALL = 1,
    BLOCK = 2,
    PADDLE = 3,
    BALL = 4
};

using Picture = std::vector<std::vector<int>>;

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
        /*
        std::string action;
        do {
            action = read<std::string> ();
        } while (action != "q" && action != "w" && action != "e");
        switch (action[0]) {
            case 'q': comp.addInput (-1); break;
            case 'w': comp.addInput (0); break;
            case 'e': comp.addInput (1); break;
        }
        */
        if (paddleCol < ballCol) { comp.addInput (1); }
        else if (paddleCol > ballCol) { comp.addInput (-1); }
        else { comp.addInput (0); }
        std::this_thread::sleep_for (std::chrono::milliseconds (50));   
    }

}

int main () {
    std::ifstream fin ("../inputs/Day13.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    //playGame (prog);
    playGame2 (prog);
    return 0;
}
