"""Chad Hogg's solution to Advent Of Code 2023, day 23

"""

import sys
import re
from dataclasses import dataclass

PATH = '.'
FOREST = '#'
NORTH_SLOPE = '^'
EAST_SLOPE = '>'
SOUTH_SLOPE = 'v'
WEST_SLOPE = '<'

@dataclass(frozen=True)
class Path:
    pastLocs: frozenset[tuple[int, int]]
    recentLoc: tuple[int, int]

def readInput() -> list[list[str]]:
    lines = sys.stdin.readlines()
    return [list(x.strip()) for x in lines]

def successors(problem: list[list[str]], path: Path) -> list[Path]:
    potential: list[Path] = []
    # Moving N / S / W / E is allowed from where you are
    if problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == NORTH_SLOPE:
        potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0] - 1, path.recentLoc[1])))
    if problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == SOUTH_SLOPE:
        potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0] + 1, path.recentLoc[1])))
    if problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == WEST_SLOPE:
        potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0], path.recentLoc[1] - 1)))
    if problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == EAST_SLOPE:
        potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0], path.recentLoc[1] + 1)))
    results: list[Path] = []
    for path in potential:
        # The space you would move into is standable
        if problem[path.recentLoc[0]][path.recentLoc[1]] != FOREST:
            # The space you would move into has not yet been visited on this path
            if path.recentLoc not in path.pastLocs:
                results.append(path)
    return results

def part1(problem: list[list[str]]) -> int:
    START: tuple[int, int] = (0, 1)
    GOAL: tuple[int, int] = (len(problem) - 1, len(problem[0]) - 2)
    longest: Path = None
    frontier: list[Path] = [Path(frozenset(), START)]
    while frontier:
        current: Path = frontier.pop()
        for succ in successors(problem, current):
            if succ.recentLoc == GOAL:
                if longest is None or len(succ.pastLocs) > len(longest.pastLocs):
                    longest = succ
            else:
                frontier.append(succ)
    return len(longest.pastLocs)

def main():
    problem: list[list[str]] = readInput()
    # print(problem)
    print(part1(problem))

main()
