"""Chad Hogg's solution to Advent Of Code 2023, day 10

"""

import sys


def readInput() -> []:
    lines = sys.stdin.readlines()
    return [line.strip() for line in lines]

def findS(lines: list[str]) -> tuple[int, int]:
    for r in range(0, len(lines)):
        for c in range(0, len(lines[r])):
            if lines[r][c] == 'S':
                return (r, c)
            
def isValid(lines: list[str], loc: tuple[int, int]) -> bool:
    return (loc[0] >= 0 and loc[0] < len(lines) and loc[1] >= 0 and loc[1] < len(lines[loc[0]]))

def get(lines: list[str], loc: tuple[int, int]) -> bool:
    assert(isValid(lines, loc))
    return lines[loc[0]][loc[1]]

def countLoopLength(lines: list[str]) -> int:
    distances = [[-1 for x in line] for line in lines]
    start = findS(lines)
    previous = start
    current = previous
    moves = 0
    distances[current[0]][current[1]] = moves
    if isValid(lines, (current[0] - 1, current[1])) and (lines[current[0] - 1][current[1]] == '|' or lines[current[0] - 1][current[1]] == 'F' or lines[current[0] - 1][current[1]] == '7'):
        current = (current[0] - 1, current[1])
    elif isValid(lines, (current[0] + 1, current[1])) and (lines[current[0] + 1][current[1]] == '|' or lines[current[0] + 1][current[1]] == 'J' or lines[current[0] + 1][current[1]] == 'L'):
        current = (current[0] + 1, current[1])
    else:
        assert(isValid(lines, (current[0], current[1] - 1)) and (lines[current[0]][current[1] - 1] == '-' or lines[current[0]][current[1] - 1] == 'F' or lines[current[0]][current[1] - 1] == 'L'))
        current = (current[0], current[1] - 1)
    moves += 1
    while current != start:
        distances[current[0]][current[1]] = moves
        after = (-1, -1)
        if get(lines, current) == '|':
            if previous == (current[0] + 1, current[1]):
                after = (current[0] - 1, current[1])
            else:
                assert(previous == (current[0] - 1, current[1]))
                after = (current[0] + 1, current[1])
        elif get(lines, current) == '-':
            if previous == (current[0], current[1] - 1):
                after = (current[0], current[1] + 1)
            else:
                assert(previous == (current[0], current[1] + 1))
                after = (current[0], current[1] - 1)
        elif get(lines, current) == 'L':
            if previous == (current[0] - 1, current[1]):
                after = (current[0], current[1] + 1)
            else:
                assert(previous == (current[0], current[1] + 1))
                after = (current[0] - 1, current[1])
        elif get(lines, current) == 'J':
            if previous == (current[0] - 1, current[1]):
                after = (current[0], current[1] - 1)
            else:
                assert(previous == (current[0], current[1] - 1))
                after = (current[0] - 1, current[1])
        elif get(lines, current) == 'F':
            if previous == (current[0] + 1, current[1]):
                after = (current[0], current[1] + 1)
            else:
                assert(previous == (current[0], current[1] + 1))
                after = (current[0] + 1, current[1])
        else:
            assert(get(lines, current) == '7')
            if previous == (current[0] + 1, current[1]):
                after = (current[0], current[1] - 1)
            else:
                assert(previous == (current[0], current[1] - 1))
                after = (current[0] + 1, current[1])
        distances[after[0]][after[1]] = moves
        moves += 1
        previous = current
        current = after
    return moves

def main():
    lines = readInput()
    print(int(countLoopLength(lines) / 2))

main()
