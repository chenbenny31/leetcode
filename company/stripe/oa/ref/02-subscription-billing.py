# Stripe OA: Subscription and Usage-Based Billing Calculator (HackerRank, 5 parts).
# Function: calculate_monthly_charges(subscriptions, changes, pricing, usage)
#   -> list[list[str]]
# Source: Parts 1-2 are the original text; only the Part 3 heading was visible, and
# the Part 3-5 rules below are FastPrep-authored practice built on the same model.
#
# General rules: money in USD cents; a billing cycle is exactly 30 days (days 1..30);
# subscription ids are unique; a user may have several subscriptions, or usage with no
# subscription. Charges are floored to the cent only once, right before charging:
# floor(1066.666 + 2066.666) = 3133, never floor each piece. Output rows are
# [user_id, total_as_string] (e.g. ["user_1", "3500"]), sorted by user id.
#
# Part 1 - Monthly subscriptions: subscriptions[i] = "user_id,subscription_id,
#   monthly_cost". A user's total is the sum of monthly_cost over their subscriptions.
# Part 2 - Mid-month changes: changes[i] = "user_id,old_sub,new_sub,new_monthly_cost,
#   change_day". On change_day the old version stops (active up to, not including, that
#   day) and new_sub starts. A version contributes monthly_cost * days_active / 30,
#   where days_active = change_day - start_day, or 31 - start_day if it runs to the end
#   of the cycle. Initial subscriptions start on day 1. Changes may come in any order;
#   a later change may replace an id that an earlier change created (chains); days
#   strictly increase along a chain. Practice convention: old_sub "-" adds new_sub
#   without replacing anything. Every new id is globally fresh.
# Part 3 - Flat usage: usage[i] = "user_id,product_id,quantity". Sum quantities per
#   (user, product) first. pricing[i] = "product_id,-1,unit_price" (one row per
#   product); usage charge = total quantity * unit_price.
# Part 4 - Marginal tiers: pricing rows "product_id,upper_bound,unit_price", unordered;
#   finite bounds are distinct cumulative quantities and exactly one row per product
#   has bound -1 (no limit). Sorted, the first tier prices units 1..bound1, the next
#   units bound1+1..bound2, ..., the unlimited tier the rest. Each unit pays its own
#   tier's price (no retroactive repricing). Tiers apply per (user, product) to the
#   aggregated quantity, never restarting per usage row.
# Part 5 - Combined invoice: every user mentioned in subscriptions, changes or usage
#   gets one row = all prorated subscription versions + all usage charges, floored
#   once; users with total 0 are included; rows sorted by user id; [] if no users.
#
# Stacking notes:
# - Keep every total multiplied by 30 from Part 1 on, even though Part 1 has no
#   fractions. Proration then stays an exact integer, and the floor-once rule is a
#   single "// 30" at the very end - no float, no per-piece rounding to undo.
# - Leave empty hooks in run() for changes and usage, so later parts fill one method
#   each and the output / sorting code is written once.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

from collections import defaultdict


