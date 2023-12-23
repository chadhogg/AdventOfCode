"""Chad Hogg's solution to Advent Of Code 2023, day 16

"""

import sys
from dataclasses import dataclass

EMPTY = '.'
FMIRROR = '/'
BMIRROR = '\\'
VSPLIT = '|'
HSPLIT = '-'

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3

@dataclass(frozen=True)
class Beam:
    row: int
    col: int
    dir: int

def readInput() -> list[list[str]]:
    lines = sys.stdin.readlines()
    result = [list(x.strip()) for x in lines]
    for line in result:
        for char in line:
            assert char == EMPTY or char == FMIRROR or char == BMIRROR or char == VSPLIT or char == HSPLIT
    return result

def isValid(grid: list[list[str]], beam: Beam) -> bool:
    return beam.row >= 0 and beam.col >= 0 and beam.row < len(grid) and beam.col < len(grid[0])

def successors(grid: list[list[str]], beam: Beam) -> list[Beam]:
    newLoc = None
    if beam.dir == UP:
        newLoc = Beam(beam.row - 1, beam.col, beam.dir)
    elif beam.dir == RIGHT:
        newLoc = Beam(beam.row, beam.col + 1, beam.dir)
    elif beam.dir == DOWN:
        newLoc = Beam(beam.row + 1, beam.col, beam.dir)
    else:
        assert beam.dir == LEFT
        newLoc = Beam(beam.row, beam.col - 1, beam.dir)
    if isValid(grid, newLoc):
        if grid[newLoc.row][newLoc.col] == EMPTY:
            return [newLoc]
        elif grid[newLoc.row][newLoc.col] == FMIRROR:
            if newLoc.dir == UP:
                return [Beam(newLoc.row, newLoc.col, RIGHT)]
            elif newLoc.dir == RIGHT:
                return [Beam(newLoc.row, newLoc.col, UP)]
            elif newLoc.dir == DOWN:
                return [Beam(newLoc.row, newLoc.col, LEFT)]
            else:
                assert newLoc.dir == LEFT
                return [Beam(newLoc.row, newLoc.col, DOWN)]
        elif grid[newLoc.row][newLoc.col] == BMIRROR:
            if newLoc.dir == UP:
                return [Beam(newLoc.row, newLoc.col, LEFT)]
            elif newLoc.dir == RIGHT:
                return [Beam(newLoc.row, newLoc.col, DOWN)]
            elif newLoc.dir == DOWN:
                return [Beam(newLoc.row, newLoc.col, RIGHT)]
            else:
                assert newLoc.dir == LEFT
                return [Beam(newLoc.row, newLoc.col, UP)]
        elif grid[newLoc.row][newLoc.col] == HSPLIT:
            if newLoc.dir == RIGHT or newLoc.dir == LEFT:
                return [newLoc]
            else:
                return [Beam(newLoc.row, newLoc.col, RIGHT), Beam(newLoc.row, newLoc.col, LEFT)]
        else:
            assert grid[newLoc.row][newLoc.col] == VSPLIT
            if newLoc.dir == UP or newLoc.dir == DOWN:
                return [newLoc]
            else:
                return [Beam(newLoc.row, newLoc.col, UP), Beam(newLoc.row, newLoc.col, DOWN)]
    else:
        return []
    
def propagate(grid: list[list[str]], start: Beam) -> set[Beam]:
    frontier = set()
    beams = set()
    frontier.add(start)
    while len(frontier) > 0:
        beam = frontier.pop()
        next = successors(grid, beam)
#        print('%s lead to %s' % (beam, next))
        for b in next:
            if not b in beams:
                beams.add(b)
                frontier.add(b)
    return beams

def energized(beams: set[Beam]) -> int:
    return len({(b.row, b.col) for b in beams})

def part1(grid: list[list[str]]) -> int:
    beams = propagate(grid, Beam(0, -1, RIGHT))
    return energized(beams)

def part2(grid: list[list[str]]) -> int:
    best = 0
    toTry = set()
    for col in range(0, len(grid[0])):
        toTry.add(Beam(-1, col, DOWN))
        toTry.add(Beam(len(grid), col, UP))
    for row in range(0, len(grid)):
        toTry.add(Beam(row, -1, RIGHT))
        toTry.add(Beam(row, len(grid[0]), DOWN))
    for beam in toTry:
        best = max(best, energized(propagate(grid, beam)))
    return best

def main():
    grid = readInput()
    print(part1(grid))
    print(part2(grid))

main()
