/// \file 2022Day07.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-07.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

struct File
{
  File (const std::string& n, unsigned long s) : m_name (n), m_size (s) { }
  const std::string m_name;
  const unsigned long m_size;
};

struct Dir
{
  Dir (std::string n, Dir* p) : m_name (n), m_parent (p), m_children ({}), m_files ({}) { } 
  const std::string m_name;
  Dir* m_parent;
  std::map<std::string, Dir*> m_children;
  std::map<std::string, File*> m_files;

  unsigned long
  getTotalSize ()
  {
    unsigned long total = 0U;
    for (const auto& pair : m_children)
    {
      total += pair.second->getTotalSize ();
    }
    for (const auto& pair : m_files)
    {
      total += pair.second->m_size;
    }
    return total;
  }

  unsigned long
  addTotalIfAtMost (unsigned limit)
  {
    unsigned long total = 0U;
    unsigned long mine = getTotalSize ();
    if (mine < limit)
    {
      total += mine;
    }
    for (const auto& pair : m_children)
    {
      total += pair.second->addTotalIfAtMost (limit);
    }
    return total;
  }

  unsigned long
  smallestSufficient (unsigned long needed)
  {
    unsigned long smallest = getTotalSize ();
    for (const auto& pair : m_children)
    {
      unsigned long current = pair.second->smallestSufficient (needed);
      if (current >= needed)
      {
        smallest = std::min (current, smallest);
      }
    }
    return smallest;
  }

};

Dir*
parseInput ()
{
  Dir* root = new Dir ("/", nullptr);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline (std::cin, line))
  {
    if (!line.empty ()) { lines.push_back (line); }
  }
  Dir* working = root;
  for (std::size_t index = 0; index < lines.size (); ++index)
  {
    const std::string& current = lines[index];
    assert (current.size () >= 1);
    if (current[0] == '$')
    {
      assert (current.size () >= 4);
      assert (current[1] == ' ');
      assert ((current[2] == 'c' && current[3] == 'd') || (current[2] == 'l' && current[3] == 's'));
      if (current[2] == 'c' && current[3] == 'd')
      {
        assert (current.size () >= 6);
        assert (current[4] == ' ');
        std::string where = current.substr (5);
        if (where == "/") { working = root; }
        else if (where == "..") { working = working->m_parent; }
        else { working = working->m_children[where]; }
      }
      else
      {
        assert (current.size () == 4);
        // No need to do anything here.
      }
    }
    else
    {
      if (current.size () >= 3 && current.substr (0, 3) == "dir")
      {
        assert (current.size () >= 5);
        assert (current[3] == ' ');
        std::string name = current.substr (4);
        assert (working->m_children.count (name) == 0);
        assert (working->m_files.count (name) == 0);
        working->m_children[name] = new Dir (name, working);
      }
      else
      {
        std::size_t split = current.find (' ');
        assert (split != std::string::npos);
        unsigned long size = atoi (current.substr (0, split).c_str ());
        assert (size > 0);
        std::string name = current.substr (split + 1);
        assert (working->m_children.count (name) == 0);
        assert (working->m_files.count (name) == 0);
        working->m_files[name] = new File (name, size);
      }
    }
  }
  return root;
}

unsigned long
part2 (Dir* root)
{
  unsigned long available = 70000000 - root->getTotalSize ();
  unsigned long needed = 30000000 - available;
  return root->smallestSufficient (needed);
}


/// \brief Runs the program.
/// \return Always 0.
int main () {
  Dir* root = parseInput ();
  std::cout << root->m_name << "\n";
  std::cout << root->addTotalIfAtMost (100000) << "\n";
  std::cout << part2 (root) << "\n";
  return 0;
}
