/// \file 2015Day15.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-15.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <regex>
#include <list>
#include <set>
#include <algorithm>

struct Ingredient
{
  std::string name;
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
};

std::vector<Ingredient>
readInput ()
{
  std::vector<Ingredient> results;
  std::string line;
  std::basic_regex regex ("^(\\w+): capacity (-?\\d+), durability (-?\\d+), flavor (-?\\d+), texture (-?\\d+), calories (-?\\d+)$");
  while (std::getline (std::cin, line)) {
    std::smatch match;
    if (std::regex_match (line, match, regex)) {
      assert (match.size () == 7);
      results.push_back ({match[1].str (), atoi (match[2].str ().c_str ()), atoi (match[3].str ().c_str ()), atoi (match[4].str ().c_str ()), atoi (match[5].str ().c_str ()), atoi (match[6].str ().c_str ())});
    }
    else {
      std::cout << "\"" << line << "\" didn't match.\n";
    }
  }
  return results;
}

using Cookie = std::vector<int>;

long long
computeScore (const Cookie& cookie, const std::vector<Ingredient>& ingredients)
{
  assert (cookie.size () == ingredients.size ());
  long long capacity = 0, durability = 0, flavor = 0, texture = 0;
  for (std::size_t index = 0; index < cookie.size (); ++index) {
    capacity += cookie.at (index) * ingredients.at (index).capacity;
    durability += cookie.at (index) * ingredients.at (index).durability;
    flavor += cookie.at (index) * ingredients.at (index).flavor;
    texture += cookie.at (index) * ingredients.at (index).texture;
  }
  return std::max (0LL, capacity) * std::max (0LL, durability) * std::max (0LL, flavor) * std::max (0LL, texture);
}

long long
computeCalories (const Cookie& cookie, const std::vector<Ingredient>& ingredients)
{
  assert (cookie.size () == ingredients.size ());
  long long calories = 0;
  for (std::size_t index = 0; index < cookie.size (); ++index) {
    calories += cookie.at (index) * ingredients.at (index).calories;
  }
  return calories;
}

// Only works for there being exactly 4 ingredients, so sue me.
Cookie
findOptimalCookie (const std::vector<Ingredient>& ingredients, int desiredCalories = -1)
{
  Cookie bestCookie;
  long long bestScore = 0LL;
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; i + j <= 100; ++j) {
      for (int k = 0; i + j + k <= 100; ++k) {
        Cookie next({i, j, k, 100 - i - j - k});
        long long score = computeScore (next, ingredients);
        if (score > bestScore && (desiredCalories == -1 || desiredCalories == computeCalories (next, ingredients))) {
          bestCookie = next;
          bestScore = score;
        }
      }
    }
  }
  return bestCookie;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Ingredient> ingredients = readInput ();
  std::cout << computeScore (findOptimalCookie (ingredients), ingredients) << "\n";
  std::cout << computeScore (findOptimalCookie (ingredients, 500), ingredients) << "\n";
  return 0;
}
