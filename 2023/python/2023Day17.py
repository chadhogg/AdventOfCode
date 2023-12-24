"""Chad Hogg's solution to Advent Of Code 2023, day 17

"""

import sys
from dataclasses import dataclass
import heapq

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3

def turnLeft(dir: int) -> int:
    return (dir + 3) % 4

def turnRight(dir: int) -> int:
    return (dir + 1) % 4

@dataclass(frozen=True, order=True)
class Location:
    row: int
    col: int

@dataclass(frozen=True, order=True)
class PathStep:
    heatLoss: int
    location: Location
    dir: int

def readInput() -> list[list[int]]:
    lines = sys.stdin.readlines()
    result = [[int(y) for y in x.strip()] for x in lines]
    return result

def isValid(grid: list[list[int]], location: Location) -> bool:
    return location.row >= 0 and location.col >= 0 and location.row < len(grid) and location.col < len(grid[0])

def successors(grid: list[list[str]], pathStep: PathStep, minMoves: int, maxMoves: int) -> set[PathStep]:
    current = pathStep
    for distance in range(1, maxMoves + 1):
        next = current.location
        if current.dir == UP:
            next = Location(next.row - 1, next.col)
        elif current.dir == RIGHT:
            next = Location(next.row, next.col + 1)
        elif current.dir == DOWN:
            next = Location(next.row + 1, next.col)
        else:
            assert current.dir == LEFT
            next = Location(next.row, next.col - 1)
        if isValid(grid, next):
            if distance >= minMoves:
                yield PathStep(current.heatLoss + grid[next.row][next.col], next, turnRight(current.dir))
                yield PathStep(current.heatLoss + grid[next.row][next.col], next, turnLeft(current.dir))
            current = PathStep(current.heatLoss + grid[next.row][next.col], next, current.dir)
        else:
            break

def minimizeHeatLoss(grid: list[list[int]], start: set[PathStep], end: Location, minMoves: int, maxMoves: int) -> PathStep:
    frontier = []
    costTo = {}
    for s in start:
        heapq.heappush(frontier, s)
    while len(frontier) > 0:
        current = heapq.heappop(frontier)
        if (current.location, current.dir) in costTo:
            assert current.heatLoss >= costTo[(current.location, current.dir)]
            continue
        costTo[(current.location, current.dir)] = current.heatLoss
        if current.location == end:
            return current
        for s in successors(grid, current, minMoves, maxMoves):
            if (s.location, s.dir) not in costTo:
                heapq.heappush(frontier, s)
            else:
                assert s.heatLoss >= costTo[(s.location, s.dir)]
    return None

def part1(grid: list[list[str]]) -> int:
    return minimizeHeatLoss(grid, {PathStep(0, Location(0, 0), RIGHT), PathStep(0, Location(0, 0), DOWN)}, Location(len(grid) - 1, len(grid[0]) - 1)).heatLoss

def main():
    grid = readInput()
    print(minimizeHeatLoss(grid, {PathStep(0, Location(0, 0), RIGHT), PathStep(0, Location(0, 0), DOWN)}, Location(len(grid) - 1, len(grid[0]) - 1), 1, 3).heatLoss)
    print(minimizeHeatLoss(grid, {PathStep(0, Location(0, 0), RIGHT), PathStep(0, Location(0, 0), DOWN)}, Location(len(grid) - 1, len(grid[0]) - 1), 4, 10).heatLoss)

main()
