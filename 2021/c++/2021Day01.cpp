/// \file 2021Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2021-12-01.

#include <iostream>
#include <vector>

/// \brief Reads the input for the problem, which is just a sequence of integers, one per line.
/// \return A vector containing all of the integers.
std::vector<int> getInput () {
  std::vector<int> readings;
  int n;
  while (std::cin >> n) {
    readings.push_back (n);
  }
  return readings;
}

/// \brief Counts the number of times an element of a vector is larger than the previous one.
/// \param[in] readings The vector.
/// \return The number of elements in it that are larger than the previous element.
int countIncreases (std::vector<int> const& readings) {
  int increases = 0;
  for (unsigned int index {1U}; index < readings.size (); ++index) {
    if (readings.at (index) > readings.at (index - 1)) {
      ++increases;
    }
  }
  return increases;
}

/// \brief Computes the sums of vector elements across a moving window.
/// \param[in] readings The vector.
/// \param[in] windowSize The length of the window, which must be positive.
/// \return A vector containing the sums of the values in each window.
std::vector<int> buildSums (std::vector<int> const& readings, unsigned int windowSize) {
  if (windowSize == 0) {
    throw std::runtime_error("Illegal window size.");
  }
  std::vector<int> sums;
  for (unsigned int index {windowSize - 1U}; index < readings.size (); ++index) {
    int total = 0;
    for (unsigned int back {0U}; back < windowSize; ++back) {
      total += readings.at (index - back);
    }
    sums.push_back (total);
  }
  return sums;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  std::vector<int> readings = getInput ();
  std::cout << countIncreases (readings) << std::endl;
  std::cout << countIncreases (buildSums (readings, 3)) << std::endl;
  return 0;
}
