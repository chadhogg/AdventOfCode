"""Chad Hogg's solution to Advent Of Code 2023, day 7

"""

import sys
import functools
import typing
from dataclasses import dataclass
from enum import Enum

class CardValue(Enum):
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE = range(0, 13)

class HandValue(Enum):
    HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_KIND, FULL_HOUSE, FOUR_OF_KIND, FIVE_OF_KIND = range(0, 7)

@dataclass
class Hand:
    cards: str
    bid: int
    value: HandValue
    valueWithJokers: HandValue

def computeHandValue(cards: str) -> HandValue:
    counts = {}
    for card in cards:
        counts[card] = counts.get(card, 0) + 1
    if len(counts) == 1:
        return HandValue.FIVE_OF_KIND
    elif 4 in counts.values():
        return HandValue.FOUR_OF_KIND
    elif 3 in counts.values() and 2 in counts.values():
        return HandValue.FULL_HOUSE
    elif 3 in counts.values():
        return HandValue.THREE_OF_KIND
    elif len(counts) == 3 and 2 in counts.values():
        return HandValue.TWO_PAIR
    elif 2 in counts.values():
        return HandValue.ONE_PAIR
    else:
        assert(len(counts) == 5)
        return HandValue.HIGH_CARD
    
def computeHandValueWithJokers(cards: str) -> HandValue:
    counts = {}
    jokers = 0
    for card in cards:
        if card == 'J':
            jokers += 1
        else:
            counts[card] = counts.get(card, 0) + 1
    if 5 in counts.values() or (4 in counts.values() and jokers == 1) or (3 in counts.values() and jokers == 2) or (2 in counts.values() and jokers == 3) or (1 in counts.values() and jokers == 4) or (jokers == 5):
        return HandValue.FIVE_OF_KIND
    elif 4 in counts.values() or (3 in counts.values() and jokers == 1) or (2 in counts.values() and jokers == 2) or (1 in counts.values() and jokers == 3):
        return HandValue.FOUR_OF_KIND
    elif (3 in counts.values() and 2 in counts.values()) or (len(counts) == 2 and 2 in counts.values() and jokers == 1):
        return HandValue.FULL_HOUSE
    elif 3 in counts.values() or (2 in counts.values() and jokers == 1) or (1 in counts.values() and jokers == 2):
        return HandValue.THREE_OF_KIND
    elif (len(counts) == 3 and 2 in counts.values()):
        return HandValue.TWO_PAIR
    elif 2 in counts.values() or jokers == 1:
        return HandValue.ONE_PAIR
    else:
        assert(len(counts) == 5 and jokers == 0)
        return HandValue.HIGH_CARD

def strToCardValue(card: str) -> CardValue:
    if card == '2': return CardValue.TWO
    elif card == '3': return CardValue.THREE
    elif card == '4': return CardValue.FOUR
    elif card == '5': return CardValue.FIVE
    elif card == '6': return CardValue.SIX
    elif card == '7': return CardValue.SEVEN
    elif card == '8': return CardValue.EIGHT
    elif card == '9': return CardValue.NINE
    elif card == 'T': return CardValue.TEN
    elif card == 'J': return CardValue.JACK
    elif card == 'Q': return CardValue.QUEEN
    elif card == 'K': return CardValue.KING
    else:
        assert(card == 'A')
        return CardValue.ACE
    
def compareHands(a: Hand, b: Hand) -> int:
    if a.value.value < b.value.value: return -1
    elif b.value.value < a.value.value: return 1
    for index in range(0, len(a.cards)):
        if strToCardValue(a.cards[index]).value < strToCardValue(b.cards[index]).value: return -1
        elif strToCardValue(b.cards[index]).value < strToCardValue(a.cards[index]).value: return 1
    return 0

def readInput() -> list[Hand]:
    lines = sys.stdin.readlines()
    return [Hand(line.split()[0], int(line.split()[1]), computeHandValue(line.split()[0]), computeHandValueWithJokers(line.split()[0])) for line in lines]

def part1(problem: list[Hand], comparator: typing.Callable) -> int:
    problem = sorted(problem, key=functools.cmp_to_key(comparator), reverse=False)
    sum = 0
    for index in range(0, len(problem)):
        sum += (index + 1) * problem[index].bid
    return sum

def compareHandsWithJoker(a: Hand, b: Hand) -> int:
    if a.valueWithJokers.value < b.valueWithJokers.value: return -1
    elif b.valueWithJokers.value < a.valueWithJokers.value: return 1
    for index in range(0, len(a.cards)):
        if a.cards[index] == 'J' and b.cards[index] != 'J': return -1
        elif b.cards[index] == 'J' and a.cards[index] != 'J': return 1
        if strToCardValue(a.cards[index]).value < strToCardValue(b.cards[index]).value: return -1
        elif strToCardValue(b.cards[index]).value < strToCardValue(a.cards[index]).value: return 1
    return 0


def main():
    problem = readInput()
    print(part1(problem, compareHands))
    print(part1(problem, compareHandsWithJoker))

main()
