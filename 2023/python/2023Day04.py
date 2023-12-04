"""Chad Hogg's solution to Advent Of Code 2023, day 4

"""

import sys
import re

def readInput() -> []:
    lines = sys.stdin.readlines()
    cards = []
    for line in lines:
        match = re.match(r'^Card\s+(\d+):(.*)\|(.*)$', line)
        cards.append({'num' : int(match.groups()[0]),
                      'winners' : re.split(r'\s+', match.groups()[1].strip()),
                      'mine' : re.split(r'\s+', match.groups()[2].strip())})
    return cards


def part1(cards: []) -> int:
    sum = 0
    for card in cards:
        matches = set(card['winners']) & set(card['mine'])
        if len(matches) > 0:
            sum += 2 ** (len(matches) - 1)
    return sum

def part2(cards: []) -> int:
    counts = {}
    for card in cards:
        counts[card['num']] = 1
    for card in cards:
        matches = set(card['winners']) & set(card['mine'])
        for i in range (1, len(matches) + 1):
            counts[card['num'] + i] += counts[card['num']]
    return sum(counts.values())

def main():
    cards = readInput()
    print(part1(cards))
    print(part2(cards))

main()
