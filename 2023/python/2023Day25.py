"""Chad Hogg's solution to Advent Of Code 2023, day 25

"""

import sys
import re
import math
from dataclasses import dataclass

@dataclass(frozen=True)
class Graph:
    vertices: set[str]
    adjacency: dict[str, set[str]]

def readInput() -> Graph:
    lines: list[str] = sys.stdin.readlines()
    vert: set[str] = set()
    adj: dict[str, set[str]] = dict()
    for line in lines:
        source, targets = line.strip().split(': ')
        targets = list(targets.split())
        vert.add(source)
        if source not in adj: adj[source] = set()
        for target in targets:
            vert.add(target)
            if target not in adj: adj[target] = set()
            adj[source].add(target)
            adj[target].add(source)
    return Graph(vert, adj)

def main():
    problem: Graph = readInput()
    print(str(problem))
    
main()
