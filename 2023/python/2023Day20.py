"""Chad Hogg's solution to Advent Of Code 2023, day 20

Least favorite problem ever.  Still don't understand why my algorithm guarantees a solution.
"""

import sys
import graphviz
import math
from dataclasses import dataclass
from collections.abc import Iterator

FLIPFLOP = 1
CONJUNCTION = 2
BROADCAST = 3

LOW = 1
HIGH = 2

@dataclass(frozen=True)
class Module:
    type: int
    name: str
    outputs: list[str]

@dataclass(frozen=True)
class Pulse:
    type: int
    dest: str
    src: str

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

def processPulse(problem: dict[str, Module], flopsHigh: set[str], conjs: dict[str, dict[str, int]], pulse: Pulse) -> Iterator[Pulse]:
    if pulse.dest not in problem:
        return
    elif problem[pulse.dest].type == BROADCAST:
        for dest in problem[pulse.dest].outputs:
            yield Pulse(pulse.type, dest, pulse.dest)
    elif problem[pulse.dest].type == FLIPFLOP:
        if pulse.type == LOW:
            if pulse.dest in flopsHigh:
                flopsHigh.remove(pulse.dest)
                for dest in problem[pulse.dest].outputs:
                    yield Pulse(LOW, dest, pulse.dest)
            else:
                flopsHigh.add(pulse.dest)
                for dest in problem[pulse.dest].outputs:
                    yield Pulse(HIGH, dest, pulse.dest)
    else:
        assert problem[pulse.dest].type == CONJUNCTION
        conjs[pulse.dest][pulse.src] = pulse.type
        if all([True if x == HIGH else False for x in conjs[pulse.dest].values()]):
            for dest in problem[pulse.dest].outputs:
                yield Pulse(LOW, dest, pulse.dest)
        else:
            for dest in problem[pulse.dest].outputs:
                yield Pulse(HIGH, dest, pulse.dest)

def buttonPress(problem: dict[str, Module], flopsHigh: set[int], conjs: dict[str, dict[str, int]]) -> (int, int):
    lows: int = 0
    highs: int = 0
    todo: list[Pulse] = []
    todo.append(Pulse(LOW, 'broadcaster', 'button'))
    while todo:
        next: Pulse = todo[0]
        del todo[0]
        if next.type == LOW: lows += 1
        else: highs += 1
        todo.extend(processPulse(problem, flopsHigh, conjs, next))
    return (lows, highs)

def initialize(problem: dict[str, Module], flopsHigh: set[int], conjs: dict[str, dict[str, int]]):
    flopsHigh.clear()
    conjs.clear()
    for module in problem.values():
        if module.type == CONJUNCTION:
            conjs[module.name] = {}
            for module2 in problem.values():
                if module.name in module2.outputs:
                    conjs[module.name][module2.name] = LOW

def part1(problem: dict[str, Module]) -> int:
    totalLow: int = 0
    totalHigh: int = 0
    flopsHigh: set[int] = set()
    conjs: dict[str, dict[str, int]] = dict()
    initialize(problem, flopsHigh, conjs)
    for _ in range(0, 1000):
        (low, high) = buttonPress(problem, flopsHigh, conjs)
        totalLow += low
        totalHigh += high
    return totalLow * totalHigh

# rx is fed by a single conjunction modules, gf.
# gf is fed by 4 conjunction modules: qs, sv, pg, sp, each of which come from a completely 
#   distinct subgraph.
# In order for gf to ouput LOW, it needs to be
#   remembering HIGH for three of them and receive HIGH for the fourth.
# (qs, sv, pg, sp) are fed by 1 conjunction each (mh, jt, pz, rn) respectively.
# In order for (sq, sv, pg, sp) to send a HIGH, (mh, jt, pz, rn) need to be receive LOWs.
# Each of (mh, jt, pz, rn) are fed by a large number of flip-flops, some of which also feed 
#   into each other.
# In order for (mh, jt, pz, rn) to send a LOW, they need to be remembering a HIGH from each
#   attached flip-flop.

def pressesUntilHigh(problem: dict[str, Module], name: str) -> int:
    flopsHigh: set[int] = set()
    conjs: dict[str, dict[str, int]] = dict()
    initialize(problem, flopsHigh, conjs)
    presses = 0
    while True:
        buttonPress(problem, flopsHigh, conjs)
        presses += 1
        if any([True if x == LOW else False for x in conjs[name].values()]):
            return presses
        
def detectCycle(problem: dict[str, Module], flops: set[str]):
    flopsHigh: set[int] = set()
    conjs: dict[str, dict[str, int]] = dict()
    initialize(problem, flopsHigh, conjs)
    presses = 0
    history = {}
    while True:
        relevantHighFlops = tuple(flopsHigh.intersection(flops))
        if relevantHighFlops in history:
            return presses - history[relevantHighFlops]
        else:
            history[relevantHighFlops] = presses                  
        buttonPress(problem, flopsHigh, conjs)
        presses += 1
    
        
def graphVizIt(problem: dict[str, Module]):
    dot = graphviz.Digraph('aoc2023day20')
    for name in problem:
        if problem[name].type == FLIPFLOP:
            dot.node(name, '%' + name)
        elif problem[name].type == CONJUNCTION:
            dot.node(name, '&' + name)
        else:
            dot.node(name, name)
    for name in problem:
        for target in problem[name].outputs:
            if target not in problem:
                dot.node(target, target)
            dot.edge(name, target)
    dot.render()

def main():
    problem = readInput()
    print(part1(problem))
    pgCycle = detectCycle(problem, {'jp', 'vr', 'hp', 'tx', 'dx', 'ph', 'mb', 'jc', 'kt', 'ct', 'kd', 'pp'})
    spCycle = detectCycle(problem, {'rv', 'fk', 'mv', 'kn', 'fn', 'nx', 'xc', 'ff', 'hr', 'sk', 'fx', 'bx'})
    svCycle = detectCycle(problem, {'vk', 'nk', 'xk', 'jm', 'lp', 'vp', 'fb', 'zb', 'dt', 'lj', 'qt', 'jq'})
    qsCycle = detectCycle(problem, {'nv', 'th', 'hx', 'xb', 'jf', 'bc', 'xm', 'gv', 'nr', 'cj', 'vh', 'jh'})
    print(math.lcm(pgCycle, spCycle, svCycle, qsCycle))
#    graphVizIt(problem)
#    print(pressesUntilHigh(problem, 'qs'))

main()
