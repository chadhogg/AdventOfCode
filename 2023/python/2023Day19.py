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
    values: dict[str, int]

    def rating(self) -> int:
        return self.values['x'] + self.values['m'] + self.values['a'] + self.values['s']

@dataclass(frozen=True)
class Problem:
    workflows: dict[str, Workflow]
    parts: list[Part]

def readInput() -> Problem:
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
            parts.append(Part({'x' : int(match3.groups()[0]), 'm' : int(match3.groups()[1]), 'a' : int(match3.groups()[2]), 's' : int(match3.groups()[3])}))
    return Problem(workflows, parts)

def passed(cond: Condition, part: Part) -> bool:
    if cond == None: return True
    elif cond.oper == '>': return part.values[cond.var] > cond.literal
    elif cond.oper == '<': return part.values[cond.var] < cond.literal
    else: return None

def classify(workflows: dict[str, Workflow], part: Part) -> bool:
    name: str = 'in'
    while name != 'A' and name != 'R':
        wf: Workflow = workflows[name]
        for rule in wf.rules:
            if passed(rule.cond, part):
                name = rule.target
                break
    return name == 'A'

def allPossibleParts():
    for a in range(1, 4001):
        for b in range(1, 4001):
            for c in range(1, 4001):
                for d in range(1, 4001):
                    yield Part({'x': a, 'm': b, 'a': c, 's': d})

def main():
    problem = readInput()
    print(sum([a.rating() if classify(problem.workflows, a) else 0 for a in problem.parts]))
    print(sum([a.rating() if classify(problem.workflows, a) else 0 for a in allPossibleParts()]))

main()
