"""Chad Hogg's solution to Advent Of Code 2023, day 1

My first attempt at writing Python in a long time, so probably not very pythonic
"""

import sys

def readInput():
    return sys.stdin.readlines()

def firstDigit(line):
    for index in range(0, len(line)):
        if line[index].isdigit():
            return line[index]
    return None

def lastDigit(line):
    for index in range(len(line) - 1, -1, -1):
        if line[index].isdigit():
            return line[index]
    return None

def part1(lines):
    total = 0
    for line in lines:
        total += int(firstDigit(line) + lastDigit(line))
    return total

def startsWithDigitWord(line):
    if line.startswith('one'):
        return '1'
    elif line.startswith('two'):
        return '2'
    elif line.startswith('three'):
        return '3'
    elif line.startswith('four'):
        return '4'
    elif line.startswith('five'):
        return '5'
    elif line.startswith('six'):
        return '6'
    elif line.startswith('seven'):
        return '7'
    elif line.startswith('eight'):
        return '8'
    elif line.startswith('nine'):
        return '9'
    else:
        return None

def fancyFirstDigit(line):
    for index in range(0, len(line)):
        written = startsWithDigitWord(line[index:])
        if line[index].isdigit():
            return line[index]
        elif not written is None:
            return written
    return None

def fancyLastDigit(line):
    for index in range(len(line) - 1, -1, -1):
        written = startsWithDigitWord(line[index:])
        if line[index].isdigit():
            return line[index]
        elif not written is None:
            return written
    return None

def part2(lines):
    total = 0
    for line in lines:
        total += int(fancyFirstDigit(line) + fancyLastDigit(line))
    return total

def main():
    lines = readInput()
    print(part1(lines))
    print(part2(lines))

main()