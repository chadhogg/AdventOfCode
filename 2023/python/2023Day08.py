"""Chad Hogg's solution to Advent Of Code 2023, day 8

"""

import sys
import re
import typing
import math
from dataclasses import dataclass

@dataclass
class Problem:
    turns: str
    network: dict


def readInput() -> []:
    lines = sys.stdin.readlines()
    turns = lines[0].strip()
    network = {}
    for line in lines[2:]:
        match = re.match('^(\w\w\w) = \((\w\w\w), (\w\w\w)\)', line)
        network[match.groups()[0]] = (match.groups()[1], match.groups()[2])
    return Problem(turns, network)

def followPath(problem: Problem, nextMove: int, startState: str, endState: str) -> int:
    moves = 0
    currentState = startState
    while not currentState.endswith(endState):
        if problem.turns[nextMove % len(problem.turns)] == 'L':
            currentState = problem.network[currentState][0]
        else:
            assert(problem.turns[nextMove % len(problem.turns)] == 'R')
            currentState = problem.network[currentState][1]
        nextMove += 1
        moves += 1
    return moves

def followPaths(problem: Problem, nextMove: int, startLetter: str, endLetter: str) -> int:
    moves = 0
    currentStates = {x for x in problem.network.keys() if x[2] == startLetter}
    while len({x for x in currentStates if not x[2] == endLetter}) > 0:
        nextStates = set()
        if problem.turns[nextMove % len(problem.turns)] == 'L':
            for s in currentStates:
                nextStates.add(problem.network[s][0])
        else:
            assert(problem.turns[nextMove % len(problem.turns)] == 'R')
            for s in currentStates:
                nextStates.add(problem.network[s][1])
        nextMove += 1
        moves += 1
        currentStates = nextStates
    return moves

def followPathsSmart(problem: Problem, nextMove: int, startLetter: str, endLetter: str) -> int:
    startStates = {x for x in problem.network.keys() if x[2] == startLetter}
    results = set()
    for state in startStates:
        results.add(followPath(problem, 0, state, 'Z'))
    return math.lcm(*results)

def main():
    problem = readInput()
    print(followPath(problem, 0, 'AAA', 'ZZZ'))
    print(followPathsSmart(problem, 0, 'A', 'Z'))

main()
