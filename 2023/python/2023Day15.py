"""Chad Hogg's solution to Advent Of Code 2023, day 15

"""

import sys
import re

def readInput() -> str:
    lines = sys.stdin.readlines()
    assert len(lines) == 1
    return lines[0].strip()

def HASH(input: str) -> int:
    hash = 0
    for char in input:
        hash += ord(char)
        hash *= 17
        hash %= 256
    return hash

def part1(steps: list[str]) -> int:
    return sum([HASH(x) for x in steps])

def addFocusingPower(boxes: list[tuple[str, str]]) -> int:
    sum = 0
    for boxIndex in range(0, len(boxes)):
        for lensIndex in range(0, len(boxes[boxIndex])):
            sum += (1 + boxIndex) * (1 + lensIndex) * int(boxes[boxIndex][lensIndex][1])
    return sum 

def part2(steps: list[str]) -> int:
    boxes = [[] for i in range(0, 256)]
    for step in steps:
        match = re.match(r'^(\w+)(-|=)(\d*)$', step)
        label = match.groups()[0]
        oper = match.groups()[1]
        focalLength = match.groups()[2]
        boxNum = HASH(label)
        box = boxes[boxNum]
        if oper == '-':
            assert focalLength == ''
            for lens in box:
                if lens[0] == label:
                    box.remove(lens)
        else:
            assert oper == '='
            assert focalLength != ''
            found = False
            for index in range(0, len(box)):
                if label == box[index][0]:
                    box[index] = (label, focalLength)
                    found = True
            if not found:
                box.append((label, focalLength))
    #for index in range(0, len(boxes)):
    #    if len(boxes[index]) > 0:
    #        print('Box %d: %s' % (index, str(boxes[index])))
    return addFocusingPower(boxes)


def main():
    initSequence = readInput()
    steps = initSequence.split(',')
    print(part1(steps))
    print(part2(steps))

main()
