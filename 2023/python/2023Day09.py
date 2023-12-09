"""Chad Hogg's solution to Advent Of Code 2023, day 9

"""

import sys


def readInput() -> []:
    lines = sys.stdin.readlines()
    sequences = []
    for line in lines:
        sequences.append([int(x) for x in line.strip().split()])
    return sequences

def isZero(x : int) -> bool:
    return x == 0

def allZeroes(sequence: list[int]) -> bool:
    return all (isZero(x) for x in sequence)

def predict(sequence: list[int]) -> int:
    reductions = [sequence]
    while not allZeroes(reductions[-1]):
        pairs = zip(reductions[-1][:-1], reductions[-1][1:])
        reductions.append([b - a for (a, b) in pairs])
    return sum(x[-1] for x in reductions)
    
def part1(sequences: list[list[int]]) -> int:
    return sum([predict(x) for x in sequences])

def predictBackwards(sequence: list[int]) -> int:
    reductions = [sequence]
    while not allZeroes(reductions[-1]):
        pairs = zip(reductions[-1][:-1], reductions[-1][1:])
        reductions.append([b - a for (a, b) in pairs])
    for index in range(len(reductions) - 2, -1, -1):
        reductions[index] = [reductions[index][0] - reductions[index + 1][0]] + reductions[index]
    return reductions[0][0]

def part2(sequences: list[list[int]]) -> int:
    return sum([predictBackwards(x) for x in sequences])


def main():
    sequences = readInput()
    print(part1(sequences))
    print(part2(sequences))


main()
