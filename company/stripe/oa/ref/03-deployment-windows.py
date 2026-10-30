# Stripe OA: Deployment Window Scheduler (HackerRank, 2 parts).
# Function: schedule_deployment_windows(part: str, input_csv: list[str])
#   -> list[list[int]]
#
# Time is minute_of_week in [0, 10079]: minutes since Monday 00:00 (60 = Monday 01:00,
# 1440 = Tuesday 00:00, 10079 = Sunday 23:59). A window "start,end" is half-open: it
# covers start..end-1 (10,12 covers minutes 10 and 11). A minute is deployable when
# it is inside at least one allowed window and inside no freeze window. Return the
# deployable windows as [[start, end], ...] sorted by start; adjacent or overlapping
# deployable minutes form one continuous window. part ("part1" / "part2") picks the
# input format.
#
# Part 1 - Allowed windows: every row is "start,end,type", type = allowed / freeze.
#   Allowed windows may overlap each other, freeze windows too. Return the week's
#   merged deployable windows.
# Part 2 - Time zones and minimum duration: the first row is
#   "utc_now,lead_time_minutes,min_continuous_minutes,k"; every other row is
#   "start,end,type,timezone_offset_minutes" in that row's local time, where
#   local = UTC + offset. Convert every window to UTC modulo the 10080-minute week and
#   split one that crosses the week boundary in two; all rows share one calendar.
#   Then a returned UTC window must start no earlier than utc_now + lead_time, last at
#   least min_continuous minutes, and end no later than 10080: clip each deployable
#   window's start to utc_now + lead_time, drop the ones that become too short, and
#   return the first k by start (fewer if fewer remain).
# Assumed: a local window with end < start wraps past the end of the week.
#
# Stacking notes:
# - A week has only 10080 minutes, so mark minutes with two difference arrays (allowed
#   count, freeze count) and scan once. One scan handles overlaps, subtraction and
#   merging, and a window that wraps is just two marks.
# - Keep run() as three steps: parse -> deployable -> select. Part 2 only changes
#   parse() and select(); the marking and scanning code is untouched.
# - Half-open everywhere: end - start is the length, no +1 / -1 anywhere.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

WEEK = 10080


# Add 1 over [start, start + length) to a difference array, wrapping past the week.
def mark(diff, start, length):
    end = start + length
    diff[start] += 1
    if end <= WEEK:
        diff[end] -= 1
    else:
        # split at the week boundary: [start, WEEK) and [0, end - WEEK)
        diff[WEEK] -= 1
        diff[0] += 1
        diff[end - WEEK] -= 1


# Part 1: allowed minus freeze over one week, merged into continuous windows.
# Each window is parsed as (start, length, type) so Part 2 can feed converted windows
# into the same marking code. Prefix sums of the two difference arrays tell, for each
# minute, how many allowed and freeze windows cover it.
class Part1:
    def run(self, input_csv):
        windows = self.parse(input_csv)
        deployable = self.deployable(windows)
        return self.select(deployable)

    # windows = [(utc_start, length, type)]
    def parse(self, input_csv):
        windows = []
        for line in input_csv:
            start, end, kind = line.split(",")
            length = int(end) - int(start)
            windows.append((int(start), length, kind))
        return windows

    # Mark every minute, then scan once. T: O(n + W) for W = 10080, S: O(W).
    def deployable(self, windows):
        # difference arrays: prefix sums give how many allowed / freeze windows cover m
        allowed = [0] * (WEEK + 1)
        frozen = [0] * (WEEK + 1)
        for start, length, kind in windows:
            if kind == "allowed":
                mark(allowed, start, length)
            else:
                mark(frozen, start, length)

        result = []
        allowed_now = 0
        frozen_now = 0
        for m in range(WEEK):
            allowed_now += allowed[m]
            frozen_now += frozen[m]
            if allowed_now == 0 or frozen_now > 0:
                continue
            # extend the last window if it ends right here, else start a new one
            if result and result[-1][1] == m:
                result[-1][1] = m + 1
            else:
                result.append([m, m + 1])
        return result

    # Part 1 returns every deployable window.
    def select(self, windows):
        return windows


# Part 2: rows are in local time and the answer has extra limits.
# parse() reads the header and shifts each window to UTC (utc = local - offset, mod
# the week); mark() already splits a window that wraps. select() clips each window's
# start to utc_now + lead, drops the short ones, and keeps the first k.
class Part2(Part1):
    def __init__(self):
        # first allowed start (utc_now + lead)
        self.earliest = 0
        # minimum length of a returned window
        self.min_len = 0
        # max number of windows returned
        self.k = 0

    def parse(self, input_csv):
        header = input_csv[0].split(",")
        utc_now = int(header[0])
        lead = int(header[1])
        self.min_len = int(header[2])
        self.k = int(header[3])
        self.earliest = utc_now + lead

        windows = []
        for line in input_csv[1:]:
            start, end, kind, offset = line.split(",")
            start = int(start)
            end = int(end)
            offset = int(offset)
            length = end - start
            if end < start:
                length += WEEK
            utc_start = (start - offset) % WEEK
            windows.append((utc_start, length, kind))
        return windows

    def select(self, windows):
        result = []
        for start, end in windows:
            start = max(start, self.earliest)
            if end <= start or end - start < self.min_len:
                continue
            result.append([start, end])
        return result[:self.k]


# The judged entry point: part picks the input format.
def schedule_deployment_windows(part, input_csv):
    if part == "part1":
        return Part1().run(input_csv)
    return Part2().run(input_csv)


def test(label, part, input_csv, want):
    print(label, "PASS" if part().run(input_csv) == want else "FAIL")


def main():
    test("p1-overlap", Part1, ["0,10,allowed", "5,20,allowed", "8,12,freeze"],
         [[0, 8], [12, 20]])
    test("p1-adjacent-merge", Part1, ["0,5,allowed", "5,9,allowed"], [[0, 9]])
    test("p1-freeze-only", Part1, ["0,5,freeze"], [])
    test("p2-offset-wrap", Part2, ["0,0,1,10", "30,90,allowed,60"],
         [[0, 30], [10050, 10080]])
    test("p2-lead-and-min", Part2,
         ["100,20,15,5", "0,200,allowed,0", "130,140,freeze,0"], [[140, 200]])
    test("p2-top-k", Part2,
         ["0,0,1,2", "0,1,allowed,0", "5,6,allowed,0", "9,10,allowed,0"],
         [[0, 1], [5, 6]])


main()
