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
    return (moves, distances)

def visualize(lines: list[str]):
    for row in range(0, len(lines)):
        print(lines[row])
            

def expandLines(lines: list[str], distances: list[list[int]]) -> list[str]:
    expanded = [['.' for x in range(0, len(lines[0]) * 2 + 1)] for y in range(0, len(lines) * 2 + 1)]
    for row in range(0, len(lines)):
        for col in range(0, len(lines[row])):
            expanded[row * 2 + 1][col * 2 + 1] = lines[row][col]
    for row in range(1, len(expanded), 2):
        for col in range(2, len(expanded[row]) - 1, 2):
            left = ((row - 1) // 2, (col - 1) // 2)
            right = (row // 2, col // 2)
            if distances[left[0]][left[1]] != -1 and (get(lines, left) == '-' or get(lines, left) == 'F' or get(lines, left) == 'L'):
                expanded[row][col] = '-'
            if distances[right[0]][right[1]] != -1 and (get(lines, right) == '-' or get(lines, right) == 'J' or get(lines, right) == '7'):
                expanded[row][col] = '-'
    for row in range(2, len(expanded) - 1, 2):
        for col in range(1, len(expanded[row]), 2):
            up = ((row - 1) // 2, (col - 1) // 2)
            down = (row // 2, col // 2)
            if distances[up[0]][up[1]] != -1 and (get(lines, up) == '|' or get(lines, up) == 'F' or get(lines, up) == '7'):
                expanded[row][col] = '|'
            if distances[down[0]][down[1]] != -1 and (get(lines, down) == '|' or get(lines, down) == 'J' or get(lines, down) == 'L'):
                expanded[row][col] = '|'
    return [''.join(line) for line in expanded]

def fill(lines: list[str], distances: list[list[int]]) -> list[str]:
    chars = [list(line) for line in lines]
    for row in range(0, len(chars)):
        for col in range(0, len(chars[row])):
            if distances[row][col] != -1:
                chars[row][col] = '#'
    todo = {(0, c) for c in range(0, len(chars[0]))}
    todo |= {(len(chars) - 1, c) for c in range(0, len(chars[0]))}
    todo |= {(r, 0) for r in range(0, len(chars))}
    todo |= {(r, len(chars[0]) - 1) for r in range(0, len(chars))}
    while len(todo) > 0:
        current = todo.pop()
        neighbors = [(current[0] - 1, current[1]), (current[0] + 1, current[1]), (current[0], current[1] - 1), (current[0], current[1] + 1)]
        for n in neighbors:
            if isValid(chars, n) and chars[n[0]][n[1]] != '#' and chars[n[0]][n[1]] != 'O':
                todo.add(n)
        chars[current[0]][current[1]] = 'O'
    return [''.join(line) for line in chars]

def countInside(lines: list[str]) -> int:
    sum = 0
    for row in range(1, len(lines), 2):
        for col in range(1, len(lines[row]), 2):
            c = lines[row][col]
            if c != '#' and c != 'O':
                sum += 1
    return sum

def main():
    lines = readInput()
    moves, distances = countLoopLength(lines)
    print(moves // 2)
    expanded = expandLines(lines, distances)
    moves, distances = countLoopLength(expanded)
    print(countInside(fill(expanded, distances)))

main()
