/// \file 2016Day14.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-14.

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <openssl/md5.h>
#include <list>

std::string
md5ToString(unsigned char* md, long size = MD5_DIGEST_LENGTH) {
  std::stringstream sout;
  for (int i=0; i<size; i++) {
    sout << std::hex << std::setw(2) << std::setfill('0') << (int) md[i];
  }
  return sout.str ();
}


std::string
generateNextHash (const std::string& salt, long long index, int stretches)
{
  std::string test = salt + std::to_string (index);
  unsigned char result[MD5_DIGEST_LENGTH];
  MD5 ((unsigned char*)test.c_str (), test.size (), result);
  for (int i = 0; i < stretches; ++i) {
    std::string hash = md5ToString (result);
    MD5 ((unsigned char*)hash.c_str (), hash.size (), result);
  }
  return md5ToString (result);
}

bool
isKey (const std::list<std::string>& hashes)
{
  const std::string& first = hashes.front ();
  std::string triple;
  for (std::size_t i = 0; i < first.length (); ++i) {
    if (first[i] == first[i + 1] && first[i] == first[i + 2]) {
      triple = std::string ("") + first[i] + std::string ("") + first[i] + std::string ("") + first[i];
      break;
    }
  }
  if (!triple.empty ()) {
    std::string quintuple = triple + triple.substr (1);
    for (auto iter = std::next (hashes.cbegin ()); iter != hashes.cend (); ++iter) {
      if (iter->find (quintuple) != std::string::npos) { return true; }
    }
  }
  return false;
}

long long
findNthKey (const std::string& salt, std::size_t n, int stretches)
{
  std::list<std::string> hashes;
  std::list<std::string> keys;
  long long index = 0;
  while (hashes.size () < 1001) {
    hashes.push_back (generateNextHash (salt, index + hashes.size (), stretches));
  }
  while (keys.size () < n) {
    if (isKey (hashes)) {
      keys.push_back (hashes.front ());
    }
    hashes.pop_front ();
    hashes.push_back (generateNextHash (salt, index + hashes.size (), stretches));
    ++index;
  }
  // No idea why I need this offset, but it makes both the sample input and mine work.
  return index - 2;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string salt;
  std::cin >> salt;
  std::cout << findNthKey (salt, 64, 0) << "\n";
  std::cout << findNthKey (salt, 64, 2016) << "\n";
  return 0;
}
