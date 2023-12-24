"""Chad Hogg's solution to Advent Of Code 2023, day 20

"""

import sys
import re
from dataclasses import dataclass

FLIPFLOP = 1
CONJUNCTION = 2
BROADCAST = 3

@dataclass(frozen=True)
class Module:
    type: int
    name: str
    outputs: list[str]

def readInput() -> dict[str, Module]:
    lines = sys.stdin.readlines()
    modules = {}
    for line in lines:
        before, after = line.strip().split(' -> ')
        outputs = [a.strip() for a in after.split(',')]
        if before[0] == '%':
            assert before[1:] not in modules
            modules[before[1:]] = Module(FLIPFLOP, before[1:], outputs)
        elif before[0] == '&':
            assert before[1:] not in modules
            modules[before[1:]] = Module(CONJUNCTION, before[1:], outputs)
        else:
            assert before == 'broadcaster'
            assert before not in modules
            modules[before] = Module(BROADCAST, before, outputs)
    return modules

def main():
    problem = readInput()

main()
