"""Chad Hogg's solution to Advent Of Code 2023, day 5
I had to fight *hard* to get part 2 working, and I'm sure it could be dramatically improved.
"""

import sys
import re

from dataclasses import dataclass
from enum import Enum

class MapType(Enum):
    SEED_TO_SOIL, SOIL_TO_FERTILIZER, FERTILIZER_TO_WATER, WATER_TO_LIGHT, LIGHT_TO_TEMPERATURE, TERMPERATURE_TO_HUMIDITY, HUMIDITY_TO_LOCATION = range(0, 7)

@dataclass
class MapRange:
    dRangeStart: int
    sRangeStart: int
    rangeLength: int

class Almanac:
    seeds: [] = []
    maps: {} = {}


def readMap(lines: [], mapName: str, lineNum: int) -> ([], int):
    assert (lineNum < len(lines))
    assert (lines[lineNum].strip() == mapName + ' map:')
    lineNum += 1
    ranges = []
    while (lineNum < len(lines) and lines[lineNum].strip() != ''):
        match = re.match(r'^(\d+) (\d+) (\d+)$', lines[lineNum].strip())
        assert(match is not None)
        assert(len(match.groups()) == 3)
        ranges.append(MapRange(int(match.groups()[0]), int(match.groups()[1]), int(match.groups()[2])))
        lineNum += 1
    lineNum += 1
    return ranges, lineNum

def readInput() -> []:
    lines = sys.stdin.readlines()
    almanac = Almanac()
    lineNum = 0
    assert (lineNum < len(lines))
    assert (lines[lineNum][0:6] == 'seeds:')
    for seed in re.split(r'\s+', lines[lineNum][6:].strip()):
        almanac.seeds.append(int(seed))
    lineNum += 1
    assert (lineNum < len(lines))
    assert (lines[lineNum].strip() == "")
    lineNum += 1
    almanac.maps[MapType.SEED_TO_SOIL], lineNum = readMap(lines, 'seed-to-soil', lineNum)
    almanac.maps[MapType.SOIL_TO_FERTILIZER], lineNum = readMap(lines, 'soil-to-fertilizer', lineNum)
    almanac.maps[MapType.FERTILIZER_TO_WATER], lineNum = readMap(lines, 'fertilizer-to-water', lineNum)
    almanac.maps[MapType.WATER_TO_LIGHT], lineNum = readMap(lines, 'water-to-light', lineNum)
    almanac.maps[MapType.LIGHT_TO_TEMPERATURE], lineNum = readMap(lines, 'light-to-temperature', lineNum)
    almanac.maps[MapType.TERMPERATURE_TO_HUMIDITY], lineNum = readMap(lines, 'temperature-to-humidity', lineNum)
    almanac.maps[MapType.HUMIDITY_TO_LOCATION], lineNum = readMap(lines, 'humidity-to-location', lineNum)
    return almanac


def applyMap(item: int, map: []) -> int:
    for mapRange in map:
        if item in range(mapRange.sRangeStart, mapRange.sRangeStart + mapRange.rangeLength):
            return item + mapRange.dRangeStart - mapRange.sRangeStart
    return item


def applyMapToAll(items: [], map: []) -> []:
    result = []
    for item in items:
        result.append(applyMap(item, map))
    return result
        

def findLocations(almanac: Almanac) -> []:
    seeds = almanac.seeds
    soils = applyMapToAll(seeds, almanac.maps[MapType.SEED_TO_SOIL])
    fertilizers = applyMapToAll(soils, almanac.maps[MapType.SOIL_TO_FERTILIZER])
    waters = applyMapToAll(fertilizers, almanac.maps[MapType.FERTILIZER_TO_WATER])
    lights = applyMapToAll(waters, almanac.maps[MapType.WATER_TO_LIGHT])
    temperatures = applyMapToAll(lights, almanac.maps[MapType.LIGHT_TO_TEMPERATURE])
    humidities = applyMapToAll(temperatures, almanac.maps[MapType.TERMPERATURE_TO_HUMIDITY])
    locations = applyMapToAll(humidities, almanac.maps[MapType.HUMIDITY_TO_LOCATION])
    return locations

def part1(almanac: Almanac) -> int:
    locations = findLocations(almanac)
    return min(locations)

@dataclass
class Range:
    start: int
    len: int

def applyMapToAllRanges(ranges: [], map: []) -> []:
    newRanges = []
    while len(ranges) > 0:
        current = ranges.pop()
        for mapRange in map:
            # If part of the range falls inside this mapRange, split it into three pieces
            if current.len > 0 and current.start < mapRange.sRangeStart + mapRange.rangeLength and current.start + current.len > mapRange.sRangeStart:
                # First, a part that lies before this mapRange.  If non-empty, split off and add to queue.
                before = Range(current.start, mapRange.sRangeStart - current.start)
                if before.len > 0:
                    ranges.append(before)
                # Second, a part that lies after this mapRange.  If non-empty, split off and add to queue.
                after = Range(mapRange.sRangeStart + mapRange.rangeLength, current.start + current.len - mapRange.sRangeStart - mapRange.rangeLength)
                if after.len > 0:
                    ranges.append(after)
                # Finally, a part that lies entirely within this range.  Apply the transformation.
                start = max(current.start, mapRange.sRangeStart)
                length = current.len - max(0, before.len) - max(0, after.len)
                during = Range(start, min(current.len, length))
                transformed = Range(during.start + mapRange.dRangeStart - mapRange.sRangeStart, during.len)
                newRanges.append(transformed)
                # Empty out this range so that we don't try to do more with it later
                current.len = 0
        # No mapRange claimed this chunk.
        if current.len > 0:
            newRanges.append(current)
    return newRanges


def findLocations2(almanac: Almanac) -> []:
    seeds = []
    for index in range(0, len(almanac.seeds), 2):
        seeds.append(Range(almanac.seeds[index], almanac.seeds[index + 1]))
    print('#seed ranges: ' + str(len(seeds)))
    soils = applyMapToAllRanges(seeds, almanac.maps[MapType.SEED_TO_SOIL])
    print('#soils ranges: ' + str(len(soils)))
    fertilizers = applyMapToAllRanges(soils, almanac.maps[MapType.SOIL_TO_FERTILIZER])
    print('#fertilizers ranges: ' + str(len(fertilizers)))
    waters = applyMapToAllRanges(fertilizers, almanac.maps[MapType.FERTILIZER_TO_WATER])
    print('#waters ranges: ' + str(len(waters)))
    lights = applyMapToAllRanges(waters, almanac.maps[MapType.WATER_TO_LIGHT])
    print('#lights ranges: ' + str(len(lights)))
    temperatures = applyMapToAllRanges(lights, almanac.maps[MapType.LIGHT_TO_TEMPERATURE])
    print('#temperatures ranges: ' + str(len(temperatures)))
    humidities = applyMapToAllRanges(temperatures, almanac.maps[MapType.TERMPERATURE_TO_HUMIDITY])
    print('#humidities ranges: ' + str(len(humidities)))
    locations = applyMapToAllRanges(humidities, almanac.maps[MapType.HUMIDITY_TO_LOCATION])
    print('#locations ranges: ' + str(len(locations)))
    return locations

def part2(almanac: Almanac) -> int:
    locations = findLocations2(almanac)
    lowest = locations[0].start
    for location in locations:
        if location.start < lowest:
            lowest = location.start
    return lowest


def main():
    almanac = readInput()
    print(part1(almanac))
    print(part2(almanac))

main()
