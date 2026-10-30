# Stripe OA: Incident Monitor (HackerRank, 3 parts).
# Function: detect_incidents(logs: list[str]) -> list[str]
#
# Input: logs[i] = "timestamp,merchant_id,status_code,count", sorted by timestamp, no
# duplicate (timestamp, merchant, code). timestamp is in seconds; status_code is 200
# (success), 4xx or 5xx (errors); count = how many such outcomes in that second.
# After each log at T, evaluate that log's merchant over the inclusive window
# [T-29, T]. An alert belongs to one (merchant_id, error status_code) pair, so a merchant
# can have several active alerts at once. While an alert is active, never TRIGGER it
# again (de-duplication).
# Output: "timestamp,event_type,merchant_id,status_code" with event_type TRIGGER or
# RESOLVE, sorted by (timestamp, merchant_id, status_code, event_type) ascending
# (RESOLVE sorts before TRIGGER).
#
# Part 1 - Basic detection: TRIGGER when the window holds >= 5 failures with that
#   exact error code for that merchant. Once triggered it stays active (no resolution
#   yet).
# Part 2 - 1% impact rule: TRIGGER only if the window has >= 5 failures of the code
#   AND those failures are strictly more than 1% of the merchant's 200 count in the
#   same window (6 errors vs 600 successes is exactly 1% -> no alert).
# Part 3 - Resolution: when a log arrives for the merchant at T and an active alert's
#   Part 2 condition no longer holds in [T-29, T], emit RESOLVE at T and deactivate it.
#   It may TRIGGER again later when the condition holds again.
#
# Stacking notes:
# - Keep the window per merchant as a deque plus running totals per code, not a
#   re-scan of the logs. Keep every code's total (including 200 and codes that dropped
#   to 0): Part 2 needs the 200 total and Part 3 needs the codes that fell back.
# - Evaluate every code of the merchant after each log, not only the log's code: a 200
#   log can change the outcome for all of that merchant's error codes.
# - Collect events as tuples (T, merchant, code, type) and sort once at the end; the
#   required output order is exactly tuple order.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

from collections import defaultdict, deque


# Part 1: per merchant, a deque of its logs inside the last 30 seconds and the running
# count per status code. After each log, drop the merchant's expired logs, then check
# every error code: holds() is the alert condition, release() is what happens when an
# active alert stops holding (nothing yet in Part 1).
class Part1:
    WINDOW = 30

    def __init__(self):
        # window[merchant] = deque of (timestamp, code, count) in its last 30 seconds
        self.window = {}
        # totals[merchant][code] = sum of counts in the merchant's window
        self.totals = {}
        # (merchant, code) pairs with an active (not resolved) alert
        self.active = set()
        # alert events as (timestamp, merchant, code, type)
        self.events = []

    # Each log enters and leaves its deque once; each evaluation scans the merchant's
    # codes. T: O(n * c + E log E) for c codes per merchant and E events, S: O(n).
    def run(self, logs):
        for line in logs:
            t, merchant, code, count = line.split(",")
            t = int(t)
            self.add(t, merchant, int(code), int(count))
            self.evaluate(t, merchant)
        self.events.sort()
        return [f"{t},{kind},{m},{c}" for t, m, c, kind in self.events]

    # Push the log and drop the merchant's logs older than t - 29.
    def add(self, t, merchant, code, count):
        if merchant not in self.window:
            self.window[merchant] = deque()
            self.totals[merchant] = defaultdict(int)
        q = self.window[merchant]
        total = self.totals[merchant]

        q.append((t, code, count))
        total[code] += count
        while q[0][0] <= t - self.WINDOW:
            _, old_code, old_count = q.popleft()
            total[old_code] -= old_count

    def evaluate(self, t, merchant):
        total = self.totals[merchant]
        success = total.get(200, 0)
        for code, failures in total.items():
            if code == 200:
                continue
            key = (merchant, code)
            holds = self.holds(failures, success)

            if holds and key not in self.active:
                self.active.add(key)
                self.events.append((t, merchant, code, "TRIGGER"))

            elif not holds and key in self.active:
                self.release(t, key)

    def holds(self, failures, success):
        return failures >= 5

    # Part 1 never resolves an alert.
    def release(self, t, key):
        pass


# Part 2: the 1% impact rule only changes the alert condition. Compare in integers:
# failures > 1% of success  <=>  failures * 100 > success (no float division).
class Part2(Part1):
    def holds(self, failures, success):
        return failures >= 5 and failures * 100 > success


# Part 3: an active alert that stops holding is resolved: deactivate it (so it can
# trigger again later) and record a RESOLVE event at this log's time.
class Part3(Part2):
    def release(self, t, key):
        self.active.remove(key)
        merchant, code = key
        self.events.append((t, merchant, code, "RESOLVE"))


# The judged entry point: the last part passes every part's tests.
def detect_incidents(logs):
    return Part3().run(logs)


def test(label, part, logs, want):
    print(label, "PASS" if part().run(logs) == want else "FAIL")


def main():
    test("p1-window-dedup", Part1, ["1,m,500,4", "30,m,500,1", "60,m,500,9"],
         ["30,TRIGGER,m,500"])
    test("p1-per-code-sorted", Part1,
         ["1,b,500,5", "1,a,500,5", "1,a,404,5", "1,a,429,4", "2,a,404,3"],
         ["1,TRIGGER,a,404", "1,TRIGGER,a,500", "1,TRIGGER,b,500"])
    test("p1-ignores-success", Part1, ["1,m,200,1000", "2,m,500,5"],
         ["2,TRIGGER,m,500"])
    test("p2-impact-strict", Part2,
         ["1,a,200,500", "1,b,200,499", "2,a,500,5", "2,b,500,5"],
         ["2,TRIGGER,b,500"])
    test("p3-dedup-resolve", Part3, ["1,m,500,5", "2,m,500,1", "3,m,200,1000"],
         ["1,TRIGGER,m,500", "3,RESOLVE,m,500"])
    test("p3-expire-retrigger", Part3, ["1,m,500,5", "31,m,200,1", "32,m,500,5"],
         ["1,TRIGGER,m,500", "31,RESOLVE,m,500", "32,TRIGGER,m,500"])


main()
