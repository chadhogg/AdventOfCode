"""Chad Hogg's solution to Advent Of Code 2023, day 21

"""

import sys

GARDEN = '.'
ROCK = '#'
START = 'S'


def readInput() -> list[list[str]]:
    lines = sys.stdin.readlines()
    return [list(l.strip()) for l in lines]

def findPlotsReachableInNSteps(problem: list[list[str]], steps: int, infinite: bool) -> set[tuple[int, int]]:
    frontier: set[tuple[int, int]] = set()
    for row in range(0, len(problem)):
        for col in range(0, len(problem[row])):
            if problem[row][col] == START:
                frontier.add((row, col))
    while steps > 0:
        next: set[tuple[int, int]] = set()
        for old in frontier:
            adjacent = [(old[0] - 1, old[1]), (old[0] + 1, old[1]), (old[0], old[1] - 1), (old[0], old[1] + 1)]
            for new in adjacent:
                if infinite or (new[0] >= 0 and new[0] < len(problem) and new[1] >= 0 and new[1] < len(problem[0])):
                    if problem[new[0] % len(problem)][new[1] % len(problem[0])] != ROCK:
                        next.add(new)
        frontier = next
        steps -= 1
    return frontier

def main():
    problem = readInput()
    print(len(findPlotsReachableInNSteps(problem, 6, False)))
    print(len(findPlotsReachableInNSteps(problem, 100, True)))

main()
