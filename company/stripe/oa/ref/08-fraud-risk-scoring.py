# Stripe OA: Merchant Fraud Risk Scoring (HackerRank, 3 passes).
# Function: calculate_merchant_fraud_scores(transactions, merchant_ids, base_scores,
#                                           rules) -> list[int]
#
# Input: one day of transactions.
#   transactions[i] = [customer_id, merchant_id, hour, amount] (hour, amount are strings)
#   merchant_ids[j] starts with score base_scores[j] (unique ids; every transaction's
#   merchant is listed).
#   rules[i] = [min_transaction_amount, multiplicative_factor, additive_factor, penalty]
#   belongs to transactions[i].
# Output: the final scores in merchant_ids order.
# The statement is one task, not labeled parts: it defines three passes, run in this
# order, each scanning all transactions left to right. They are presented as parts
# here because each pass builds on the scores of the previous one.
#
# Part 1 - Amount pass: if amount > its rule's min_transaction_amount (strictly),
#   multiply the merchant's current score by the rule's multiplicative_factor.
# Part 2 - Repeat-customer pass: group transactions by (customer_id, merchant_id).
#   When a group reaches its 3rd transaction, add the additive_factor of its 1st, 2nd
#   and 3rd transactions to the merchant's score; every later transaction of the group
#   adds only its own additive_factor. Groups with fewer than 3 add nothing.
# Part 3 - Same-hour pass: group by (customer_id, merchant_id, hour) with the same
#   "from the 3rd" rule on penalty values. Hours 12-17 add the penalty, hours 9-11 and
#   18-21 subtract it, any other hour leaves the score unchanged.
#
# Stacking notes:
# - Keep the score in a dict merchant -> score; every pass reads the transactions with
#   their rules and updates that dict.
# - Make the run a chain of passes (passes() calls super().passes() first), so each
#   part appends one pass and the passes keep the statement's order.
# - Python ints never overflow; in C++ / Java keep scores in 64-bit.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

from collections import defaultdict


# +1 for hours 12-17, -1 for 9-11 and 18-21, 0 otherwise.
def hour_sign(hour):
    if 12 <= hour <= 17:
        return 1
    if 9 <= hour <= 11 or 18 <= hour <= 21:
        return -1
    return 0


# Part 1: the amount pass. One scan; a transaction above its rule's threshold
# multiplies its merchant's score.
class Part1:
    def __init__(self):
        # score[merchant] = current score
        self.score = {}

    # Each pass is linear. T: O(n + m) for n transactions and m merchants, S: O(n + m).
    def run(self, transactions, merchant_ids, base_scores, rules):
        for i in range(len(merchant_ids)):
            self.score[merchant_ids[i]] = base_scores[i]
        self.passes(transactions, rules)

        out = []
        for merchant in merchant_ids:
            out.append(self.score[merchant])
        return out

    def passes(self, transactions, rules):
        for i in range(len(transactions)):
            customer, merchant, hour, amount = transactions[i]
            min_amount = rules[i][0]
            factor = rules[i][1]
            if int(amount) > min_amount:
                self.score[merchant] *= factor


# Part 2: the repeat-customer pass, run after the amount pass.
# Per (customer, merchant) group, the first two additives are held back; the third
# transaction releases all three, and every later one applies its own right away.
# The rule is written once as add_from_third() so Part 3 can reuse it.
class Part2(Part1):
    def passes(self, transactions, rules):
        super().passes(transactions, rules)

        # items = [(group key, merchant, delta)] in transaction order
        items = []
        for i in range(len(transactions)):
            customer, merchant, hour, amount = transactions[i]
            additive = rules[i][2]
            items.append(((customer, merchant), merchant, additive))
        self.add_from_third(items)

    def add_from_third(self, items):
        # count[key] = transactions seen in the group so far
        count = defaultdict(int)
        # held[key] = deltas of the group's first two transactions, not applied yet
        held = defaultdict(int)
        for key, merchant, delta in items:
            count[key] += 1
            if count[key] < 3:
                held[key] += delta
            else:
                self.score[merchant] += held[key] + delta
                held[key] = 0


# Part 3: the same-hour pass. Same "from the third" rule, but grouped by
# (customer, merchant, hour), and the penalty's sign depends on the hour.
class Part3(Part2):
    def passes(self, transactions, rules):
        super().passes(transactions, rules)

        # items = [(group key, merchant, signed penalty)] in transaction order
        items = []
        for i in range(len(transactions)):
            customer, merchant, hour, amount = transactions[i]
            penalty = rules[i][3]
            delta = hour_sign(int(hour)) * penalty
            items.append(((customer, merchant, hour), merchant, delta))
        self.add_from_third(items)


# The judged entry point: the last part passes every part's tests.
def calculate_merchant_fraud_scores(transactions, merchant_ids, base_scores, rules):
    return Part3().run(transactions, merchant_ids, base_scores, rules)


def test(label, part, transactions, merchant_ids, base_scores, rules, want):
    got = part().run(transactions, merchant_ids, base_scores, rules)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("p1-amount-strict", Part1, [["c", "m", "0", "10"], ["c", "m", "0", "11"]],
         ["m"], [1], [[10, 3, 0, 0], [10, 2, 0, 0]], [2])
    test("p1-merchant-order", Part1, [["c", "x", "0", "5"]], ["y", "x"], [1, 2],
         [[4, 5, 0, 0]], [1, 10])
    test("p2-repeat", Part2,
         [["a", "m", "12", "0"], ["b", "m", "12", "0"], ["a", "m", "12", "0"],
          ["b", "m", "12", "0"], ["a", "m", "12", "0"], ["a", "m", "12", "0"]],
         ["m"], [0], [[9, 1, 1, 7], [9, 1, 100, 7], [9, 1, 2, 7], [9, 1, 100, 7],
                      [9, 1, 3, 7], [9, 1, 4, 7]], [10])
    test("p3-hour-sign", Part3,
         [["a", "m", "12", "0"]] * 3 + [["b", "m", "21", "0"]] * 3
         + [["c", "m", "22", "0"]] * 3, ["m"], [10],
         [[9, 1, 0, 1]] * 3 + [[9, 1, 0, 2]] * 3 + [[9, 1, 0, 5]] * 3, [7])
    test("p3-hour-groups", Part3,
         [["a", "m", "13", "0"], ["a", "m", "13", "0"], ["a", "m", "14", "0"]],
         ["m"], [0], [[9, 1, 0, 1]] * 3, [0])


main()
