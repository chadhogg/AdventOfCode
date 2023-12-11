"""Chad Hogg's solution to Advent Of Code 2023, day 11

"""

import sys


def readInput() -> list[list[str]]:
    lines = sys.stdin.readlines()
    return [list(l.strip()) for l in lines]

rowsCache = {}
def isRowBlank(universe: list[list[str]], row: int) -> bool:
    if not row in rowsCache:
        rowsCache[row] = (len(list(filter(lambda v: v != '.', universe[row]))) == 0)
    return rowsCache[row]

colsCache = {}
def isColBlank(universe: list[list[str]], col: int) -> bool:
    if not col in colsCache:
        colsCache[col] = (len(list(filter(lambda v: v != '.', [universe[x][col] for x in range(0, len(universe))]))) == 0)
    return colsCache[col]

def numBlankRowsBetween(universe: list[list[str]], startRow: int, stopRow: int) -> int:
    count = 0
    for row in range(min(startRow, stopRow), max(startRow, stopRow)):
        if isRowBlank(universe, row):
            count += 1
    return count

def numBlankColsBetween(universe: list[list[str]], startCol: int, stopCol: int) -> int:
    count = 0
    for col in range(min(startCol, stopCol), max(startCol, stopCol)):
        if isColBlank(universe, col):
            count += 1
    return count

def findGalaxies(universe: list[list[str]]) -> list[(int, int)]:
    galaxies = []
    for row in range(0, len(universe)):
        for col in range(0, len(universe[0])):
            if(universe[row][col] == '#'):
                galaxies.append((row, col))
    return galaxies

def sumOfShortestPaths(universe: list[list[str]], galaxies: list[(int, int)], multiplier: int) -> int:
    sum = 0
    for i in range(0, len(galaxies)):
        for j in range(i + 1, len(galaxies)):
            dist = abs(galaxies[i][0] - galaxies[j][0]) + abs(galaxies[i][1] - galaxies[j][1])
            dist += (multiplier - 1) * numBlankRowsBetween(universe, galaxies[i][0], galaxies[j][0])
            dist += (multiplier - 1) * numBlankColsBetween(universe, galaxies[i][1], galaxies[j][1])
            sum += dist
    return sum

def solve(universe: list[list[str]], multiplier: int) -> int:
    return sumOfShortestPaths(universe, findGalaxies(universe), multiplier)

def main():
    universe = readInput()
    print(solve(universe, 2))
    print(solve(universe, 1000000))

main()
