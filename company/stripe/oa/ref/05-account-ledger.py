# Stripe OA: Financial Account Ledger (HackerRank, 4 parts).
# Function: process_ledger(commands: list[str]) -> str
#
# Input: commands in chronological order, one of
#   INIT account_id starting_balance
#   FUND timestamp account_id method amount     (method WIRE / ACH / STABLECOIN)
#   BALANCE timestamp account_id
# All INITs come first. Day 1 is Monday; days 6 and 7 of every seven-day cycle are
# Saturday and Sunday (business days are the other five).
# Output: every BALANCE result joined by commas ("" if there is no BALANCE).
# The source has 4 progressive parts but does not label where each one starts; the
# split below is inferred from its four examples.
#
# Part 1 - Accounts and settlement days (timestamps are whole days):
#   INIT creates an account with a settled starting balance; a repeated INIT of the
#   same account is ignored. FUND (positive amount) for an unknown account is ignored.
#   BALANCE appends starting balance + every earlier accepted fund whose settlement
#   day <= the query day, or FAILURE for an unknown account. Settlement is by calendar
#   day: a fund settling on day d counts for any later BALANCE on day d, never for a
#   command earlier in the input.
#   A weekend WIRE / ACH request moves to the following Monday (its effective day).
#   WIRE settles on its effective day, ACH one business day after it, STABLECOIN on its
#   request day, weekends included.
# Part 2 - Cutoff hours: a timestamp may also be "day,hour" (hour 0-23; a bare day
#   means hour 0). A business-day request at or after the cutoff (WIRE 17, ACH 20)
#   uses the next business day as its effective day; weekend requests still move to
#   Monday whatever the hour.
# Part 3 - ACH daily limit: per account, accepted ACH principal per effective day may
#   not exceed 5,000,000. A request that would exceed it is rejected whole: it never
#   settles and consumes no limit. WIRE and STABLECOIN are unlimited.
# Part 4 - ACH weekly limit: also at most 10,000,000 per account per week, where days
#   1-7 are week 1, 8-14 week 2, ... (by effective day). Exceeding either limit rejects
#   the request, and a rejected request consumes neither limit.
#
# Stacking notes:
# - Store accepted funds as (settle_day, amount) per account and sum them at BALANCE
#   time: a fund that settles later never needs a pending queue, and n <= 1000 keeps
#   the scan cheap.
# - Split fund() into small hooks from the start: parse_time() (Part 2 adds hours),
#   effective_day() (Part 2 adds cutoffs), reserve_ach() (Parts 3-4 add limits).
# - Compute the ACH effective day once and key both limits by it; a Saturday request
#   counts toward Monday's day and that Monday's week.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

from collections import defaultdict


def is_weekend(day):
    return (day - 1) % 7 >= 5


def next_business(day):
    day += 1
    while is_weekend(day):
        day += 1
    return day


# Part 1: accounts, funds and settlement days, with whole-day timestamps.
# A fund is stored with the day it settles; BALANCE adds up the ones settled by then.
# Only ACH needs an extra step (one business day after its effective day).
class Part1:
    def __init__(self):
        # start[account] = starting balance
        self.start = {}
        # funds[account] = [(settle_day, amount)] of accepted funds
        self.funds = defaultdict(list)
        # BALANCE results in query order
        self.out = []

    # BALANCE scans the account's funds. T: O(n^2) worst case (n <= 1000), S: O(n).
    def run(self, commands):
        for line in commands:
            parts = line.split()
            command = parts[0]

            if command == "INIT":
                account = parts[1]
                if account not in self.start:
                    self.start[account] = int(parts[2])
                continue

            day, hour = self.parse_time(parts[1])
            account = parts[2]
            if command == "FUND":
                self.fund(day, hour, account, parts[3], int(parts[4]))
            else:
                self.balance(day, account)
        return ",".join(self.out)

    # Returns (day, hour); Part 1 timestamps are whole days.
    def parse_time(self, token):
        return int(token), 0

    def fund(self, day, hour, account, method, amount):
        if account not in self.start:
            return

        if method == "STABLECOIN":
            settle = day

        elif method == "WIRE":
            settle = self.effective_day(day, hour, method)

        else:
            eff = self.effective_day(day, hour, method)
            # a rejected ACH never settles
            if not self.reserve_ach(account, eff, amount):
                return
            settle = next_business(eff)

        self.funds[account].append((settle, amount))

    def balance(self, day, account):
        if account not in self.start:
            self.out.append("FAILURE")
            return
        settled = 0
        for settle_day, amount in self.funds[account]:
            if settle_day <= day:
                settled += amount
        self.out.append(str(self.start[account] + settled))

    # Part 1: no cutoffs, a weekend request moves to Monday.
    def effective_day(self, day, hour, method):
        if is_weekend(day):
            return next_business(day)
        return day

    # Record an ACH against the limits; False = rejected. Part 1 has no limits.
    def reserve_ach(self, account, eff, amount):
        return True


