/// \file 2025Day05.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-05.

#include <cstddef>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using Number = long long;

using Range = std::pair<Number, Number>;

struct Problem
{
  std::vector<Range> ranges;
  std::vector<Number> ids;
};

Problem
getInput ()
{
  Problem prob;
  Number x, y;
  while (scanf ("%lld-%lld", &x, &y) == 2) {
    prob.ranges.push_back ({x, y});
  }
  // Partial success on the last match.
  prob.ids.push_back (x);
  while (scanf ("%lld", &x) == 1) {
    prob.ids.push_back (x);
  }
  return prob;
}

bool
isInARange (const Problem& prob, Number id)
{
  for (const Range& range : prob.ranges) {
    if (id >= range.first && id <= range.second) { return true; }
  }
  return false;
}

Number
countFreshIngredients (const Problem& prob) {
  Number count = 0;
  for (Number id : prob.ids) {
    if (isInARange (prob, id)) { ++count; }
  }
  return count;
}

bool
firstContainsSecond (const Range& r1, const Range& r2) {
  return (r1.first <= r2.first && r1.second >= r2.second);
}

bool
firstIsBeforeSecond (const Range& r1, const Range& r2) {
  return (r1.second < r2.first);
}

bool
firstExtendsIntoSecond (const Range& r1, const Range r2) {
  return (r1.first <= r2.first && r1.second <= r2.second && r2.first <= r1.second);
}

bool
firstStopsRightBeforeSecond (const Range& r1, const Range r2) {
  return (r1.second == r2.first);
}

Number
mergeRanges (const std::vector<Range>& original) {
  std::vector<Range> ranges = original;
  std::sort (ranges.begin (), ranges.end ());
  // I changed it to restart the loop on every change, which seems necessary for some reason.
  for (std::size_t index = 0; index < ranges.size () - 1; ) {
    Range& r1 = ranges[index];
    Range& r2 = ranges[index + 1];
    // If first range falls entirely within second range, remove first range.  Index stays the same.
    if (firstContainsSecond (r2, r1)) {
      ranges.erase (ranges.begin () + index);
      index = 0;
    }
    // If the second range falls entirely within first range, remove second range.  Index stays the same.
    else if (firstContainsSecond (r1, r2)) {
      ranges.erase (ranges.begin () + index + 1);
      index = 0;
    }
    // If first range falls entirely before second range, move on.
    else if (firstIsBeforeSecond (r1, r2)) {
      ++index;
    }
    // After sorting, it should not be possible for first range to fall entirely after second range.
    else if (firstIsBeforeSecond (r2, r1)) {
      assert (false);
    }
    // If first range partially overlaps second range from below, merge them.  Index stays the same.
    else if (firstExtendsIntoSecond (r1, r2)) {
      r2.first = r1.second + 1;
      index = 0;
    }
    // If they are adjacent, merge them.  Index stays the same.
    else if (firstStopsRightBeforeSecond (r1, r2)) {
      r1.second = r2.second;
      ranges.erase (ranges.begin () + index + 1);
      index = 0;
    }
    // After sorting, no other cases should be possible.
    else {
      assert (false);
    }
  }
  for (std::size_t index = 0; index < ranges.size () - 1; ++index) {
    assert (ranges[index].second < ranges[index + 1].first);
  }
  Number total = 0;
  for (const Range& range : ranges) {
    total += (range.second - range.first + 1);
  }
  return total;
}


int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << countFreshIngredients (prob) << "\n";
  std::cout << mergeRanges (prob.ranges) << "\n";
  return EXIT_SUCCESS;
}
