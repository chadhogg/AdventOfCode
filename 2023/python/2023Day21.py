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
    #print(len(findPlotsReachableInNSteps(problem, 64, False)))
    #print(len(findPlotsReachableInNSteps(problem, 100, True)))
#    print('%d: %d' % (len(problem), len(findPlotsReachableInNSteps(problem, len(problem), True))))
#    print('%d: %d' % (2 * len(problem), len(findPlotsReachableInNSteps(problem, 2 * len(problem), True))))
#    print('%d: %d' % (3 * len(problem), len(findPlotsReachableInNSteps(problem, 3 * len(problem), True))))
#    print('%d: %d' % (4 * len(problem), len(findPlotsReachableInNSteps(problem, 4 * len(problem), True))))
# regression suggests that 15397/17161 x^2 + 228/131 x + 1 fits these data points perfectly
#    print(15397 * 26501365 * 26501365 // 17161 + 228 * 26501365 // 131 + 1)
# this answer (630129827294205) was too high, though, perhaps because 26501365 is not a multiple of len(problem)
# the prime factorization of 26501365 is 5 * 11 * 481843
# 481843 is far too large to calculate, so maybe try multiples of 5?
    # print('%d: %d' % (5, len(findPlotsReachableInNSteps(problem, 5, True))))
    # print('%d: %d' % (10, len(findPlotsReachableInNSteps(problem, 10, True))))
    # print('%d: %d' % (15, len(findPlotsReachableInNSteps(problem, 15, True))))
    # print('%d: %d' % (20, len(findPlotsReachableInNSteps(problem, 20, True))))
    # print('%d: %d' % (25, len(findPlotsReachableInNSteps(problem, 25, True))))
    # print('%d: %d' % (30, len(findPlotsReachableInNSteps(problem, 30, True))))
# this suggested that 1293/1400 x^2 + 649/1400 x + 97/10 fit, but not precisely
# Observation from someone on reddit: 26501365 is 65 + (202300 * 131), where 131 is the width
# So my original regression may work correctly for 26501300, though testing that would be too expensive
    # print('%d: %d' % (len(problem) + 65, len(findPlotsReachableInNSteps(problem, len(problem) + 65, True))))
    # print('%d: %d' % (2 * len(problem) + 65, len(findPlotsReachableInNSteps(problem, 2 * len(problem) + 65, True))))
    # print('%d: %d' % (3 * len(problem) + 65, len(findPlotsReachableInNSteps(problem, 3 * len(problem) + 65, True))))
    # print('%d: %d' % (4 * len(problem) + 65, len(findPlotsReachableInNSteps(problem, 4 * len(problem) + 65, True))))
# This time the regression is (15397x^2 + 28235x - 79827)/17161, and it fits these four points perfectly.
    print((15397 * 26501365 * 26501365 + 28235 * 26501365 - 79827) // 17161)

main()
