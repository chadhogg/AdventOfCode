/// \file 2015Day12.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-12.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

std::string
readInput ()
{
  std::string line;
  std::getline (std::cin, line);
  return line;
}

long long
sumNumbers (const std::string& input) {
  bool inString = false;
  long long total = 0;
  for (std::size_t index = 0; index < input.size (); ) {
    if (input.at (index) == '\"') {
      inString = !inString;
      ++index;
    }
    else if (inString || !(isdigit (input.at (index)) || input.at (index) == '-')) {
      ++index;
    }
    else {
      std::string digits = "";
      while (index < input.size () && (isdigit (input.at (index)) || input.at (index) == '-')) {
        digits += input.at (index);
        ++index;
      }
      total += atoi (digits.c_str ());
    }
  }
  return total;
}

struct JSONThing
{
  virtual ~JSONThing () {};
  virtual std::string to_string () const = 0;
  virtual long long sumNumbers (const std::string& skipValue = "") const = 0;
};

struct JSONString : public JSONThing
{
  std::string str;

  JSONString (const std::string& s) : str (s) {}
  virtual ~JSONString () {};
  virtual std::string to_string () const { return "\"" + str + "\""; }
  virtual long long sumNumbers (const std::string& skipValue = "") const { return 0; }
};

struct JSONNumber : public JSONThing
{
  int num;

  JSONNumber (const int& i) : num (i) {}
  virtual ~JSONNumber () {};
  virtual std::string to_string () const { return "" + num; }
  virtual long long sumNumbers (const std::string& skipValue = "") const { return num; }
};

struct JSONArray : public JSONThing
{
  std::vector<JSONThing*> elements;

  virtual ~JSONArray ()
  {
    for (JSONThing* thing : elements) {
      delete thing;
    }
  }
  virtual std::string to_string () const
  {
    std::string result = "[";
    for (std::size_t index = 0; index < elements.size (); ++index) {
      if (index > 0) { result += ","; }
      result += elements.at (index)->to_string ();
    }
    return result + "]";
  }
  virtual long long sumNumbers (const std::string& skipValue = "") const
  {
    long long total = 0;
    for (JSONThing* p : elements) {
      total += p->sumNumbers (skipValue);
    }
    return total;
  }
};

struct JSONObject : public JSONThing
{
  std::map<std::string, JSONThing*> properties;

  virtual ~JSONObject ()
  {
    for (auto it = properties.begin (); it != properties.end (); ++it) {
      delete it->second;
    }
  }
  virtual std::string to_string () const
  {
    std::string result = "{";
    for (auto it = properties.cbegin (); it != properties.cend (); ++it) {
      if (it != properties.cbegin ()) { result += ","; }
      result += "\"" + it->first + "\":" + it->second->to_string ();
    }
    return result + "}";
  }
  virtual long long sumNumbers (const std::string& skipValue = "") const
  {
    long long total = 0;
    for (auto it = properties.cbegin (); it != properties.cend (); ++it) {
      total += it->second->sumNumbers (skipValue);
      if (skipValue != "" && it->second->to_string () == "\"" + skipValue + "\"") { return 0; }
    }
    return total;
  }
};

// Assumptions (which appear to be true for my input):
//   1: No whitespace, ever.
//   2: Strings never contain brackets, braces, commas, or colons.
JSONThing*
parse (const std::string& input) {
  if (input.at (0) == '\"' && input.at (input.size () - 1) == '\"') {
    return new JSONString (input.substr (1, input.size () - 2));
  }
  else if (input.at (0) == '-' || isdigit (input.at (0))) {
    return new JSONNumber (atoi (input.c_str ()));
  }
  else if (input.at (0) == '[') {
    JSONArray* array = new JSONArray ();
    std::size_t start = 1;
    int bracketPairs = 0;
    int bracePairs = 0;
    for (std::size_t current = start; current < input.size () - 1; ++current) {
      if (input.at (current) == '[') { ++bracketPairs; }
      else if (input.at (current) == ']') { assert (bracketPairs > 0); --bracketPairs; }
      else if (input.at (current) == '{') { ++bracePairs; }
      else if (input.at (current) == '}') { assert (bracePairs > 0); --bracePairs; }
      else if (input.at (current) == ',' && bracketPairs == 0 && bracePairs == 0) {
        array->elements.push_back (parse (input.substr (start, current - start)));
        start = current + 1;
      }
    }
    if (start < input.size () - 1) {
      array->elements.push_back (parse (input.substr (start, input.size () - 1 - start)));
    }
    return array;
  }
  else if (input.at (0) == '{') {
    JSONObject* object = new JSONObject ();
    std::size_t start = 1;
    for (std::size_t current = 1; current < input.size () - 1; ) {
      assert (input.at (start) == '\"');
      std::size_t colon = input.find (':', current);
      std::string key = input.substr (start + 1, colon - 2 - start);
      std::size_t current2;
      int bracketPairs = 0;
      int bracePairs = 0;
      for (current2 = colon + 1; current2 < input.size () - 1; ++current2) {
        if (input.at (current2) == '[') { ++bracketPairs; }
        else if (input.at (current2) == ']') { assert (bracketPairs > 0); --bracketPairs; }
        else if (input.at (current2) == '{') { ++bracePairs; }
        else if (input.at (current2) == '}') { assert (bracePairs > 0); --bracePairs; }
        else if (input.at (current2) == ',' && bracketPairs == 0 && bracePairs == 0) {
          break;
        }
      }
      JSONThing* value = parse (input.substr (colon + 1, current2 - 1 - colon));
      object->properties[key] = value;
      start = current2 + 1;
      current = start;
    }
    return object;
  }
  else { throw "Impossible"; }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input = readInput ();
  std::cout << sumNumbers (input) << "\n";

  JSONThing* json = parse (input);
  std::cout << json->sumNumbers () << "\n";
  std::cout << json->sumNumbers ("red") << "\n";
  delete json;

  return 0;
}

