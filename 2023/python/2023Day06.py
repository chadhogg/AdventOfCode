"""Chad Hogg's solution to Advent Of Code 2023, day 6

"""

import sys
import re

def readInput() -> []:
    lines = sys.stdin.readlines()
    assert(len(lines) == 2)
    assert(lines[0][0:5] == 'Time:')
    assert(lines[1][0:9] == 'Distance:')
    times = re.findall(r'\d+', lines[0])
    distances = re.findall(r'\d+', lines[1])
    assert(len(times) == len(distances))
    problem = {}
    problem['t'] = [int(t) for t in times]
    problem['d'] = [int(d) for d in distances]
    return problem

# x is time spent holding button / speed
# (t - x) is time moving before race ends
# x * (t - x) >= d

def findMinWinningSpeed(t : int, d : int) -> int:
    for x in range (1, t):
        if x * (t - x) > d:
            return x
    return None

def findMaxWinningSpeed(t : int, d : int) -> int:
    for x in range (t, 1, -1):
        if x * (t - x) > d:
            return x
    return None

def part1(problem: {}) -> int:
    product = 1
    for index in range(0, len(problem['t'])):
        small = findMinWinningSpeed(problem['t'][index], problem['d'][index])
        large = findMaxWinningSpeed(problem['t'][index], problem['d'][index])
        product *= (large - small + 1)
    return product

def main():
    problem = readInput()
    print(problem)
    print(part1(problem))
    revised = {'t': [int(''.join([str(t) for t in problem['t']]))],
               'd': [int(''.join([str(d) for d in problem['d']]))]}
    print(revised)
    print(part1(revised))

main()
