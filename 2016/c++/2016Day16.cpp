/// \file 2016Day16.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-16.

#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>

struct Data {
  std::vector<bool> bits;

  void
  reverse ()
  {
    std::reverse (bits.begin (), bits.end ());
  }

  void
  invert ()
  {
    for (std::size_t index = 0; index < bits.size (); ++index) {
      bits[index] = !bits[index];
    }
  }

  void
  combine (const Data& other)
  {
    bits.push_back (false);
    bits.insert (bits.end (), other.bits.begin (), other.bits.end ());
  }

  std::size_t
  length () const
  {
    return bits.size ();
  }

  Data
  checksum () const
  {
    Data result;
    for (std::size_t index = 0; index < bits.size (); index += 2) {
      result.bits.push_back (bits.at (index) == bits.at (index + 1));
    }
    return result;
  }

  void
  truncate (std::size_t maxLength)
  {
    assert (bits.size () >= maxLength);
    bits.erase (bits.begin () + maxLength, bits.end ());
  }
};

std::istream&
operator>> (std::istream& in, Data& data)
{
  std::string line;
  in >> line;
  for (char c: line) {
    assert (c == '0' || c == '1');
    data.bits.push_back (c == '1');
  }
  return in;
}

std::ostream&
operator<< (std::ostream& out, const Data& data)
{
  for (bool b : data.bits) {
    out << (b ? '1' : '0');
  }
  return out;
}

Data
fillDisk (const Data& original, std::size_t desiredLength)
{
  Data a = original;
  while (a.length () < desiredLength) {
    Data b = a;
    b.reverse ();
    b.invert ();
    a.combine (b);
  }
  return a;
}

Data
checksumUntilOddLength (const Data& original)
{
  Data checksum = original.checksum ();
  while (checksum.length () % 2 == 0) {
    checksum = checksum.checksum ();
  }
  return checksum;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Data original;
  std::cin >> original;
  Data first = fillDisk (original, 272);
  first.truncate (272);
  std::cout << checksumUntilOddLength (first) << "\n";
  Data second = fillDisk (original, 35651584);
  second.truncate (35651584);
  std::cout << checksumUntilOddLength (second) << "\n";
  return 0;
}
