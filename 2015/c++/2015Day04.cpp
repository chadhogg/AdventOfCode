/// \file 2015Day04.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-04.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <map>
#include <openssl/md5.h>

std::string
md5ToString(unsigned char* md, long size = MD5_DIGEST_LENGTH) {
  std::stringstream sout;
  for (int i=0; i<size; i++) {
    sout << std::hex << std::setw(2) << std::setfill('0') << (int) md[i];
  }
  return sout.str ();
}


long long
findStart (const std::string& input, std::string match) 
{
  for (long long i = 0; ; ++i) {
    std::string test = input + std::to_string (i);
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5 ((unsigned char*)test.c_str (), test.size (), result);
    if (md5ToString (result).substr (0, match.size ()) == match) {
      return i;
    }
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input;
  std::cin >> input;
  std::cout << findStart (input, "00000") << "\n";
  std::cout << findStart (input, "000000") << "\n";
  return 0;
}
