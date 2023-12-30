"""Chad Hogg's solution to Advent Of Code 2023, day 24

"""

import sys
import re
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


def main():
    problem: list[Hailstone] = readInput()
    print(str(problem))

main()
