"""Chad Hogg's solution to Advent Of Code 2023, day 23

"""

import sys
import re
import time
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

def successors(problem: list[list[str]], path: Path, icy: bool) -> list[Path]:
    LAST_CHOICE: tuple[int, int] = (133, 128)
    potential: list[Path] = []
    # Safe to force a move to end here, and can save a lot of fruitless exploration
    if path.recentLoc == LAST_CHOICE:
        potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0] + 1, path.recentLoc[1])))
    # Can't take that shortcut, need to actually consider other directions                         
    else:
        # Moving N / S / W / E is allowed from where you are
        if not icy or problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == NORTH_SLOPE:
            potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0] - 1, path.recentLoc[1])))
        if not icy or problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == SOUTH_SLOPE:
            potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0] + 1, path.recentLoc[1])))
        if not icy or problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == WEST_SLOPE:
            potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0], path.recentLoc[1] - 1)))
        if not icy or problem[path.recentLoc[0]][path.recentLoc[1]] == PATH or problem[path.recentLoc[0]][path.recentLoc[1]] == EAST_SLOPE:
            potential.append(Path(frozenset(path.pastLocs.union({path.recentLoc})), (path.recentLoc[0], path.recentLoc[1] + 1)))
    results: list[Path] = []
    for path in potential:
        # The space you would move into exists
        if path.recentLoc[0] >= 0 and path.recentLoc[0] < len(problem) and path.recentLoc[1] >= 0 and path.recentLoc[1] < len(problem[0]):
            # The space you would move into is standable
            if problem[path.recentLoc[0]][path.recentLoc[1]] != FOREST:
                # The space you would move into has not yet been visited on this path
                if path.recentLoc not in path.pastLocs:
                    results.append(path)
    return results

def solve(problem: list[list[str]], icy: bool) -> int:
    START: tuple[int, int] = (0, 1)
    GOAL: tuple[int, int] = (len(problem) - 1, len(problem[0]) - 2)
    longest: Path = None
    frontier: list[Path] = [Path(frozenset(), START)]
    while frontier:
        current: Path = frontier.pop()
        for succ in successors(problem, current, icy):
            if succ.recentLoc == GOAL:
                if longest is None or len(succ.pastLocs) > len(longest.pastLocs):
                    longest = succ
            else:
                frontier.append(succ)
    return len(longest.pastLocs)

@dataclass
class CompressedProblem:
    adjLists: dict[tuple[int, int], list[tuple[int, int, int]]]
    start: tuple[int, int]
    end: tuple[int, int]
    extraSteps: int

def findNextIntersections(problem: list[list[str]], path: Path) -> list[tuple[int, int, int]]:
    result: list[tuple[int, int, int]] = []
    frontier: list[Path] = [path]
    while frontier:
        current: Path = frontier.pop()
        neighbors: list[Path] = successors(problem, current, False)
        if len(neighbors) > 1 and current.recentLoc != path.recentLoc:
            result.append((current.recentLoc[0], current.recentLoc[1], len(current.pastLocs)))
        else:
            for neighbor in neighbors:
                frontier.append(neighbor)
    return result

def buildCompressedProblem(problem: list[list[str]]) -> CompressedProblem:
    firstIntersection: tuple[int, int, int] = findNextIntersections(problem, Path(frozenset(), (0, 1)))[0]
    lastIntersection: tuple[int, int, int] = findNextIntersections(problem, Path(frozenset(), (len(problem) - 1, len(problem[0]) - 2)))[0]
    intersections: dict[tuple[int, int], list[tuple[int, int, int]]] = {lastIntersection[0:2]: []}
    frontier: set[tuple[int, int]] = {firstIntersection[0:2]}
    while frontier:
        current = frontier.pop()
        neighbors = findNextIntersections(problem, Path(frozenset(), current))
        assert current not in intersections
        intersections[current] = []
        for (y, x, dist) in neighbors:
            intersections[current].append((y, x, dist))
            if (y, x) not in frontier and (y, x) not in intersections:
                frontier.add((y, x))
    return CompressedProblem(intersections, firstIntersection[0:2], lastIntersection[0:2], firstIntersection[2] + lastIntersection[2])

@dataclass
class PathWithCost:
    path: Path
    cost: int

def solve2(problem: CompressedProblem) -> int:
    longest: PathWithCost = None
    frontier: list[PathWithCost] = [PathWithCost(Path(frozenset(), problem.start), 0)]
    while frontier:
        current = frontier.pop()
        if current.path.recentLoc == problem.end:
            if longest is None or current.cost > longest.cost:
                longest = current
        else:
            for (y, x, cost) in problem.adjLists[current.path.recentLoc]:
                if (y, x) not in current.path.pastLocs:
                    frontier.append(PathWithCost(Path(frozenset(current.path.pastLocs.union({current.path.recentLoc})), (y, x)), current.cost + cost))
    return longest.cost + problem.extraSteps

def main():
    problem: list[list[str]] = readInput()
    # print(problem)
    print(solve(problem, True))
    # print(solve(problem, False))
    # Welp, that works for the sample input, takes at least 12 hours on my input
    print(solve2(buildCompressedProblem(problem)))

main()
