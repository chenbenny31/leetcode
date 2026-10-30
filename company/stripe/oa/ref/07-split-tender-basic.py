# Stripe OA: Split Tender Transactions Part 1 - Basic Split Tendering (HackerRank).
# Function: process_events(events: list[str]) -> list[str]
#
# Input: events in chronological order (equal timestamps keep input order), one CSV
# line each. Amounts are non-negative decimals with at most 2 decimal places:
#   timestamp,ADD_GIFTCARD,customer_id,card_id,amount,valid_before
#   timestamp,CHARGE,charge_id,customer_id,amount,zip_code
# Card ids and charge ids are unique. valid_before and zip_code are present but unused.
# Output: one string per event, in input order:
#   ADD_GIFTCARD -> ""
#   CHARGE -> "CC_AMOUNT" then ",CARD_ID:AMOUNT_USED" for each card debited; just
#             "CC_AMOUNT" if no card was debited. Every amount printed with exactly two
#             decimals ("10.00", "0.00").
#
# Part 1 - Basic split tendering (the only judged part here): every customer's gift card
#   balance starts at 0. ADD_GIFTCARD adds the amount to that customer's card. CHARGE
#   takes from the customer's gift card balance first: if it covers the amount, the
#   credit card pays 0; otherwise the card is drained to 0 and the rest goes on the
#   credit card. Each customer holds at most one active card at a time.
#
# Later parts (not judged in this variant; solved in 09-split-tender-full, in cents):
#   Part 2 expires cards (usable only while T < valid_before) and spends several cards by
#   (valid_before, card_id); Part 3 adds a zip-range surcharge to the credit-card part;
#   Part 4 refunds to the credit card first and turns the rest into a new
#   never-expiring card "REFUND_<charge_id>".
#
# Stacking notes:
# - Keep a per-customer list of [card_id, balance] even though Part 1 has one card: the
#   output needs the card id, and Part 2 needs several cards per customer. Only the
#   order you drain them in changes later (09 swaps the deque for a heap).
# - Convert money to integer cents on input and back to "x.yy" on output, so later
#   surcharges and refunds stay exact; never add floats.
# - Pop a card once it is drained instead of filtering the list every charge.

from collections import defaultdict, deque


# "12.5" -> 1250: pad the fraction to two digits instead of going through float.
def to_cents(s):
    whole, _, frac = s.partition(".")
    frac = (frac + "00")[:2]
    return int(whole) * 100 + int(frac)


def money(cents):
    return f"{cents // 100}.{cents % 100:02d}"


# Part 1 (the only part here): each customer has a queue of gift cards. A charge
# takes from the front card until the amount is covered or the cards run out; the rest
# goes on the credit card. A drained card is popped so it is never looked at again.
class Part1:
    def __init__(self):
        # cards[customer] = deque of [card_id, balance_cents] in add order
        self.cards = defaultdict(deque)

    # Each card is drained and popped once. T: O(n), S: O(n).
    def run(self, events):
        out = []
        for line in events:
            out.append(self.handle(line.split(",")))
        return out

    def handle(self, f):
        if f[1] == "ADD_GIFTCARD":
            customer = f[2]
            self.cards[customer].append([f[3], to_cents(f[4])])
            return ""

        customer = f[3]
        queue = self.cards[customer]
        rest = to_cents(f[4])
        # "card:amount" entries for the cards this charge took money from
        used = []
        while rest > 0 and queue:
            card = queue[0]
            take = min(card[1], rest)
            card[1] -= take
            rest -= take
            # a card added with balance 0 is popped without being listed
            if take > 0:
                used.append(f"{card[0]}:{money(take)}")
            if card[1] == 0:
                queue.popleft()

        result = [money(rest)]
        result.extend(used)
        return ",".join(result)


# The judged entry point.
def process_events(events):
    return Part1().run(events)


def test(label, events, want):
    print(label, "PASS" if Part1().run(events) == want else "FAIL")


def main():
    test("p1-no-card", ["1,CHARGE,c1,u1,3.10,10000"], ["3.10"])
    test("p1-covered-then-short",
         ["1,ADD_GIFTCARD,u1,g1,5,99", "2,CHARGE,c1,u1,1.25,10000",
          "3,CHARGE,c2,u1,4,10000"],
         ["", "0.00,g1:1.25", "0.25,g1:3.75"])
    test("p1-drained-then-new",
         ["1,ADD_GIFTCARD,u1,g1,1.5,99", "2,CHARGE,c1,u1,1.5,1", "3,CHARGE,c2,u1,2,1",
          "4,ADD_GIFTCARD,u1,g2,0.5,99", "5,CHARGE,c3,u1,1,1"],
         ["", "0.00,g1:1.50", "2.00", "", "0.50,g2:0.50"])
    test("p1-other-customer", ["1,ADD_GIFTCARD,u1,g1,2,9", "2,CHARGE,c1,u2,1,1"],
         ["", "1.00"])


main()
