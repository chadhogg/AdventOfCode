"""Chad Hogg's solution to Advent Of Code 2023, day 24

"""

import sys
import re
import math
from dataclasses import dataclass

@dataclass(frozen=True)
class Vec3D:
    x: int
    y: int
    z: int

@dataclass(frozen=True)
class Hailstone:
    p0: Vec3D
    v: Vec3D

def readInput() -> list[Hailstone]:
    lines: list[str] = sys.stdin.readlines()
    stones: list[Hailstone] = []
    for line in lines:
        match = re.match(r'(-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+)', line.strip())
        g = match.groups()
        stones.append(Hailstone(Vec3D(int(g[0]), int(g[1]), int(g[2])), Vec3D(int(g[3]), int(g[4]), int(g[5]))))
    return stones

def intersect2D(h1: Hailstone, h2: Hailstone) -> tuple[float, float]:
    h1Start = (h1.p0.x, h1.p0.y)
    h1Next = (h1.p0.x + h1.v.x, h1.p0.y + h1.v.y)
    h2Start = (h2.p0.x, h2.p0.y)
    h2Next = (h2.p0.x + h2.v.x, h2.p0.y + h2.v.y)
    h1Slope: float = h1.v.y / h1.v.x
    h2Slope: float = h2.v.y / h2.v.x
    # Parallel rays cannot intersect anywhere
    if math.isclose(h1Slope, h2Slope): return None
    h1Intercept: float = h1.p0.y - h1Slope * h1.p0.x
    h2Intercept: float = h2.p0.y - h2Slope * h2.p0.x
    # h1Slope * x + h1Intercept = h2Slope * x + h2Intercept
    # -> (h1Slope - h2Slope) * x = h2Intercept - h1Intercept
    # -> x = (h2Intercept - h1Intercept) / (h1Slope - h2Slope)
    x: float = (h2Intercept - h1Intercept) / (h1Slope - h2Slope)
    y: float = h1Slope * x + h1Intercept
    assert math.isclose(y, h2Slope * x + h2Intercept)
    return (x, y)

def isInFuture(stone: Hailstone, point: tuple[float, float]) -> bool:
    # Apparently this doesn't work
    # change: float = (point[1] - stone.p0.y) / (point[0] - stone.p0.x)
    # # True if both have the same sign
    # return change * (stone.v.y / stone.v.x) > 0
    if stone.v.x > 0:
        return point[0] > stone.p0.x
    else:
        return point[0] < stone.p0.x

def part1(problem: list[Hailstone], minX: int, maxX: int, minY: int, maxY: int) -> int:
    count: int = 0
    for i in range(0, len(problem)):
        for j in range(i + 1, len(problem)):
            result = intersect2D(problem[i], problem[j])
            # An intersection exists
            if result is not None:
                # That intersection occurs in the future
                if isInFuture(problem[i], result) and isInFuture(problem[j], result):
                    # That intersection occurs in the test area
                    if result[0] >= minX and result[0] <= maxX and result[1] >= minY and result[1] <= maxY:
                        count += 1
    return count

def main():
    problem: list[Hailstone] = readInput()
    # print(str(problem))
    # print(part1(problem, 7, 27, 7, 27))
    print(part1(problem, 200000000000000, 400000000000000, 200000000000000, 400000000000000))

main()
