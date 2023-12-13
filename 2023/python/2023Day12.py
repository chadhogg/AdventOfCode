"""Chad Hogg's solution to Advent Of Code 2023, day 12

"""

import sys
from dataclasses import dataclass
import functools

OPERATIONAL = '.'
DAMAGED = '#'
UNKNOWN = '?'

@dataclass(frozen=True)
class SpringRow:
    springs: str
    dmgLengths: tuple


def readInput() -> list[SpringRow]:
    lines = sys.stdin.readlines()
    rows = []
    for line in lines:
        part1, part2 = line.strip().split()
        rows.append(SpringRow(part1, tuple([int(x) for x in part2.split(',')])))
    return rows

# Automatic memoization -- how cool is that?
@functools.cache
def countArrangements(row: SpringRow) -> int:
    result = 0
    if len(row.dmgLengths) == 0:
        # No more matches needed
        if DAMAGED not in row.springs:
            # And no more damaged parts
            return 1
        else:
            # And some damaged parts that won't be matched
            return 0
    elif len(row.springs) < sum(row.dmgLengths) + len(row.dmgLengths) - 1:
        # Not enough text left to match everything with separators in between them
        return 0
    elif len(row.springs) == row.dmgLengths[0]:
        # We only have what's left
        if OPERATIONAL in row.springs:
            # And we can't use it
            return 0
        else:
            # And it will work!
            assert len(row.dmgLengths) == 1
            return 1
    elif row.springs[0] == DAMAGED:
        # We *MUST* use this character
        if OPERATIONAL not in row.springs[0:row.dmgLengths[0]] and row.springs[row.dmgLengths[0]] != DAMAGED:
            # And we can!
            return countArrangements(SpringRow(row.springs[row.dmgLengths[0] + 1:], row.dmgLengths[1:]))
        else:
            # But we can't
            return 0
    elif row.springs[0] == UNKNOWN and OPERATIONAL not in row.springs[0:row.dmgLengths[0]] and row.springs[row.dmgLengths[0]] != DAMAGED:
        # We can use this character, but don't have to
        return countArrangements(SpringRow(row.springs[row.dmgLengths[0] + 1:], row.dmgLengths[1:])) + countArrangements(SpringRow(row.springs[1:], row.dmgLengths))
    else:
        # We cannot use this character
        return countArrangements(SpringRow(row.springs[1:], row.dmgLengths))
    

def part1(rows: list[SpringRow]):
    return sum(countArrangements(row) for row in rows)

def part2(rows: list[SpringRow]):
    sum = 0
    for row in rows:
        longSprings = row.springs + '?' + row.springs + '?' + row.springs + '?' + row.springs + '?' + row.springs
        longDmgLengths = row.dmgLengths * 5
        sum += countArrangements(SpringRow(longSprings, longDmgLengths))
    return sum
#    return sum([countArrangements(SpringRow(row.springs + '?' + row.springs + '?' + row.springs + '?' + row.springs + '?' + row.springs, row.dmgLengths + row.dmgLengths + row.dmgLengths + row.dmgLengths + row.dmgLengths) for row in rows)])

def main():
    rows = readInput()
    print(part1(rows))
    print(part2(rows))

main()
