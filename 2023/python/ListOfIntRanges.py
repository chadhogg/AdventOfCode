"""A module that deals with efficiently tracking non-overlapping ranges of integers."""

import copy

class IntRange:
    """A range of integers, from low to high inclusively."""
    def __init__(self, low: int, high: int):
        assert low <= high
        self.low = low
        self.high = high
    
    def __str__(self) -> str:
        return '[%d, %d]' % (self.low, self.high)
    
    def __repr__(self) -> str:
        return str(self)
    
    def __deepcopy__(self, memo):
        return IntRange(self.low, self.high)

    def isIn(self, num: int):
        return num >= self.low and num <= self.high


class IntRangeList:
    """A list of non-overlapping IntRanges, in numerical order."""
    def __init__(self):
        self.ranges = []
    
    def __str__(self) -> str:
        return str(self.ranges)
    
    def __repr__(self) -> str:
        return str(self)

    def __deepcopy__(self, memo):
        c = IntRangeList()
        c.ranges = [copy.deepcopy(x) for x in self.ranges]
        return c

    def _simplify(self):
        index = 0
        while index < len(self.ranges) - 1:
            if self.ranges[index].high + 1 == self.ranges[index + 1].low:
                self.ranges[index].high = self.ranges[index + 1].high
                del self.ranges[index + 1]
            else:
                index += 1

    def _mergeInRecurse(self, extra: IntRange):
        for index in range(0, len(self.ranges)):
            old = self.ranges[index]
            # Case 00: extra range falls entirely before existing range --
            if extra.high < old.low:
                # So just insert before old range and we are finished.
                self.ranges.insert(index, extra)
                return
            # Case 01: extra range starts before existing range but ends somewhere inside it --
            elif extra.low < old.low and extra.high <= old.high:
                # So extend old range to include extra and we are finished.
                old.low = extra.low
                return
            # Case 02: extra range starts before existing range and ends after it --
            elif extra.low < old.low and extra.high > old.high:
                # So split extra into two parts and handle them separately.
                self._mergeInRecurse(IntRange(extra.low, old.high)) # Case 01
                self._mergeInRecurse(IntRange(old.high + 1, extra.high)) # Case 05
                return
            # Case 03: extra range starts inside existing and ends inside existing --
            elif extra.low >= old.low and extra.high <= old.high:
                # So ignore extra range.
                return
            # Case 04: extra range starts inside existing and ends after existing --
            elif extra.low <= old.high and extra.high > old.high:
                # So split extra into to parts and handle them separately.
                self._mergeInRecurse(IntRange(extra.low, old.high)) # Case 03
                self._mergeInRecurse(IntRange(old.high + 1, extra.high)) # Case 05
                return
            # Case 05: extra range falls entirely after existing --
            else:
                assert extra.low > old.high
                # So just move on to next existing range
        # It comes after all existing ranges, so append.
        assert len(self.ranges) == 0 or self.ranges[-1].high < extra.low
        self.ranges.append(extra)
            
    def mergeIn(self, extra: IntRange):
        self._mergeInRecurse(extra)
        self._simplify()

    def contains(self, num: int) -> bool:
        return any([r.isIn(num) for r in self.ranges])


class Compressed2DBooleanList:
    """A list of IntRangeLists, in which consecutive duplicates are merged."""

    def __init__(self, rows: int):
        self.rows = rows
        self._lines = [(IntRangeList(), rows)]

    def setHorizontal(self, y: int, xLow: int, xHigh: int):
        row = 0
        index = 0
        while index < len(self._lines):
            # The row we want to change comes after this region, so keep moving.
            if y >= row + self._lines[index][1]:
                row += self._lines[index][1]
                index += 1
            # The row we want to change is exactly this region, so just do it.
            elif row == y and self._lines[index][1] == 1:
                self._lines[index][0].mergeIn(IntRange(xLow, xHigh))
                return
            # The row we want to change is somewhere inside this region, so split it.
            else:
                assert y >= row and y < row + self._lines[index][1]
                before = (copy.deepcopy(self._lines[index][0]), y - row)
                single = (self._lines[index][0], 1)
                after = (copy.deepcopy(self._lines[index][0]), self._lines[index][1] - (y - row) - 1)
                single[0].mergeIn(IntRange(xLow, xHigh))
                self._lines[index] = single
                if(after[1] > 0):
                    self._lines.insert(index + 1, after)
                if(before[1] > 0):
                    self._lines.insert(index, before)
                return
    
    def _setVerticalRecurse(self, yLow: int, yHigh: int, x: int):
        index = 0
        row = 0
        while index < len(self._lines):
            # The range we want to change falls entirely after this one, so move on.
            if yLow >= row + self._lines[index][1]:
                row += self._lines[index][1]
                index += 1
            # The range we want to change is exactly this range, so change it.
            elif yLow == row and yHigh == row + self._lines[index][1] - 1:
                self._lines[index][0].mergeIn(IntRange(x, x))
                return
            # The range we want to change lies inside part of this range, so split this range up.
            elif yLow >= row and yHigh <= row + self._lines[index][1] - 1:
                before = (copy.deepcopy(self._lines[index][0]), yLow - row)
                inside = (self._lines[index][0], yHigh - yLow + 1)
                after = (copy.deepcopy(self._lines[index][0]), row + self._lines[index][1] - 1 - yHigh)
                assert before[1] + inside[1] + after[1] == self._lines[index][1]
                self._lines[index] = inside
                if after[1] > 0:
                    self._lines.insert(index + 1, after)
                if before[1] > 0:
                    self._lines.insert(index, before)
                self._setVerticalRecurse(yLow, yHigh, x)
                return
            # The range we want to change lines partially but not entirely below this range, so split it up.
            elif yLow < row and yHigh >= row:
                self._setVerticalRecurse(yLow, row - 1, x)
                self._setVerticalRecurse(row, yHigh, x)
                return
            # The range we want to change lies partially but not entirely above this range, so split it up.
            else:
                assert yLow <= row + self._lines[index][1] and yHigh > row
                self._setVerticalRecurse(yLow, row + self._lines[index][1] - 1, x)
                self._setVerticalRecurse(row + self._lines[index][1], yHigh, x)
                return
        assert False

    def setVertical(self, yLow: int, yHigh: int, x: int):
        self._setVerticalRecurse(yLow, yHigh, x)
        self.compress()

    def compress(self):
        index = 0
        while index < len(self._lines) - 1:
            if self._lines[index][0] == self._lines[index + 1][0]:
                self._lines[index] = (self._lines[index][0], self._lines[index][1] + self._lines[index + 1][1])
                del self._lines[index + 1]
            else:
                index += 1
    
    def getLine(self, y: int) -> IntRangeList:
        row = 0
        index = 0
        while row < y:
            row += self._lines[index][1]
            if row <= y:
                index += 1
        return self._lines[index][0]

    def __str__(self):
        output = ''
        for index in range(0, len(self._lines)):
            output += str(self._lines[index][0]) + ' repeated ' + str(self._lines[index][1]) + ' times\n'
        return output
