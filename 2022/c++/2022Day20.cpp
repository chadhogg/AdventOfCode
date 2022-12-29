/// \file 2022Day20.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-20.

// For the longest time I had the correct answer for the sample, wrong answer for my input.
// This was because of an unwarranted, untrue assumption: that each number appeared exactly once.
// Because of that assumption, the way I was keeping track of which number should be processed
//   next was broken.
// Now that that's resolved I could probably go back to my original linked list implementation
//   with removal and insertion rather than swapping, but I'm not curious enough to go try it.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>

using Number = long;
using File = std::list<Number>;

bool
contains (const File& file, Number value)
{
  for (Number val : file)
  {
    if (val == value) { return true; }
  }
  return false;
}

File
getInput ()
{
  File file;
  Number number;
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

std::size_t
find (const Number& val, const std::vector<Number*>& pointers)
{
  for (std::size_t index = 0; index < pointers.size (); ++index)
  {
    if (&val == pointers[index]) { return index; }
  }
  assert (false);
}

std::vector<Number*>::iterator
next (std::vector<Number*>& vec, const std::vector<Number*>::iterator& it)
{
  std::vector<Number*>::iterator copy (it);
  ++copy;
  if (copy == vec.end ()) { copy = vec.begin (); }
  return copy;
}

std::vector<Number*>::iterator
prev (std::vector<Number*>& vec, const std::vector<Number*>::iterator& it)
{
  std::vector<Number*>::iterator copy (it);
  if (copy == vec.begin ()) { copy = vec.end (); }
  --copy;
  return copy;
}

// I'm not clear on why std::swap isn't working, but it's not.
void
mySwap (std::vector<Number*>::iterator& a, std::vector<Number*>::iterator& b)
{
  Number* temp = *a;
  *a = *b;
  *b = temp;
}

File
indirectVectorMix (File original, int times)
{
  std::vector<Number*> pointers;
  pointers.reserve (original.size ());
  for (Number& val : original)
  {
    pointers.push_back (&val);
  }
  for (int time = 0; time < times; ++time)
  {
    for (Number& val : original)
    {
      std::size_t currentIndex = find (val, pointers);
      Number count = val;
      std::vector<Number*>::iterator it = pointers.begin () + currentIndex;
      if (count > 0)
      {
        count = count % (pointers.size () - 1);
        while (count > 0)
        {
          std::vector<Number*>::iterator it2 = next (pointers, it);
          mySwap (it, it2);
          it = it2;
          --count;
        }
      }
      else
      {
        count = - (-count % (pointers.size () - 1));
        while (count < 0)
        {
          std::vector<Number*>::iterator it2 = prev (pointers, it);
          mySwap (it, it2);
          it = it2;
          ++count;
        }
      }
      //for (Number* point : pointers)
      //{
      //  std::cout << *point << " ";
      //}
      //std::cout << "\n";
    }
  }
  File result;
  for (Number* point : pointers)
  {
    result.push_back (*point);
  }
  return result;
}

Number
sumOfThree (const File& mixed)
{
  Number result = 0;
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

File
applyKey (const File& original)
{
  File result;
  const Number KEY = 811589153;
  for (auto val : original)
  {
    result.push_back (val * KEY);
  }
  return result;
}

int main ()
{
  File file = getInput ();
  File mixed = indirectVectorMix (file, 1);
  std::cout << sumOfThree (mixed) << "\n";
  File decrypted = applyKey (file);
  File manyMixed = indirectVectorMix (decrypted, 10);
  std::cout << sumOfThree (manyMixed) << "\n";
  return 0;
}

// My answers:
// Part 1: 1591
// Part 2: 14579387544492
