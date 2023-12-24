"""Chad Hogg's solution to Advent Of Code 2023, day 19

"""

import sys
import re
from dataclasses import dataclass

@dataclass(frozen=True)
class Condition:
    var: str
    oper: str
    literal: int

@dataclass(frozen=True)
class Rule:
    cond: Condition
    target: str

@dataclass(frozen=True)
class Workflow:
    name: str
    rules: list[Rule]

@dataclass(frozen=True)
class Part:
    x: int
    m: int
    a: int
    s: int

@dataclass(frozen=True)
class Problem:
    workflows: dict[str, Workflow]
    parts: list[Part]

def readInput() -> list[list[int]]:
    lines = sys.stdin.readlines()
    workflows = {}
    parts = []
    firstPart = True
    for line in lines:
        if line.strip() == '':
            assert firstPart
            firstPart = False
        elif firstPart:
            match = re.match(r'^(\w+)\{(.*)\}$', line.strip())
            name = match.groups()[0]
            rules = []
            ruleStrings = match.groups()[1]
            for ruleString in ruleStrings.split(','):
                if ruleString != ruleStrings.split(',')[-1]:
                    match2 = re.match(r'^(\w)([<|>])(\d+):(\w+)$', ruleString)
                    rules.append(Rule(Condition(match2.groups()[0], match2.groups()[1], int(match2.groups()[2])), match2.groups()[3]))
                else:
                    rules.append(Rule(None, ruleString))
            workflows[name] = Workflow(name, rules)
        else:
            match3 = re.match(r'^\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\}$', line.strip())
            parts.append(Part(int(match3.groups()[0]), int(match3.groups()[1]), int(match3.groups()[2]), int(match3.groups()[3])))
    return Problem(workflows, parts)


def main():
    problem = readInput()
    print(str(problem))

main()
