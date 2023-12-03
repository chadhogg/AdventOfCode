"""Chad Hogg's solution to Advent Of Code 2023, day 3

"""

import sys
import re

def readInput() -> []:
    lines = sys.stdin.readlines()
    return lines

def isSymbol(s: str) -> bool:
    return not s.isalnum() and not s == '.' and not s.isspace()

def touchesSymbol(lines: [], i: int, m: re.Match) -> bool:
    for row in range (i - 1, i + 2):
        for col in range (m.start() - 1, m.end() + 1):
            if row >= 0 and row < len(lines) and col >= 0 and col < len(lines[row]):
                if isSymbol(lines[row][col]): return True
    return False

def findPartNumbers(lines: []) -> []:
    partNumbers = []
    for i in range(0, len(lines)):
        matches = re.finditer(r'\d+', lines[i])
        for match in matches:
            if touchesSymbol(lines, i, match):
                partNumbers.append({"row": i, "match": match})
    return partNumbers

def part1(lines: []) -> int:
    sum = 0
    for partNum in findPartNumbers(lines):
        sum += int(partNum['match'].group(0))
    return sum

def isAdjacent(partNum: {}, gear: {}) -> bool:
    if partNum['row'] >= gear['row'] - 1 and partNum['row'] <= gear['row'] + 1:
        if gear['match'].start() >= partNum['match'].start() - 1 and gear['match'].end() <= partNum['match'].end() + 1:
            return True
    return False
    

def part2(lines: []) -> int:
    sum = 0
    partNums = findPartNumbers(lines)
    for i in range(0, len(lines)):
        matches = re.finditer(r'\*', lines[i])
        for match in matches:
            adjacent = []
            for partNum in partNums:
                if isAdjacent (partNum, {'row': i, 'match': match}):
                    adjacent.append (partNum)
            if len(adjacent) == 2:
                sum += int(adjacent[0]['match'].group(0)) * int(adjacent[1]['match'].group(0))
    return sum


def main():
    lines = readInput()
    print(part1(lines))
    print(part2(lines))

main()