# Part 2: timestamps may carry an hour ("day,hour"). Only parsing and the effective
# day change: a business-day request at or after its method's cutoff hour moves to the
# next business day, exactly like a weekend request.
class Part2(Part1):
    # CUTOFF[method] = first hour that moves a business-day request to the next one
    CUTOFF = {"WIRE": 17, "ACH": 20}

    def parse_time(self, token):
        day, _, hour = token.partition(",")
        if hour == "":
            return int(day), 0
        return int(day), int(hour)

    def effective_day(self, day, hour, method):
        if is_weekend(day) or hour >= self.CUTOFF[method]:
            return next_business(day)
        return day


# Part 3: ACH daily limit. Keep a running total per (account, effective day); check the
# new total before recording it, so a rejected request consumes nothing.
class Part3(Part2):
    DAY_LIMIT = 5_000_000

    def __init__(self):
        super().__init__()
        # ach_day[(account, effective_day)] = accepted ACH principal
        self.ach_day = defaultdict(int)

    def reserve_ach(self, account, eff, amount):
        key = (account, eff)
        if self.ach_day[key] + amount > self.DAY_LIMIT:
            return False
        self.ach_day[key] += amount
        return True


# Part 4: ACH weekly limit on top of the daily one. Check the week first, then let
# Part 3 check and record the day, and only then record the week: a request rejected
# by either limit consumes neither.
class Part4(Part3):
    WEEK_LIMIT = 10_000_000

    def __init__(self):
        super().__init__()
        # ach_week[(account, week)] = accepted ACH principal, week = (eff - 1) // 7
        self.ach_week = defaultdict(int)

    def reserve_ach(self, account, eff, amount):
        key = (account, (eff - 1) // 7)
        if self.ach_week[key] + amount > self.WEEK_LIMIT:
            return False
        if not super().reserve_ach(account, eff, amount):
            return False
        self.ach_week[key] += amount
        return True


# The judged entry point: the last part passes every part's tests.
def process_ledger(commands):
    return Part4().run(commands)


def test(label, part, commands, want):
    print(label, "PASS" if part().run(commands) == want else "FAIL")


def main():
    test("p1-settle-days", Part1,
         ["INIT a 100", "FUND 1 a WIRE 10", "FUND 1 a ACH 20", "FUND 1 a STABLECOIN 5",
          "BALANCE 1 a", "BALANCE 2 a"], "115,135")
    test("p1-weekend", Part1,
         ["INIT a 0", "FUND 5 a ACH 8", "FUND 6 a STABLECOIN 1", "FUND 6 a WIRE 2",
          "FUND 7 a ACH 4", "BALANCE 7 a", "BALANCE 8 a", "BALANCE 9 a"], "1,11,15")
    test("p1-unknown", Part1,
         ["INIT a 5", "INIT a 9", "FUND 1 b WIRE 3", "BALANCE 1 b", "BALANCE 1 a"],
         "FAILURE,5")
    test("p1-no-query", Part1, ["INIT a 1"], "")
    test("p2-cutoffs", Part2,
         ["INIT a 0", "FUND 1,16 a WIRE 10", "FUND 1,17 a WIRE 20", "FUND 1,19 a ACH 5",
          "FUND 1,20 a ACH 7", "BALANCE 1,23 a", "BALANCE 2,0 a", "BALANCE 3,0 a",
          "FUND 5,18 a WIRE 100", "BALANCE 7,0 a", "BALANCE 8,0 a"],
         "10,35,42,42,142")
    test("p3-day-limit", Part3,
         ["INIT a 0", "INIT b 0", "FUND 1 a ACH 5000000", "FUND 1 a ACH 1",
          "FUND 1,21 a ACH 1", "FUND 1 b ACH 5000000", "BALANCE 3 a", "BALANCE 3 b"],
         "5000001,5000000")
    test("p4-week-limit", Part4,
         ["INIT a 0", "FUND 1 a ACH 5000000", "FUND 2 a ACH 5000000", "FUND 3 a ACH 1",
          "FUND 7 a ACH 1", "BALANCE 9 a"], "10000001")


main()
