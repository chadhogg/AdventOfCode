/// \file 2016Day04.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-04.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <regex>
#include <map>

struct PossibleRoom
{
  std::string encyrptedName;
  int sectorId;
  std::string checksum;
};

std::vector<PossibleRoom>
readInput ()
{
  std::vector<PossibleRoom> rooms;
  std::string line;
  std::basic_regex reg ("^([a-z-]+)-(\\d\\d\\d)\\[([a-z]+)\\]$");
  while (std::getline (std::cin, line)) {
    std::smatch match;
    if (std::regex_match (line, match, reg)) {
      rooms.push_back ({match[1].str (), atoi (match[2].str ().c_str ()), match[3].str ()});
    }
    else {
      std::cerr << "Did not match.\n";
    }
  }
  return rooms;
}

bool
checksumMatches (const PossibleRoom& room)
{
  std::map<char, int> occurrences;
  for (char c : room.encyrptedName) {
    if (isalpha (c)) {
      if (!occurrences.contains (c)) { occurrences[c] = 0; }
      ++occurrences[c];
    }
  }
  std::string expected;
  while (expected.size () < room.checksum.size ()) {
    char most = occurrences.begin ()->first;
    for (auto it = occurrences.begin (); it != occurrences.end (); ++it) {
      if (it->second > occurrences[most]) { most = it->first; }
    }
    expected += most;
    occurrences.erase (most);
  }
  return expected == room.checksum;
}

void
removeDecoys (std::vector<PossibleRoom>& rooms)
{
  for (auto it = rooms.begin (); it != rooms.end (); ) {
    if (checksumMatches (*it)) { ++it; }
    else { it = rooms.erase (it); }
  }
}

int
part1 (const std::vector<PossibleRoom>& rooms)
{
  int total = 0;
  for (const PossibleRoom& room : rooms) {
    total += room.sectorId;
  }
  return total;
}

std::string
decrypt (const PossibleRoom& room)
{
  std::string result;
  for (char c : room.encyrptedName) {
    if (c == '-') { result.push_back (' '); }
    else { result.push_back (((c - 'a') + room.sectorId) % 26 + 'a'); }
  }
  return result;
}

void
part2 (const std::vector<PossibleRoom>& rooms)
{
  for (const PossibleRoom& room : rooms) {
    std::cout << decrypt (room) << " " << room.sectorId << "\n";
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<PossibleRoom> rooms = readInput ();
  removeDecoys (rooms);
  std::cout << part1 (rooms) << "\n";
  part2 (rooms);
  return 0;
}
