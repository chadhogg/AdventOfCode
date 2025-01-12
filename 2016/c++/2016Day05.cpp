/// \file 2016Day05.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-05.

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

std::string
findPassword (const std::string& door)
{
  std::string result;
  long long i = 0;
  while (result.size () < 8) {
    std::string test = door + std::to_string (i);
    unsigned char chars[MD5_DIGEST_LENGTH];
    MD5 ((unsigned char*)test.c_str (), test.size (), chars);
    std::string str = md5ToString (chars);
    if (str.substr (0, 5) == "00000") {
      result.push_back (str[5]);
    }
    ++i;
  }
  return result;
}

std::string
findPassword2 (const std::string& door)
{
  std::string result = "        ";
  long long i = 0;
  while (result.find (' ') != std::string::npos) {
    std::string test = door + std::to_string (i);
    unsigned char chars[MD5_DIGEST_LENGTH];
    MD5 ((unsigned char*)test.c_str (), test.size (), chars);
    std::string str = md5ToString (chars);
    if (str.substr (0, 5) == "00000") {
      int pos = atoi (str.substr (5, 1).c_str ());
      if (pos < 8 && result[pos] == ' ') {
        result[pos] =  str[6];
      }
    }
    ++i;
  }
  return result;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input;
  std::cin >> input;
  std::cout << findPassword (input) << "\n";
  std::cout << findPassword2 (input) << "\n";
  return 0;
}
