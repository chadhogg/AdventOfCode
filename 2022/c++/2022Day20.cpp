/// \file 2022Day20.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-20.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>

using File = std::list<int>;

bool
contains (const File& file, int value)
{
  for (int val : file)
  {
    if (val == value) { return true; }
  }
  return false;
}

File
getInput ()
{
  File file;
  int number;
  std::cin >> number;
  while (std::cin)
  {
    //assert (!contains (file, number));
    file.push_back (number);
    std::cin >> number;
  }
  return file;
}

std::string
to_string (const File& file)
{
  std::string result = "[";
  for (auto it = file.begin (); it != file.end (); ++it)
  {
    result += std::to_string (*it);
    if (std::next (it) != file.end ()) { result += ", "; }
  }
  return result + "]";
}

/*
Case 1: Move X forward without wrapping from Y.
  We are currently in position Y.
  We want to insert before position X + Y.
  Just doing ++ Y times should work.
Example: move the 3 in [5, 3, 2, 1, 4, 7]
  It is at position 1.  We want to insert before position 4.
Case 2: Move X forward from Y, needing to wrap around the end with length of Z.
  We are currently in position Y.
  We want to insert before position (X + Y) % (Z - 1).
  This seems to require ++ (Y + 1) times.
Example: move the 3 in [5, 2, 1, 4, 3, 7]
  It is at position 4.  One ++ takes us to position 5, then to position 0, then to position 1. A fourth to position 2.
Case 3: Move X forward from Y, passing by itself at least once.
  We are currently in position Y.
  We want to insert before position (X + Y) % (Z - 1).
Example: Move the 5 in [2, 5, 1] -> [2, 1, 5] -> [5, 1, 2] -> [1, 5, 2] -> [1, 2, 5] -> [5, 2, 1]
*/

File
naiveMix (File file)
{
  std::cout << to_string (file) << "\n";
  std::cout << file.size () << "\n";
  File mixed = file;
  for (auto value : file)
  {
    auto originalLocation = mixed.begin ();
    while (*originalLocation != value) { ++originalLocation; }

    auto finalLocation = originalLocation;
    if (value > 0)
    {
      int moves = value % (mixed.size () - 1);
      for (int i = 0; i < moves; ++i)
      {
        ++finalLocation;
        if (finalLocation == mixed.end ()) { finalLocation = mixed.begin (); }
      }
    }
    else if (value < 0)
    {
      int moves = - (-value % (mixed.size () - 1));
      for (int i = 0; i > moves; --i)
      {
        if (finalLocation == mixed.begin ()) { finalLocation = mixed.end (); }
        --finalLocation;
      }
    }
    std::cout << "Moving " << value << " from index " << std::distance (mixed.begin (), originalLocation) << " to index " << std::distance (mixed.begin (), finalLocation) << "\n";
    std::cout << "I think we want to move forward " << (*originalLocation + mixed.size () * 5) % mixed.size () << "\n";
    originalLocation = mixed.erase (originalLocation);
    mixed.insert (finalLocation, value);
    
    //if (finalLocation == mixed.begin ()) { finalLocation = mixed.end (); }
    //mixed.insert (finalLocation, *originalLocation);
    //mixed.erase (originalLocation);
    std::cout << to_string (mixed) << "\n";
    
  }
  return mixed;
}

std::size_t
find (const int& val, const std::vector<int*>& pointers)
{
  for (std::size_t index = 0; index < pointers.size (); ++index)
  {
    if (&val == pointers[index]) { return index; }
  }
  assert (false);
}

std::vector<int*>::iterator
next (std::vector<int*>& vec, const std::vector<int*>::iterator& it)
{
  std::vector<int*>::iterator copy (it);
  ++copy;
  if (copy == vec.end ()) { copy = vec.begin (); }
  return copy;
}

std::vector<int*>::iterator
prev (std::vector<int*>& vec, const std::vector<int*>::iterator& it)
{
  std::vector<int*>::iterator copy (it);
  if (copy == vec.begin ()) { copy = vec.end (); }
  --copy;
  return copy;
}

void
mySwap (std::vector<int*>::iterator& a, std::vector<int*>::iterator& b)
{
  int* temp = *a;
  *a = *b;
  *b = temp;
}

File
superSlowMix (File original)
{
  std::vector<int*> pointers;
  pointers.reserve (original.size ());
  for (int& val : original)
  {
    pointers.push_back (&val);
  }
  for (int& val : original)
  {
    std::size_t currentIndex = find (val, pointers);
    int count = val;
    std::vector<int*>::iterator it = pointers.begin () + currentIndex;
    while (count > 0)
    {
      std::vector<int*>::iterator it2 = next (pointers, it);
      mySwap (it, it2);
      it = it2;
      --count;
    }
    while (count < 0)
    {
      std::vector<int*>::iterator it2 = prev (pointers, it);
      mySwap (it, it2);
      it = it2;
      ++count;
    }
    //for (int* point : pointers)
    //{
    //  std::cout << *point << " ";
    //}
    //std::cout << "\n";
  }
  File result;
  for (int* point : pointers)
  {
    result.push_back (*point);
  }
  return result;
}

int
part1 (const File& mixed)
{
  int result = 0;
  auto it = mixed.begin ();
  while (*it != 0) { ++it; }
  for (int outer = 0; outer < 3; ++outer)
  {
    for (int inner = 0; inner < 1000; ++inner)
    {
      ++it;
      if (it == mixed.end ()) { it = mixed.begin (); }
    }
    result += *it;
  }
  return result;
}

int main ()
{
  File file = getInput ();
  File mixed = superSlowMix (file);
  //std::cout << to_string (mixed) << "\n";
  std::cout << part1 (mixed) << "\n";
  return 0;
}

// My answers:
// Part 1: 1591
// Part 2: 
