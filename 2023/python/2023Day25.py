"""Chad Hogg's solution to Advent Of Code 2023, day 25

"""

import sys
import graphviz
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

def graphVizIt(problem: Graph):
    dot = graphviz.Graph('aoc2023day25')
    for x in problem.vertices:
        dot.node(x, x)
    for x in problem.vertices:
        for y in problem.vertices:
            if y in problem.adjacency[x] and x < y:
                dot.edge(x, y)
    # Note: This option is necessary to get the graph organized in a way that makes it easy to find
    dot.render(engine='neato')

def partition(problem: Graph, edgesToSkip: list[tuple[str, str]], vertex: str) -> int:
    finished: set[str] = set()
    frontier: set[str] = {vertex}
    while frontier:
        current: str = frontier.pop()
        for other in problem.adjacency[current]:
            if (current, other) not in edgesToSkip and (other, current) not in edgesToSkip:
                if other not in finished and other not in frontier:
                    frontier.add(other)
        finished.add(current)
    return len(finished) * (len(problem.vertices) - len(finished))

def main():
    problem: Graph = readInput()
    # print(str(problem))
    # graphVizIt(problem)
    # Vizualization reveals that the edges to choose are pxp-nqq, dct-kns, and ksq-jxb
    print(partition(problem, [('pxp', 'nqq'), ('dct', 'kns'), ('ksq', 'jxb')], 'pxp'))

main()
