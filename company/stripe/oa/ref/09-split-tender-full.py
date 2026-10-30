# Stripe OA: Split Tender Payments with Expiration and Refunds (HackerRank, 4 parts).
# Function: process_events(events: list[str], surcharge_rates: list[str]) -> list[str]
#
# Input: events in time order (nondecreasing timestamps, equal ones keep input order),
# one CSV line each, every amount a non-negative integer number of cents:
#   timestamp,ADD_GIFTCARD,customer_id,card_id,amount,valid_before
#   timestamp,CHARGE,charge_id,customer_id,amount,zip_code
#   timestamp,REFUND,charge_id,refund_amount
# surcharge_rates: "start_zip-end_zip:rate", e.g. "90000-99999:0.05"; inclusive,
# non-overlapping ranges of five-digit zips. Card and charge ids are unique; a refund
# names an earlier charge, at most once per charge.
# Output: one string per event, in input order:
#   ADD_GIFTCARD -> ""
#   CHARGE -> "CC_AMOUNT" then ",CARD_ID:AMOUNT_USED" for each card it took money from
#             (amount > 0), sorted by card id; just "CC_AMOUNT" if no card was used
#   REFUND -> "CC_REFUND,GC_REFUND"
#
# Part 1 - Basic split tendering: ADD_GIFTCARD gives the customer a card with that
#   balance. CHARGE takes from the customer's gift card first; whatever is left goes on
#   the credit card. Each customer has at most one active card at a time.
# Part 2 - Expiration and priority: a customer may hold several cards. At charge time T
#   a card is usable only if T < valid_before; at or after that its balance is voided
#   for good. Spend usable cards by ascending valid_before, ties by card id, draining
#   one card completely before the next. (Output ids are still sorted by id.)
# Part 3 - Regional surcharge: after the gift cards, the credit card pays
#   rest + rest * rate, where rate comes from the range containing the charge's zip
#   (0 if none). rest * rate is always a whole number of cents.
# Part 4 - Refunds: refund_amount <= what the charge cost in total. Return it to the
#   credit card first, up to the credit-card amount actually charged (incl. surcharge);
#   any remainder becomes a new never-expiring card "REFUND_<charge_id>" for the same
#   customer. Never-expiring cards are spent after every usable finite card.
#
# Stacking notes:
# - Store each customer's cards as a heap of (key, card_id) from Part 1, even though
#   Part 1 has one card per customer. Later parts only change the key and add cards.
# - Credit-card money always goes through surcharge(); it returns 0 until Part 3.
# - Record each charge's customer and credit-card amount when it happens; Part 4's
#   refund event only carries the charge id.
# - Integer cents everywhere; the rate is a Fraction, so rest * rate is exact.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

import heapq
from bisect import bisect_right
from collections import defaultdict
from fractions import Fraction

NEVER = float("inf")


# Part 1: gift card first, credit card for the rest.
# Cards live in a per-customer min-heap of (key, card_id) with balances in a separate
# dict. A drained card is popped when it reaches the top (lazy removal), so each card
# is pushed and popped once. Part 1 uses key 0, so the heap order is just card id.
class Part1:
    def __init__(self):
        # cards[customer] = min-heap of (key, card_id)
        self.cards = defaultdict(list)
        # balance[card_id] = remaining cents
        self.balance = {}
        # charges[charge_id] = (customer, cents put on the credit card incl. surcharge)
        self.charges = {}

    # T: O(n log n) for n events, S: O(n).
    def run(self, events, surcharge_rates):
        out = []
        for line in events:
            out.append(self.handle(line.split(",")))
        return out

    def handle(self, f):
        t = int(f[0])
        if f[1] == "ADD_GIFTCARD":
            self.add_card(f[2], f[3], int(f[4]), int(f[5]))
            return ""
        return self.charge(t, f[2], f[3], int(f[4]), f[5])

    def add_card(self, customer, card, amount, valid_before):
        self.balance[card] = amount
        heapq.heappush(self.cards[customer], (self.key(valid_before), card))

    def key(self, valid_before):
        return 0

    def spendable(self, t, key, card):
        return self.balance[card] > 0

    def charge(self, t, charge, customer, rest, zip_code):
        heap = self.cards[customer]
        # used[card_id] = cents this charge took from that card
        used = {}
        while rest > 0 and heap:
            key, card = heap[0]
            # an empty (or, from Part 2, expired) card is gone for good
            if not self.spendable(t, key, card):
                heapq.heappop(heap)
                continue
            take = min(self.balance[card], rest)
            self.balance[card] -= take
            rest -= take
            used[card] = take

        cc = rest + self.surcharge(rest, zip_code)
        self.charges[charge] = (customer, cc)

        # cards are printed by id, not in spending order
        pieces = [str(cc)]
        for card in sorted(used):
            pieces.append(f"{card}:{used[card]}")
        return ",".join(pieces)

    def surcharge(self, principal, zip_code):
        return 0


