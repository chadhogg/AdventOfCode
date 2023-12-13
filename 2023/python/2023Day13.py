"""Chad Hogg's solution to Advent Of Code 2023, day 13

"""

import sys
def readInput() -> list[list[list[str]]]:
    lines = sys.stdin.readlines()
    patterns = []
    current = []
    for line in lines:
        if len(line.strip()) == 0:
            patterns.append(current)
            current = []
        else:
            current.append(list(line.strip()))
    if len(current) > 0:
        patterns.append(current)
    return patterns

def visualize(pattern: list[list[str]]):
    for line in pattern:
        print(''.join(line))

#afterLine is 0: check 0 vs 1
#afterLine is 1: check 1 vs 2, 0 vs 3
#afterLine is 2: check 2 vs 3, 1 vs 4, 0 vs 5

def rowsAboveHorizontalExcept(pattern: list[list[str]], noCount: int) -> int:
    for afterLine in range(0, len(pattern) - 1):
        reflection = True
        for offset in range(0, min(afterLine + 1, len(pattern) - afterLine - 1)):
            if pattern[afterLine - offset] != pattern[afterLine + offset + 1]:
                reflection = False
                break
        if reflection and (not noCount or noCount != afterLine + 1):
            return afterLine + 1
    return None

def part1(patterns: list[list[list[str]]]) -> int:
    result = 0
    for pattern in patterns:
        horiz = rowsAboveHorizontalExcept(pattern, None)
        if horiz:
            result += 100 * horiz
        else:
            result += rowsAboveHorizontalExcept(list(zip(*pattern)), None)
    return result

def tryAllCombos(pattern: list[list[list[str]]], forbidden: int) -> int:
    for row in range(0, len(pattern)):
        for col in range(0, len(pattern[row])):
            old = pattern[row][col]
            pattern[row][col] = '#' if old == '.' else '.'
            h = rowsAboveHorizontalExcept(pattern, forbidden)
            if h:
                return h
            pattern[row][col] = old
    return None


def part2(patterns: list[list[list[str]]]) -> int:
    result = 0
    for pattern in patterns:
        flipped = list(zip(*pattern))
        for index in range(0, len(flipped)):
            flipped[index] = list(flipped[index])
        horiz = rowsAboveHorizontalExcept(pattern, None)
        vert = rowsAboveHorizontalExcept(flipped, None)
        h = tryAllCombos(pattern, horiz)
        if h:
            result += h * 100
        else:
            v = tryAllCombos(flipped, vert)
            assert v
            result += v
    return result

def main():
    patterns = readInput()
    print(part1(patterns))
    print(part2(patterns))

main()
