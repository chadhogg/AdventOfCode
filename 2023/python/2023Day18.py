"""Chad Hogg's solution to Advent Of Code 2023, day 18

"""

import sys
import re
from dataclasses import dataclass

from ListOfIntRanges import IntRange, IntRangeList, Compressed2DBooleanList

RIGHT = 'R'
DOWN = 'D'
LEFT = 'L'
UP = 'U'

GROUND_LEVEL = '.'
TRENCH = '#'

@dataclass
class Instruction:
    direction: str
    length: int
    color: str

def readInput() -> list[Instruction]:
    lines = sys.stdin.readlines()
    insts = []
    for line in lines:
        match = re.match(r'([R|D|L|U]) (\d+) \(#(\w\w\w\w\w\w)\)', line.strip())
        if match:
            insts.append(Instruction(match.groups()[0], int(match.groups()[1]), match.groups()[2]))
        else:
            print(line + ' doesn\'t match pattern')
    return insts
    

def visualize(pattern: list[list[str]]):
    for line in pattern:
        print(''.join(line))

def visualize2(sparse: Compressed2DBooleanList, edges: tuple[int, int, int, int]):
    for index in range(sparse.rows - 1, -1, -1):
        line = sparse.getLine(index)
        output = ''
        for x in range(0, edges[3] - edges[2] + 3):
            if line.contains(x):
                output += '#'
            else:
                output += '.'
        print(output)

def findEdges(insts: list[Instruction]) -> tuple[int, int, int, int]:
    minY, maxY, minX, maxX = 0, 0, 0, 0
    current = (0, 0)
    for inst in insts:
        if inst.direction == RIGHT: current = (current[0], current[1] + inst.length)
        elif inst.direction == LEFT: current = (current[0], current[1] - inst.length)
        elif inst.direction == UP: current = (current[0] + inst.length, current[1])
        else: current = (current[0] - inst.length, current[1])
        minX = min(minX, current[1])
        maxX = max(maxX, current[1])
        minY = min(minY, current[0])
        maxY = max(maxY, current[0])
    return (minY, maxY, minX, maxX)

def buildImage(insts: list[Instruction], edges: tuple[int, int, int, int]) -> list[list[str]]:
    grid = []
    for y in range(edges[0], edges[1] + 3):
        grid.append(['.'] * (edges[3] - edges[2] + 3))
    current = (-edges[0] + 1, -edges[2] + 1)
    grid[current[0]][current[1]] = TRENCH
    for inst in insts:
        if inst.direction == RIGHT:
            for i in range(0, inst.length):
                current = (current[0], current[1] + 1)
                grid[current[0]][current[1]] = TRENCH
        elif inst.direction == LEFT:
            for i in range(0, inst.length):
                current = (current[0], current[1] - 1)
                grid[current[0]][current[1]] = TRENCH
        elif inst.direction == UP:
            for i in range(0, inst.length):
                current = (current[0] + 1, current[1])
                grid[current[0]][current[1]] = TRENCH
        else:
            for i in range(0, inst.length):
                current = (current[0] - 1, current[1])
                grid[current[0]][current[1]] = TRENCH

    return grid

def countInside(grid: list[list[str]]) -> int:
    copy = [r[:] for r in grid]
    FILLED = '@'
    todo = set()
    for y in range(0, len(copy)):
        for x in range(0, len(copy[0])):
            if (y == 0 or y == len(copy) - 1 or x == 0 or x == len(copy[0]) - 1):
                copy[y][x] = FILLED
                todo.add((y, x))
    while(len(todo) > 0):
        y, x = todo.pop()
        candidates = [(y - 1, x), (y + 1, x), (y, x - 1), (y, x + 1)]
        for y2, x2 in candidates:
            if y2 >= 0 and y2 <= len(copy) - 1 and x2 >= 0 and x2 <= len(copy[0]) - 1:
                if copy[y2][x2] == GROUND_LEVEL:
                    copy[y2][x2] = FILLED
                    todo.add((y2, x2))
    sum = 0
    for line in copy:
        for char in line:
            if char != FILLED: sum += 1
    return sum

def decodeInstructions(old: list[Instruction]) -> list[Instruction]:
    results = []
    for inst in old:
        dist = int(inst.color[0:5], 16)
        dir = RIGHT if inst.color[5] == '0' else DOWN if inst.color[5] == '1' else LEFT if inst.color[5] == '2' else UP
        results.append(Instruction(dir, dist, ''))
    return results

def buildSparseMatrix(insts: list[Instruction], edges: tuple[int, int, int, int]) -> Compressed2DBooleanList:
    sparseGrid = Compressed2DBooleanList(edges[1] - edges[0] + 3)
    current = (-edges[0] + 1, -edges[2] + 1)
    for inst in insts:
        if inst.direction == RIGHT:
            sparseGrid.setHorizontal(current[0], current[1], current[1] + inst.length)
            current = (current[0], current[1] + inst.length)
        elif inst.direction == LEFT:
            sparseGrid.setHorizontal(current[0], current[1] - inst.length, current[1])
            current = (current[0], current[1] - inst.length)
        elif inst.direction == UP:
            sparseGrid.setVertical(current[0], current[0] + inst.length, current[1])
            current = (current[0] + inst.length, current[1])
        else:
            sparseGrid.setVertical(current[0] - inst.length, current[0], current[1])
            current = (current[0] - inst.length, current[1])
        sparseGrid.compress()
    return sparseGrid

def countInsideOfSparseGrid(sparseGrid: Compressed2DBooleanList) -> int:
    # @TODO This takes 12 minutse to run on my input, and there's no reason to be getting
    #       every line individually.  But I'm not motivated to improve it at the moment.
    total = 0
    for y in range(sparseGrid.rows - 1, -1, -1):
        thisLine = sparseGrid.getLine(y)
        before = sparseGrid.getLine(y - 1) if len(thisLine.ranges) > 0 else None
        index = 0
        insideStarted = -1
        while index < len(thisLine.ranges):
            r = thisLine.ranges[index]
            startAbove = before.contains(r.low)
            endAbove = before.contains(r.high)
            if startAbove ^ endAbove or r.low == r.high:
                # The two ends go in different directions, so this is the start or end of an enclosed region
                if insideStarted == -1:
                    insideStarted = r.low
                else:
                    total += (r.high - insideStarted + 1)
                    insideStarted = -1
                index += 1
            else:
                # The two ends go in same direction, so this is a boundary
                if insideStarted == -1:
                    total += r.high - r.low + 1
                index += 1
        assert insideStarted == -1
    return total

def main():
    insts = readInput()
    edges = findEdges(insts)
    grid = buildImage(insts, edges)
    print(countInside(grid))
    decoded = decodeInstructions(insts)
    decEdges = findEdges(decoded)
    sparseGrid = buildSparseMatrix(decoded, decEdges)
    print(countInsideOfSparseGrid(sparseGrid))

main()
