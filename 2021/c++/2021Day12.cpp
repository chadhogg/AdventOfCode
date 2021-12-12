/// \file 2021Day12.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/12.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <algorithm>

#include "utilities.hpp"

/// The name of a graph vertex.
using Cave = std::string;
/// An undirected edge from one cave to another.
using Edge = std::pair<Cave, Cave>;
/// A collection of edges.
using EdgeList = std::vector<Edge>;
/// A path within the graph.
using Path = std::vector<std::string>;

/// The vertex from which all paths must start.
const Cave START = "start";
/// The vertex to which all paths must reach.
const Cave END = "end";

/// \brief Tests whether or not a vertex represents a small cave.
/// \param[in] cave The vertex.
/// \return True if it is a small cave.
inline bool isSmallCave (Cave const& cave) {
    return std::islower (cave.at (0));
}

/// \brief Tests whether or not a path has visited a vertex.
/// \param[in] path THe path.
/// \param[in] cave The vertex.
/// \return True if that vertex has been visited in that path.
bool contains (Path const& path, Cave const& cave) {
    for (auto it = path.cbegin (); it != path.cend (); ++it) {
        if (*it == cave) {
            return true;
        }
    }
    return false;
}

/// \brief Tests whether or not a path contains a duplicated small cave.
/// \param[in] path A path.
/// \return True if that path contains a duplicated small cave.
bool hasDuplicatedSmall (Path const& path) {
    for (auto it = path.cbegin (); it != path.end (); ++it) {
        if (isSmallCave (*it)) {
            for (auto it2 = it + 1; it2 != path.end (); ++it2) {
                if (*it == *it2) {
                    return true;
                }
            }
        }
    }
    return false;
}

/// \brief Reads a collection of edges from the user.
/// \return A collection of all edges.
EdgeList getInput () {
    EdgeList edges;
    std::string line;
    while (std::cin >> line) {
        int index = line.find ('-');
        edges.push_back ({line.substr (0, index), line.substr (index + 1)});
    }
    return edges;
}

/// \brief Counts the number of paths that never visit a small cave twice.
/// \param[in] edges A collection of edges.
/// \return The number of legal paths from START to END.
unsigned long findPaths (EdgeList const& edges) {
    std::vector<Path> paths;
    unsigned long count {0U};
    paths.push_back ({START});
    while (!paths.empty ()) {
        Path pre = paths.back ();
        paths.pop_back ();
        if (pre.back () == END) {
            ++count;
        }
        else {
            for (Edge const& edge : edges) {
                if (pre.back () == edge.first) {
                    if (!isSmallCave (edge.second) || !contains (pre, edge.second)) {
                        Path post {pre};
                        post.push_back (edge.second);
                        paths.push_back (post);
                    }
                }
                else if (pre.back () == edge.second) {
                    if (!isSmallCave (edge.first) || !contains (pre, edge.first)) {
                        Path post {pre};
                        post.push_back (edge.first);
                        paths.push_back (post);
                    }
                }
            }
        }
    }
    return count;
}

/// \brief Counts the number of paths that visit at most one small cave twice.
/// \param[in] edges A collection of edges.
/// \return The number of legal paths from START to END.
unsigned long findPaths2 (EdgeList const& edges) {
    std::vector<Path> paths;
    unsigned long count {0U};
    paths.push_back ({START});
    while (!paths.empty ()) {
        Path pre = paths.back ();
        paths.pop_back ();
        if (pre.back () == END) {
            ++count;
        }
        else {
            for (Edge const& edge : edges) {
                if (pre.back () == edge.first && edge.second != START) {
                    if (!isSmallCave (edge.second) || !contains (pre, edge.second) || !hasDuplicatedSmall (pre)) {
                        Path post {pre};
                        post.push_back (edge.second);
                        paths.push_back (post);
                    }
                }
                else if (pre.back () == edge.second && edge.first != START) {
                    if (!isSmallCave (edge.first) || !contains (pre, edge.first) || !hasDuplicatedSmall (pre)) {
                        Path post {pre};
                        post.push_back (edge.first);
                        paths.push_back (post);
                    }
                }
            }
        }
    }
    return count;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    EdgeList edges = getInput ();
    std::cout << findPaths (edges) << "\n";
    std::cout << findPaths2 (edges) << "\n";
    return 0;
}