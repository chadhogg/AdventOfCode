/// \file 2024Day04.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-04.
/// Part 1 is so elegant and general; part 2 is ... not.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>

using WordGrid = std::vector<std::string>;

/// @brief Reads a grid of characters.
/// @return A vector of strings, each of which is presumably the same length.
WordGrid
getInput ()
{
  WordGrid grid;
  std::string line;
  while (std::getline (std::cin, line)) {
    if (line != "") {
      grid.push_back (line);
    }
  }
  return grid;
}

enum Direction { HORIZ_FORWARD, HORIZ_BACKWARD, VERT_FORWARD, VERT_BACKWARD, UP_FORWARD, UP_BACKWARD, DOWN_FORWARD, DOWN_BACKWARD};

/// @brief Tests whether or not a partial string match can be successfully continued.
/// @param grid The word grid we are searching in.
/// @param word The word we are looking for.
/// @param ROWS The number of rows in the grid.
/// @param COLS The number of columns in the grid.
/// @param row The row from which our partial match started.
/// @param col The column from which our partial match started.
/// @param plus The number of characters that have already matched.
/// @param dir The direction the match is going in the grid.
/// @return True if the next character lies inside the bounds of the grid and matches the next character of the word.
bool
stillMatches (const WordGrid& grid, const std::string& word, const unsigned int ROWS, const unsigned int COLS, unsigned int row, unsigned int col, unsigned int plus, Direction dir)
{
  switch (dir) {
    case HORIZ_FORWARD: return (col + plus < COLS && grid.at (row).at (col + plus) == word.at (plus));
    case HORIZ_BACKWARD: return (col >= plus && grid.at (row).at (col - plus) == word.at (plus));
    case VERT_FORWARD: return (row + plus < ROWS && grid.at (row + plus).at (col) == word.at (plus));
    case VERT_BACKWARD: return (row >= plus && grid.at (row - plus).at (col) == word.at (plus));
    case UP_FORWARD: return (row >= plus && col + plus < COLS && grid.at (row - plus). at (col + plus) == word.at (plus));
    case UP_BACKWARD: return (row + plus < ROWS && col >= plus && grid.at (row + plus).at (col - plus) == word.at (plus));
    case DOWN_FORWARD: return (row + plus < ROWS && col + plus < COLS && grid.at (row + plus).at (col + plus) == word.at (plus));
    case DOWN_BACKWARD: return (row >= plus && col >= plus && grid.at (row - plus).at (col - plus) == word.at (plus));
  }
  assert (false);
  return false;
}

/// @brief Counts how many times a word appears in a word search.
/// @param grid The grid to search in.
/// @param word The word to search for.
/// @return The number of times it can be found horizontally, vertically, or along either diagonal, forwards or backwards.
long
countInstances (const WordGrid& grid, const std::string& word)
{
  const unsigned int ROWS = grid.size ();
  const unsigned int COLS = grid.at (0).length ();
  std::string backwards = word;
  std::reverse (backwards.begin (), backwards.end ());

  long count = 0;
  for (unsigned int row = 0; row < ROWS; ++row) {
    for (unsigned int col = 0; col < COLS; ++col) {
      std::set<Direction> possible;
      for (int dir = HORIZ_FORWARD; dir <= DOWN_BACKWARD; ++dir) { possible.insert(static_cast<Direction> (dir)); }

      for (unsigned plus = 0; plus < word.length () && !possible.empty (); ++plus) {
        for (auto it = possible.begin (); it != possible.end (); ) {
          if (!stillMatches (grid, word, ROWS, COLS, row, col, plus, *it)) { it = possible.erase(it); }
          else { ++it; }
        }
      }
      count += possible.size ();
    }
  }

  return count;
}

/// @brief Counts how many times a 3-letter word appears in a cross pattern.
/// @param grid The grid to search in.
/// @param word The 3-letter word to look for.
/// @return The number of times that pattern appears.
long
weirdPart2 (const WordGrid& grid, const std::string& word)
{
  assert (word.length () == 3);
  long count = 0;
  for (unsigned int centerRow = 1; centerRow < grid.size () - 1; ++centerRow) {
    for (unsigned int centerCol = 1; centerCol < grid.at (centerRow).length () - 1; ++centerCol) {
      if (grid.at (centerRow).at (centerCol) == word.at (1)) {
        char a = grid.at (centerRow - 1).at (centerCol - 1);
        char b = grid.at (centerRow + 1).at (centerCol + 1);
        if ((a == word.at (0) && b == word.at (2)) || (a == word.at (2) && b == word.at (0))) {
          char c = grid.at (centerRow - 1).at (centerCol + 1);
          char d = grid.at (centerRow + 1).at (centerCol - 1);
          if ((c == word.at (0) && d == word.at (2)) || (c == word.at (2) && d == word.at (0))) {
            ++count;
          }
        }
      }
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  WordGrid grid = getInput ();
  std::cout << countInstances (grid, "XMAS") << "\n";
  std::cout << weirdPart2 (grid, "MAS") << "\n";
}
