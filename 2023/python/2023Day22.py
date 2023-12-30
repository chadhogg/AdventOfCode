"""Chad Hogg's solution to Advent Of Code 2023, day 22

"""

import sys
import re
import functools
from dataclasses import dataclass

# A started as (1,0,1):(1,2,1), becomes (1,0,1):(1,2,1)
# B started as (0,0,2):(2,0,2), becomes (0,0,2):(2,0,2) supported by A
# C started as (0,2,3):(2,2,3), becomes (0,2,2):(2,2,2) supported by A
# D started as (0,0,4):(0,2,4), becomes (0,0,3):(0,2,3) supported by B and C
# E started as (2,0,5):(2,2,5), becomes (2,0,3):(2,2,3) supported by B and C
# F started as (0,1,6):(2,1,6), becomes (0,1,4):(2,1,4) supported by D and E
# G started as (1,1,8):(1,1,9), becomes (1,1,5):(1,1,6) supported by F

# My code is settling them correctly
# My code is finding the supports for each block correctly

@dataclass(frozen=True)
class Point:
    x: int
    y: int
    z: int

    def __lt__(self, other):
        if self.z < other.z: return True
        elif self.z > other.z: return False
        elif self.x < other.x: return True
        elif self.x > other.x: return False
        elif self.y < other.y: return True
        else: return False

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

@dataclass(frozen=True,order=False)
class Brick:
    a: Point
    b: Point

    def __lt__(self, other):
        if self.a < other.a: return True
        elif self.a > other.a: return False
        else: return self.b < other.b

    def __eq__(self, other):
        return self.a == other.a and self.b == other.b
    
    def __gt__(self, other):
        return not self < other and not self == other


def readInput() -> list[Brick]:
    lines = sys.stdin.readlines()
    bricks = []
    for line in lines:
        match = re.match(r'(\d+),(\d+),(\d+)~(\d+),(\d+),(\d+)', line.strip())
        g = match.groups()
        a = Point(int(g[0]), int(g[1]), int(g[2]))
        b = Point(int(g[3]), int(g[4]), int(g[5]))
        assert (a.x == b.x and a.y == b.y) or (a.x == b.x and a.z == b.z) or (a.y == b.y and a.z == b.z)
        # print('%s -> %s' % (line.strip(), Brick(min(a, b), max(a, b))))
        bricks.append(Brick(min(a, b), max(a, b)))
    bricks.sort()
    return bricks

def findCovered(brick: Brick) -> list[tuple[int, int]]:
    covered = []
    # Brick is a single cube or extends in the z dimension
    if brick.a.x == brick.b.x and brick.a.y == brick.b.y:
        return [(brick.a.x, brick.a.y)]
    # Brick extends in the y dimension
    elif brick.a.x == brick.b.x and brick.a.z == brick.b.z:
        assert brick.a.y < brick.b.y
        return [(brick.a.x, y) for y in range(brick.a.y, brick.b.y + 1)]
    # Brick extends in the x dimension
    else:
        assert brick.a.y == brick.b.y and brick.a.z == brick.b.z
        assert brick.a.x < brick.b.x
        return [(x, brick.a.y) for x in range(brick.a.x, brick.b.x + 1)]

def settle(problem: list[Brick]) -> list[Brick]:
    heights: dict[tuple[int, int], int] = dict()
    bricks: list[Brick] = []
    for brick in problem:
        covered = findCovered(brick)
        newZ = max(heights[x] if x in heights else 0 for x in covered) + 1
        assert newZ <= brick.a.z
        bricks.append(Brick(Point(brick.a.x, brick.a.y, newZ), Point(brick.b.x, brick.b.y, newZ + brick.b.z - brick.a.z)))
        for x in covered:
            heights[x] = newZ + brick.b.z - brick.a.z
    bricks.sort()
    return bricks

# Maps each brick to the bricks that support it
def findSupports(bricks: list[Brick]) -> dict[Brick, set[Brick]]:
    result: dict[Brick, set[Brick]] = dict()
    for b1 in bricks:
        b1c = findCovered(b1)
        supports: set[Brick] = set()
        for b2 in bricks:
            if b1.a.z == b2.b.z + 1:
                b2c = findCovered(b2)
                for c1 in b1c:
                    for c2 in b2c:
                        if c1[0] == c2[0] and c1[1] == c2[1]:
                            supports.add(b2)
        result[b1] = supports
    return result

def canBeDisintegrated(supports: dict[Brick, set[Brick]], brick: Brick) -> bool:
    for b in supports:
        if len(supports[b]) == 1 and brick in supports[b]:
            return False
    return True

def part1(supports: dict[Brick, set[Brick]]) -> int:
    count = 0
    for b in supports:
        if canBeDisintegrated(supports, b):
            count += 1
    return count

def totalSupported(supports: dict[Brick, set[Brick]], moved: frozenset[Brick]) -> int:
    newFallers = set()
    for above in supports:
        # This block is supported by only blocks that have been disintegrated / fallen
        if above not in moved and supports[above] and supports[above].issubset(moved):
            newFallers.add(above)
    if newFallers:
        # Recurse on the bigger collection
        return totalSupported(supports, frozenset(newFallers.union(moved)))
    else:
        # One of the ones that moved was disintegrated; the others fell
        return len(moved) - 1

def part2(supports: dict[Brick, set[Brick]]) -> int:
    return sum([totalSupported(supports, {b}) for b in supports])

def main():
    problem = readInput()
    # print(problem)
    settled = settle(problem)
    # print(settled)
    supports = findSupports(settled)
    # print(supports)
    print(part1(supports))
    print(part2(supports))

main()
