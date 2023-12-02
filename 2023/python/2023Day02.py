"""Chad Hogg's solution to Advent Of Code 2023, day 2

"""

import sys
import re

def readInput():
    lines = sys.stdin.readlines()
    games = []
    for line in lines:
        number = int(re.match(r'^Game (\d+):.*', line).group(1))
        roundPart = re.split(r':', line)[1]
        rounds = re.split(r';', roundPart)
        game = {'number' : number, 'rounds' : []}
        for round in rounds:
            shown = {'blue': 0, 'red': 0, 'green': 0}
            for part in re.split(r',', round):
                groups = re.match(r'\s*(\d+)\s*(\w+)\s*', part)
                shown[groups[2]] = int(groups[1])
            game['rounds'].append(shown)
        games.append(game)
    return games

def possible(game, red, green, blue):
    for round in game['rounds']:
        if round['blue'] > blue or round['red'] > red or round['green'] > green:
            return False
    return True

def part1(games):
    sum = 0
    for game in games:
        if possible(game, 12, 13, 14):
            sum += game['number']
    return sum

def minPossible(game):
    limit = game['rounds'][0]
    for round in game['rounds']:
        limit['blue'] = max(limit['blue'], round['blue'])
        limit['red'] = max(limit['red'], round['red'])
        limit['green'] = max(limit['green'], round['green'])
    return limit

def computePower(limit):
    return limit['blue'] * limit['red'] * limit['green']

def part2(games):
    sum = 0
    for game in games:
        sum += computePower(minPossible( game))
    return sum

def main():
    games = readInput()
    print(part1(games))
    print(part2(games))

main()
