/// \file 2021Day04.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/4.


#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <array>
#include "utilities.hpp"

/// The length and width of a bingo board.
const int GRID_SIZE = 5;
/// A 5x5 array of some kind of data.
template <typename T>
using grid = std::array<std::array<T, GRID_SIZE>, GRID_SIZE>;

/// \brief A bingo card.
class Board {
public:
    /// \brief Constructs a new, empty bingo board.
    /// You should then fill it in by extracting it from an input stream.
    /// \post This is an empty board containing default values.
    Board () 
    : m_numbers {}, m_marked {} {
    }

    /// \brief Marks any copy of a number that exists on this board.
    /// \param[in] number The number to mark.
    /// \post Any copies of that number have been marked.
    void markNumber (int number) {
        for (unsigned int row {0U}; row < GRID_SIZE; ++row) {
            for (unsigned int col {0U}; col < GRID_SIZE; ++col) {
                if (m_numbers.at (row).at (col) == number) {
                    m_marked[row][col] = true;
                }
            }
        }
    }

    /// \brief Tests whether or not any row or column has been entirely marked.
    /// \return True if this bingo board has reached a win state, or false otherwise.
    bool hasWon () const {
        for (unsigned int row {0U}; row < GRID_SIZE; ++row) {
            bool allMarked = true;
            for (unsigned int col {0U}; col < GRID_SIZE; ++col) {
                if (!m_marked.at (row).at (col)) {
                    allMarked = false;
                }
            }
            if (allMarked) {
                return true;
            }
        }
        for (unsigned int col {0U}; col < GRID_SIZE; ++col) {
            bool allMarked = true;
            for (unsigned int row {0U}; row < GRID_SIZE; ++row) {
                if (!m_marked.at (row).at (col)) {
                    allMarked = false;
                }
            }
            if (allMarked) {
                return true;
            }
        }
        return false;
    }

    /// \brief Gets the sum of all unmarked values on this bingo card.
    /// \return The sum of all unmarkd values.
    int getUnmarkedSum () const {
        int sum {0};
        for (unsigned int row {0U}; row < GRID_SIZE; ++row) {
            for (unsigned int col {0U}; col < GRID_SIZE; ++col) {
                if (!m_marked.at (row).at (col)) {
                    sum += m_numbers.at (row).at (col);
                }
            }
        }
        return sum;
    }

    friend std::istream& operator>> (std::istream&in, Board& board);

private:

    /// A 5x5 matrix of numbers.
    grid<int> m_numbers;
    /// Whether or not each spot on the card has been marked.
    grid<bool> m_marked;
};

/// \brief Extracts a board from an input stream.
/// \param[inout] in The input stream.
/// \param[out] board The board to re-initialize with new values from the input stream.
/// \return The same input stream.
/// \post Twenty-five integers have been extracted from the input stream and stored in the board.
std::istream& operator>> (std::istream& in, Board& board) {
    for (unsigned int row {0U}; row < GRID_SIZE; ++row) {
        for (unsigned int col {0U}; col < GRID_SIZE; ++col) {
            board.m_numbers[row][col] = read<int> (in);
            board.m_marked[row][col] = false;
        }
    }
    return in;
}

/// \brief A bingo card-related problem.
class Problem {
public:
    /// \brief Constructs a new Problem, reading all of its values from an input stream.
    /// \param[inout] in The input stream.
    /// \post A line of called numbers and however many board representations exist have been extracted from the input stream and stored here.
    Problem (std::istream & in)
    : m_calledNumbers {}, m_boards {} {
        std::string numbersLine {read<std::string> (in)};
        std::stringstream stream;
        stream << numbersLine;
        std::string temp;
        while (getline (stream, temp, ',')) {
            m_calledNumbers.push_back (atoi (temp.c_str ()));
        }

        Board board;
        while (in >> board) {
            m_boards.push_back (Board (board));
        }
    }

    /// \brief Marks a number on all boards.
    /// \param[in] index The 0-based index of which number should be marked.
    /// \post All boards that contained that number have it marked.
    void markNumber (unsigned int index) {
        int number {m_calledNumbers[index]};
        for (Board & board : m_boards) {
            board.markNumber (number);
        }
    }

    /// \brief Gets a pointer to the (only) board that is in a winning state.
    /// \return A pointer to the winning board, or NULL if there are none yet.
    ///   Note that this points to an object that is owned and managed by this problem.
    const Board* getWinner () const {
        for (Board const& board : m_boards) {
            if (board.hasWon ()) {
                return &board;
            }
        }
        return NULL;
    }

    /// \brief Eliminates winning boards unless there is only one, in which case it is returned.
    /// \return A pointer to the only board, or NULL if that board is not in a winning state or
    ///   there is more than one remaining board.
    /// \post Any board that is winning and not the only one has been discarded.
    const Board* getLastWinner () {
        for (auto it = m_boards.begin (); it != m_boards.end (); /* intentionally blank */) {
            if ((*it).hasWon ()) {
                if (m_boards.size () == 1) {
                    return &(*it);
                }
                else {
                    m_boards.erase (it);
                }
            }
            else {
                ++it;
            }
        }
        return NULL;

    }

    /// \brief Gets one of the numbers that will be called in the bingo game.
    /// \param[in] index The 0-based index of which one will be called.
    /// \return That number.
    int getCalledNumber (unsigned int index) const {
        return m_calledNumbers.at (index);
    }

private:

    /// The sequence of numbers that will be called in the bingo game.
    std::vector<int> m_calledNumbers;
    /// The collection of active bingo boards.
    std::vector<Board> m_boards;
};


/// \brief Runs the program.
/// \return Always 0.
int main () {
    Problem problem (std::cin);
    Problem copy (problem);

    const Board* winner = problem.getWinner ();
    unsigned int index {0U};
    while (winner == NULL) {
        problem.markNumber (index);
        winner = problem.getWinner ();
        if (winner != NULL) {
            std::cout << winner->getUnmarkedSum () * problem.getCalledNumber (index) << "\n";
        }
        ++index;
    }

    winner = copy.getLastWinner ();
    index = 0U;
    while (winner == NULL) {
        problem.markNumber (index);
        winner = problem.getLastWinner ();
        if (winner != NULL) {
            std::cout << winner->getUnmarkedSum () * problem.getCalledNumber (index) << "\n";
        }
        ++index;
    }
    return 0;
}