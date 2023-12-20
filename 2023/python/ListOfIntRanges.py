"""A module that deals with efficiently tracking non-overlapping ranges of integers."""

class IntRange:
    def __init__(self, low: int, high: int):
        assert low <= high
        self.low = low
        self.high = high
    
    def __str__(self) -> str:
        return '[%d, %d]' % (self.low, self.high)
    
    def __repr__(self) -> str:
        return str(self)

    def isIn(self, num: int):
        return num >= self.low and num <= self.high


class IntRangeList:

    def __init__(self):
        self.ranges = []
    
    def __str__(self) -> str:
        return str(self.ranges)

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
