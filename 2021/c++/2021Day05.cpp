/// \file 2021Day05.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/5.

#include <iostream>
#include <string>
#include <vector>

#include "utilities.hpp"

/// \brief A point on a grid.
struct Point {
    /// \brief The horizontal position of the point.
    int col;
    /// \brief The vertical position of the point.
    int row;
};

/// \brief A line.
struct Line {
    /// \brief One of the line's endpoints.
    Point start;
    /// \brief Another of the line's endpoints.
    Point end;
};

/// \brief Reads a collection of lines from standard input.
/// \return A vector of lines.
std::vector<Line> getInput () {
    std::vector<Line> lines;
    std::string line;
    while (getline (std::cin, line)) {
        std::string firstHalf = line.substr (0, line.find ("->"));
        std::string secondHalf = line.substr (line.find ("->") + 2);
        Point p {atoi (firstHalf.substr (0, firstHalf.find (",")).c_str ()), atoi (firstHalf.substr (firstHalf.find (",") + 1).c_str ())};
        Point q {atoi (secondHalf.substr (0, secondHalf.find (",")).c_str ()), atoi (secondHalf.substr (secondHalf.find (",") + 1).c_str ())};        
        lines.push_back ({p, q});
    }
    return lines;
}

/// \brief Finds the largest column value in a collection of lines.
/// \param[in] lines The lines.
/// \return The largest column value that appears in the lines.
int getMaxCol (std::vector<Line> const& lines) {
    int max = 0;
    for (Line const& line : lines) {
        if(line.start.col > max) {
            max = line.start.col;
        }
        if(line.end.col > max) {
            max = line.end.col;
        }
    }
    return max;
}

/// \brief Finds the largest row value in a collection of lines.
/// \param[in] lines The lines.
/// \return The largest row value that appears in the lines.
int getMaxRow (std::vector<Line> const& lines) {
    int max = 0;
    for (Line const& line : lines) {
        if(line.start.row > max) {
            max = line.start.row;
        }
        if(line.end.row > max) {
            max = line.end.row;
        }
    }
    return max;
}

/// \brief Draws a picture made of lines, for debugging.
/// \param[in] picture The picture.
/// \post It has been drawn on the screen.
void drawPicture (std::vector<std::vector<int>> const& picture) {
    for (unsigned int row {0U}; row < picture.size (); ++row) {
        for (unsigned int col {0U}; col < picture.at (row).size (); ++col) {
            if (picture.at (row).at (col) == 0) {
                std::cout << ".";
            }
            else {
                std::cout << picture.at (row).at (col);
            }
        }
        std::cout << "\n";
    }
}

/// \brief Creates a picture containing a collection of lines.
/// \param[in] lines The lines.
/// \param[in] maxRow The largest row value that appears in the lines.
/// \param[in] maxCol The largest col value that appears in the lines.
/// \param[in] diagonals Whether or not to include diagonal lines.
/// \return A picture of the lines.
std::vector<std::vector<int>> drawLines (std::vector<Line> const& lines, int maxRow, int maxCol, bool diagonals) {
    std::vector<std::vector<int>> picture;
    for (int row {0}; row < maxRow + 1; ++row) {
        picture.push_back (std::vector<int> ());
        for (int col {0}; col < maxCol + 1; ++col) {
            picture.at (row).push_back (0);
        }
    }
    for (Line const& line : lines) {
        //drawPicture (picture);
        //std::cout << "About to draw line " << line.start.row << "," << line.start.col << " -> " << line.end.row << "," << line.end.col << "\n";
        if (line.start.row == line.end.row) {
            for (int col = std::min (line.start.col, line.end.col); col <= std::max (line.start.col, line.end.col); ++col) {
                ++picture[line.start.row][col];
            }
        }
        else if (line.start.col == line.end.col) {
            for (int row = std::min (line.start.row, line.end.row); row <= std::max (line.start.row, line.end.row); ++row) {
                ++picture[row][line.start.col];
            }
        }
        else if (diagonals) {
            if (line.start.row <= line.end.row) {
                if (line.start.col <= line.end.col) {
                    for (int offset = 0; offset + line.start.col <= line.end.col; ++offset) {
                        ++picture[line.start.row + offset][line.start.col + offset];
                    }
                }
                else {
                    for (int offset = 0; line.start.col - offset >= line.end.col; ++offset) {
                        ++picture[line.start.row + offset][line.start.col - offset];
                    }
                }
            }
            else {
                if (line.start.col <= line.end.col) {
                    for (int offset = 0; line.end.col - offset >= line.start.col; ++offset) {
                        ++picture[line.end.row + offset][line.end.col - offset];
                    }
                }
                else {
                    for (int offset = 0; line.end.col + offset <= line.start.col; ++offset) {
                        ++picture[line.end.row + offset][line.end.col + offset];
                    }
                }
            }
        }
    }
    return picture;
}

/// \brief Counts the number of points in a picture that are touched by multiple lines.
/// \param[in] picture The picture.
/// \return The number of points touched by multiple lines.
int countOverlaps (std::vector<std::vector<int>> const& picture) {
    int count = 0;
    for (unsigned int row {0U}; row < picture.size (); ++row) {
        for (unsigned int col {0U}; col < picture.at (row).size (); ++col) {
            if(picture.at (row).at (col) > 1) {
                ++count;
            }
        }
    }
    return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    std::vector<Line> lines = getInput ();
    std::vector<std::vector<int>> picture = drawLines (lines, getMaxRow (lines), getMaxCol (lines), false);
    std::cout << countOverlaps (picture) << "\n";
    picture = drawLines (lines, getMaxRow (lines), getMaxCol (lines), true);
    //drawPicture (picture);
    std::cout << countOverlaps (picture) << "\n";
    
    return 0;
}