/// \file 2016Day09.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-09.

#include <iostream>
#include <string>
#include <cassert>
#include <regex>

std::string
decompress (const std::string& compressed)
{
  std::string output;
  std::string rest = compressed;
  std::basic_regex regex ("\\((\\d+)x(\\d+)\\)");
  while (!rest.empty ()) {
    std::smatch match;
    if (std::regex_search (rest, match, regex)) {
      int length = atoi (match[1].str ().c_str ());
      int count = atoi (match[2].str ().c_str ());
      std::string marker = "(" + std::to_string (length) + "x" + std::to_string (count) + ")";
      std::size_t start = rest.find (marker);
      assert (start != std::string::npos);
      output += rest.substr (0, start);
      rest = rest.substr (start + marker.size ());
      std::string repeated = rest.substr (0, length);
      for (int i = 0; i < count; ++i) {
        output += repeated;
      }
      rest = rest.substr (length);
    }
    else {
      output += rest;
      rest = "";
    }
  }
  return output;
}

std::size_t
decompress2 (const std::string& compressed)
{
  std::size_t output = 0;
  std::string rest = compressed;
  std::basic_regex regex ("\\((\\d+)x(\\d+)\\)");
  while (!rest.empty ()) {
    std::smatch match;
    if (std::regex_search (rest, match, regex)) {
      int length = atoi (match[1].str ().c_str ());
      int count = atoi (match[2].str ().c_str ());
      std::string marker = "(" + std::to_string (length) + "x" + std::to_string (count) + ")";
      std::size_t start = rest.find (marker);
      assert (start != std::string::npos);
      output += start;
      rest = rest.substr (start + marker.size ());
      std::string repeated = rest.substr (0, length);
      output += decompress2 (repeated) * count;
      rest = rest.substr (length);
    }
    else {
      output += rest.length ();
      rest = "";
    }
  }
  return output;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string compressed;
  std::cin >> compressed;
  std::string uncompressed = decompress (compressed);
  assert (uncompressed.find (' ') == std::string::npos);
  assert (uncompressed.find ('\t') == std::string::npos);
  assert (uncompressed.find ('\n') == std::string::npos);
  std::cout << uncompressed.size () << "\n";
  std::cout << decompress2 (compressed) << "\n";
  return 0;
}

