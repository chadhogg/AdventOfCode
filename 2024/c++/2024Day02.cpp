/// \file 2024Day02.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-02.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using Level = int;
using Report = std::vector<Level>;
using Input = std::vector<Report>;

/// @brief Reads the input.
/// @return A series of reports to check.
Input
getInput ()
{
  Input problem;
  std::string line;
  while (std::getline (std::cin, line)) {
    // Surely there's a better way, but this works.
    std::stringstream stream(line);
    Report report;
    Level level;
    stream >> level;
    while (stream) {
      report.push_back (level);
      stream >> level;
    }
    problem.push_back (report);
  }
  return problem;
}

/// @brief Checks whether or not a report is safe.
/// @param report The report to check.
/// @return True if strictly increasing or strictly decreasing, and by amounts within a certain range.
bool
isSafe (const Report& report)
{
  const Level MIN_DIST = 1;
  const Level MAX_DIST = 3;
  bool increasing = report.at (0) < report.at (1);
  Level current = report.at (0);
  for (unsigned int index = 1; index < report.size (); ++index) {
    Level next = report.at (index);
    if (increasing && next <= current) { return false; }
    else if (!increasing && next >= current) { return false; }
    if (std::abs (next - current) < MIN_DIST || std::abs (next - current) > MAX_DIST) {
      return false;
    }
    current = next;
  }
  return true;
}

/// @brief Counts how many reports are safe.
/// @param problem The series of reports.
/// @return The number of them that are safe.
int
countSafeReports (const Input& problem)
{
  int count = 0;
  for (const Report& report : problem) {
    if (isSafe (report)) { ++count; }
  }
  return count;
}

/// @brief Tests whether or not a report is safe with the Problem Dampener.
/// @param report The report to test.
/// @return True if the report is safe, or could be made safe by removing one level.
bool
canBeMadeSafe (const Report& report)
{
  if (isSafe (report)) { return true; }
  for (unsigned int index = 0; index < report.size (); ++index) {
    Report copy = report;
    copy.erase (copy.begin () + index);
    if (isSafe (copy)) { return true; }
  }
  return false;
}

/// @brief Counts how many reports are safe with the Problem Dampener.
/// @param problem The series of reports.
/// @return How many of them can be made safe.
int
countCanBeSafeReports (const Input& problem)
{
  int count = 0;
  for (const Report& report : problem) {
    if (canBeMadeSafe (report)) { ++count; }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Input problem = getInput ();
  std::cout << countSafeReports (problem) << "\n";
  std::cout << countCanBeSafeReports (problem) << "\n";
  return 0;
}