# Part 2: several cards per customer, with expiration.
# The heap key becomes valid_before, so the heap already spends the earliest-expiring
# card first and breaks ties by card id. A card at or past valid_before is treated like
# an empty one: popped and never used again.
class Part2(Part1):
    def key(self, valid_before):
        return valid_before

    def spendable(self, t, key, card):
        if t >= key:
            return False
        return super().spendable(t, key, card)


# Part 3: regional surcharge on the credit-card part.
# Sort the zip ranges once; for each charge, binary-search the last range starting at or
# before the zip and check that the zip is inside it.
class Part3(Part2):
    def __init__(self):
        super().__init__()
        # ranges = [(start_zip, end_zip, exact rate)] sorted by start
        self.ranges = []
        # starts[i] = ranges[i][0], for binary search
        self.starts = []

    # T: O((n + m) log(n + m)) for n events and m ranges, S: O(n + m).
    def run(self, events, surcharge_rates):
        for line in surcharge_rates:
            zips, rate = line.split(":")
            lo, hi = zips.split("-")
            self.ranges.append((int(lo), int(hi), Fraction(rate)))
        self.ranges.sort()
        for r in self.ranges:
            self.starts.append(r[0])
        return super().run(events, surcharge_rates)

    def surcharge(self, principal, zip_code):
        z = int(zip_code)
        i = bisect_right(self.starts, z) - 1
        if i < 0:
            return 0
        start, end, rate = self.ranges[i]
        if z > end:
            return 0
        return int(principal * rate)


# Part 4: refunds. Pay back the credit card first, up to what it was charged; the rest
# becomes a new card that never expires (key = inf, so it is spent after every finite
# card).
class Part4(Part3):
    def handle(self, f):
        if f[1] != "REFUND":
            return super().handle(f)
        charge = f[2]
        amount = int(f[3])
        customer, cc = self.charges[charge]
        cc_refund = min(amount, cc)
        gc_refund = amount - cc_refund
        if gc_refund > 0:
            self.add_card(customer, "REFUND_" + charge, gc_refund, NEVER)
        return f"{cc_refund},{gc_refund}"


# The judged entry point: the last part passes every part's tests.
def process_events(events, surcharge_rates):
    return Part4().run(events, surcharge_rates)


def test(label, part, events, rates, want):
    print(label, "PASS" if part().run(events, rates) == want else "FAIL")


def main():
    test("p1-basic", Part1,
         ["1,ADD_GIFTCARD,u,g,50,99", "2,CHARGE,c1,u,30,00000", "3,CHARGE,c2,u,30,00000",
          "4,CHARGE,c3,u,5,00000", "5,ADD_GIFTCARD,u,h,7,99", "6,CHARGE,c4,u,3,00000",
          "7,CHARGE,c5,v,4,00000"], [],
         ["", "0,g:30", "10,g:20", "5", "", "0,h:3", "4"])
    test("p2-order-and-sort", Part2,
         ["1,ADD_GIFTCARD,u,gb,4,5", "1,ADD_GIFTCARD,u,ga,10,9",
          "2,CHARGE,c1,u,7,00000"], [], ["", "", "0,ga:3,gb:4"])
    test("p2-tie-and-expire", Part2,
         ["1,ADD_GIFTCARD,u,g2,5,10", "1,ADD_GIFTCARD,u,g1,5,10",
          "2,CHARGE,c1,u,3,00000", "10,CHARGE,c2,u,4,00000"], [],
         ["", "", "0,g1:3", "4"])
    test("p3-surcharge", Part3,
         ["1,ADD_GIFTCARD,u,g,100,50", "2,CHARGE,c1,u,300,15000",
          "3,CHARGE,c2,u,50,20000", "4,CHARGE,c3,u,10,19999"],
         ["10000-19999:0.1"], ["", "220,g:100", "50", "11"])
    test("p4-refund-card", Part4,
         ["1,ADD_GIFTCARD,u,g,100,50", "2,CHARGE,c1,u,300,15000", "3,REFUND,c1,250",
          "4,ADD_GIFTCARD,u,h,5,99", "5,CHARGE,c2,u,40,20000"],
         ["10000-19999:0.1"], ["", "220,g:100", "220,30", "", "5,REFUND_c1:30,h:5"])
    test("p4-refund-cc-only", Part4, ["1,CHARGE,c1,u,100,00000", "2,REFUND,c1,60"], [],
         ["100", "60,0"])


main()
