/// \file 2015Day02.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-02.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using Number = int;

struct Box
{
  Number height;
  Number width;
  Number length;

  Number
  getSurfaceArea () const
  {
    return 2 * height * width + 2 * height * length + 2 * width * length;
  }

  Number
  getSmallestSideArea () const
  {
    Number a = height * width;
    Number b = height * length;
    Number c = width * length;
    return std::min ({a, b, c});
  }

  Number
  getSmallestSidePerimeter () const
  {
    Number a = 2 * (height + width);
    Number b = 2 * (height + length);
    Number c = 2 * (width + length);
    return std::min ({a, b, c});
  }

  Number
  getVolume () const
  {
    return height * width * length;
  }
};

std::vector<Box>
readInput ()
{
  std::vector<Box> boxes;
  Number a, b, c;
  while (scanf ("%dx%dx%d", &a, &b, &c) == 3) {
    boxes.push_back ({a, b, c});
    scanf ("\n");
  }
  return boxes;
}

Number
part1 (const std::vector<Box>& boxes)
{
  Number total = 0;
  for (const Box& box : boxes) {
    total += box.getSurfaceArea () + box.getSmallestSideArea ();
  }
  return total;
}

Number
part2 (const std::vector<Box>& boxes)
{
  Number total = 0;
  for (const Box& box: boxes) {
    total += box.getSmallestSidePerimeter () + box.getVolume ();
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Box> boxes = readInput ();
  std::cout << part1 (boxes) << "\n";
  std::cout << part2 (boxes) << "\n";
  return 0;
}
