"""Chad Hogg's solution to Advent Of Code 2023, day 14

"""

import sys

ROUND = 'O'
CUBE = '#'
EMPTY = '.'

def readInput() -> list[list[str]]:
    lines = sys.stdin.readlines()
    return [list(line.strip()) for line in lines]

def visualize(pattern: list[list[str]]):
    for line in pattern:
        print(''.join(line))

def tiltNorth(map: list[list[str]]) -> list[list[str]]:
    result = map
    for row in range(0, len(result)):
        for col in range(0, len(result[row])):
            if result[row][col] == ROUND:
                next = row - 1
                while next >= 0 and result[next][col] == EMPTY:
                    result[next][col] = ROUND
                    result[next + 1][col] = EMPTY
                    next -= 1
    return result

def tiltWest(map: list[list[str]]) -> list[list[str]]:
    result = map
    for col in range(0, len(result[0])):
        for row in range(0, len(result)):
            if result[row][col] == ROUND:
                next = col - 1
                while next >= 0 and result[row][next] == EMPTY:
                    result[row][next] = ROUND
                    result[row][next + 1] = EMPTY
                    next -= 1
    return result
    
def tiltSouth(map: list[list[str]]) -> list[list[str]]:
    result = map
    for row in range(len(result) - 1, -1, -1):
        for col in range(0, len(result[row])):
            if result[row][col] == ROUND:
                next = row + 1
                while next < len(result) and result[next][col] == EMPTY:
                    result[next][col] = ROUND
                    result[next - 1][col] = EMPTY
                    next += 1
    return result

def tiltEast(map: list[list[str]]) -> list[list[str]]:
    result = map
    for col in range(len(result[0]) - 1, -1, -1):
        for row in range(0, len(result)):
            if result[row][col] == ROUND:
                next = col + 1
                while next < len(result[0]) and result[row][next] == EMPTY:
                    result[row][next] = ROUND
                    result[row][next - 1] = EMPTY
                    next += 1
    return result


def totalLoad(map: list[list[str]]) -> int:
    sum = 0
    for row in range(0, len(map)):
        for col in range(0, len(map[row])):
            if map[row][col] == ROUND:
                sum += len(map) - row
    return sum

def doCycle(map: list[list[str]]) -> list[list[str]]:
    return tiltEast(tiltSouth(tiltWest(tiltNorth(map))))

def doLotsOfCycles(map: list[list[str]]) -> list[list[str]]:
    TODO = 1000000000
    previousResults = {}
    result = map
    cyclesDone = 0
    while cyclesDone < TODO:
        tuple_version = tuple(tuple(x) for x in result)
        if tuple_version in previousResults:
            loopLength = cyclesDone - previousResults[tuple_version]
            completeCycles = int((TODO - cyclesDone) / loopLength)
            cyclesDone += (loopLength * completeCycles) + 1
            result = doCycle(result)
        else:
            result = doCycle(result)
            previousResults[tuple_version] = cyclesDone
            cyclesDone += 1
    return result

def main():
    map = readInput()
    print(totalLoad(tiltNorth(map)))
    print(totalLoad(doLotsOfCycles(map)))

main()