# Part 1: sum each user's monthly costs.
# Store each running subscription as (user, cost, start_day) and charge it only when it
# ends; at the end of the cycle every survivor is charged for 31 - start days. In Part 1
# every start is day 1, so that is cost * 30 / 30 - the plain sum.
class Part1:
    def __init__(self):
        # total30[user] = 30 * exact charge in cents
        self.total30 = defaultdict(int)
        # active[sub] = (user, monthly_cost, start_day) of versions still running
        self.active = {}

    # T: O(n log n) (sorting users), S: O(n) for n input rows.
    def run(self, subscriptions, changes, pricing, usage):
        for line in subscriptions:
            user, sub, cost = line.split(",")
            self.active[sub] = (user, int(cost), 1)

        self.apply_changes(changes)

        # every version still running lasts through day 30
        for user, cost, start in self.active.values():
            self.total30[user] += cost * (31 - start)

        self.add_usage(pricing, usage)

        result = []
        for user in sorted(self.total30):
            result.append([user, str(self.total30[user] // 30)])
        return result

    # Part 1 has no changes and no usage.
    def apply_changes(self, changes):
        pass

    def add_usage(self, pricing, usage):
        pass


# Part 2: mid-month changes. A change ends the old version (charge it for
# change_day - start days) and starts the new one on change_day.
# Changes come unordered, so sort them by day first: along one chain the days strictly
# increase and different chains never touch, so day order replays every chain.
class Part2(Part1):
    # T: O(c log c) for c changes.
    def apply_changes(self, changes):
        rows = [line.split(",") for line in changes]
        rows.sort(key=lambda row: int(row[4]))
        for user, old, new, cost, day in rows:
            day = int(day)
            # "-" adds a subscription without replacing one
            if old != "-":
                _, old_cost, start = self.active.pop(old)
                self.total30[user] += old_cost * (day - start)
            self.active[new] = (user, int(cost), day)


# Part 3: flat-price usage. Sum quantities per (user, product) first, then price each
# total once - Part 4's tiers need the aggregated quantity, never one row at a time.
# Pricing is split into load_pricing() / usage_charge() so Part 4 swaps only those two.
class Part3(Part2):
    def __init__(self):
        super().__init__()
        # price[product] = flat unit price
        self.price = {}

    # T: O(u + p) for u usage rows and p pricing rows.
    def add_usage(self, pricing, usage):
        self.load_pricing(pricing)

        # qty[(user, product)] = total quantity used
        qty = defaultdict(int)
        for line in usage:
            user, product, q = line.split(",")
            qty[(user, product)] += int(q)

        for (user, product), q in qty.items():
            self.total30[user] += 30 * self.usage_charge(product, q)

    def load_pricing(self, pricing):
        for line in pricing:
            product, _, price = line.split(",")
            self.price[product] = int(price)

    def usage_charge(self, product, qty):
        return qty * self.price.get(product, 0)


# Part 4: marginal tiers. Sort each product's tiers by upper bound (the unlimited tier
# becomes bound = inf, so it sorts last), then charge the units of each tier at that
# tier's price: tier i covers units prev_bound + 1 .. bound.
class Part4(Part3):
    def __init__(self):
        super().__init__()
        # tiers[product] = [(upper_bound, unit_price)] sorted, unlimited (inf) last
        self.tiers = defaultdict(list)

    # T: O(p log p) for sorting the tiers.
    def load_pricing(self, pricing):
        for line in pricing:
            product, bound, price = line.split(",")
            if bound == "-1":
                upper = float("inf")
            else:
                upper = int(bound)
            self.tiers[product].append((upper, int(price)))
        for product in self.tiers:
            self.tiers[product].sort()

    # Each unit pays the price of the tier that contains it. T: O(tiers).
    def usage_charge(self, product, qty):
        charge = 0
        # prev = upper bound of the previous tier (units already priced)
        prev = 0
        for bound, price in self.tiers[product]:
            if qty <= prev:
                break
            units = min(qty, bound) - prev
            charge += units * price
            prev = bound
        return charge


# Part 5 adds no code: Part 1 already floors once per user, every mentioned user gets
# a key in total30 (zero totals included), and rows are sorted by user.
Part5 = Part4


# The judged entry point: the last part passes every part's tests.
def calculate_monthly_charges(subscriptions, changes, pricing, usage):
    return Part5().run(subscriptions, changes, pricing, usage)


def test(label, part, subscriptions, changes, pricing, usage, want):
    got = part().run(subscriptions, changes, pricing, usage)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("p1-sum", Part1, ["a,s1,300", "a,s2,600", "b,s3,900"], [], [], [],
         [["a", "900"], ["b", "900"]])
    test("p2-chain-unordered", Part2, ["a,s1,300"],
         ["a,s2,s3,900,21", "a,s1,s2,600,11"], [], [], [["a", "600"]])
    test("p2-add-sub", Part2, [], ["a,-,s1,300,21"], [], [], [["a", "100"]])
    test("p2-floor-once", Part2, ["a,s1,1", "a,s2,1"], ["a,s1,s3,2,11", "a,s2,s4,2,11"],
         [], [], [["a", "3"]])
    test("p3-flat", Part3, [], [], ["api,-1,5"], ["a,api,2", "b,api,4", "a,api,1"],
         [["a", "15"], ["b", "20"]])
    test("p4-tiers", Part4, [], [], ["api,-1,1", "api,10,5"],
         ["b,api,6", "c,api,3", "b,api,6", "d,api,0"],
         [["b", "52"], ["c", "15"], ["d", "0"]])
    test("p5-combined", Part5, ["a,s1,300", "b,s2,0"], ["a,s1,s3,600,16"],
         ["api,-1,1", "api,2,10"], ["a,api,3", "c,api,0"],
         [["a", "471"], ["b", "0"], ["c", "0"]])


main()
