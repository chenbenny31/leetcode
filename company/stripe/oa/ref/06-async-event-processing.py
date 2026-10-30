# Stripe OA: Asynchronous Event Processing (HackerRank, 4 parts).
# Function: process_events(events: list[str]) -> list[str]
#
# Input: one CSV line per event, in the order received:
#   event_id,event_time,event_type,payment_id,payment_event_type,merchant_id,amount
# "-" marks an empty field. An event whose event_id was seen before is never processed
# again; the first row with an id uses it up even if that event turns out invalid.
# Output: one line per payment, in the order its create event was processed:
#   payment_id,merchant_id,state,amount_authorized,amount_captured,last_event_at
# Numbers default to 0 (e.g. "payment_1,merchant_1,created,0,0,0").
#
# Part 1 - Aggregate payment states. Only event_type "payment", with
#   payment_event_type create (carries merchant_id), authorize / capture (carry amount).
#   none -create-> created -authorize-> authorized (more authorizes add up);
#   authorized / partially_captured -capture-> partially_captured, or successful once
#   amount_captured == amount_authorized (captured never exceeds authorized).
#   Events always arrive in a valid order. last_event_at = time of the last event
#   applied to that payment.
# Part 2 - Ignore out-of-order events: an event the current state does not allow is
#   skipped. Allowed: none: create | created: authorize | authorized: authorize,
#   capture | partially_captured: capture | successful: nothing.
# Part 3 - Refunds: event_type "refund" carries only payment_id. Only successful
#   accepts it -> refunded (changes only state and last_event_at). refunded accepts
#   nothing.
# Part 4 - Merchant risk: event_type "merchant_update" with merchant_id and amount =
#   risk score 0..100. A score >= 80 blocks the merchant for the rest of the input (a
#   lower score later does not unblock). Every later authorize for a blocked merchant's
#   payments is ignored, old or new; create / capture / refund still work. The update
#   itself changes no payment.
#
# Stacking notes:
# - One Payment record per id and one handle() with an if / elif per action. A later
#   part handles its new event type first and passes everything else to super().
# - Keep "is this event allowed now?" in one method, allowed(). Part 2 turns it into a
#   state -> actions table, Part 3 adds two rows, Part 4 adds one extra condition.
# - The dedup set is needed from Part 1 (its example repeats an event id).
# - A dict of payments keeps insertion order = creation order; no extra list.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.


class Payment:
    def __init__(self, merchant, t):
        self.merchant = merchant
        self.state = "created"
        self.authorized = 0
        self.captured = 0
        self.last = t


# Part 1: replay payment events into one record per payment.
# Dedup by event id first, then apply create / authorize / capture. The order is
# trusted, so the only check is that the payment exists (or not, for a create).
class Part1:
    def __init__(self):
        # event ids already handled
        self.seen = set()
        # payments[payment_id] = its aggregated state, in creation order
        self.payments = {}

    # One pass over the events. T: O(n), S: O(n).
    def run(self, events):
        for line in events:
            eid, t, etype, pid, ptype, merchant, amount = line.split(",")
            if eid in self.seen:
                continue
            self.seen.add(eid)

            # payment events are named by their sub-type, the others by their type
            if etype == "payment":
                action = ptype
            else:
                action = etype
            self.handle(int(t), action, pid, merchant, amount)

        out = []
        for pid, p in self.payments.items():
            amounts = f"{p.authorized},{p.captured}"
            out.append(f"{pid},{p.merchant},{p.state},{amounts},{p.last}")
        return out

    def handle(self, t, action, pid, merchant, amount):
        p = self.payments.get(pid)
        if not self.allowed(p, action):
            return

        if action == "create":
            self.payments[pid] = Payment(merchant, t)
            return

        if action == "authorize":
            p.authorized += int(amount)
            p.state = "authorized"

        elif action == "capture":
            p.captured += int(amount)
            if p.captured == p.authorized:
                p.state = "successful"
            else:
                p.state = "partially_captured"

        p.last = t

    def allowed(self, p, action):
        if p is None:
            return action == "create"
        return action != "create"


# Part 2: events can now arrive out of order. Replace the existence check with a table
# of the actions each state accepts; everything else stays the same.
class Part2(Part1):
    # VALID[state] = actions allowed in that state ("none" = payment does not exist)
    VALID = {
        "none": {"create"},
        "created": {"authorize"},
        "authorized": {"authorize", "capture"},
        "partially_captured": {"capture"},
        "successful": set(),
    }

    def allowed(self, p, action):
        if p is None:
            state = "none"
        else:
            state = p.state
        return action in self.VALID[state]


# Part 3: refunds. Two new table rows (successful -> refund, refunded -> nothing) and
# one new branch in handle(); the refund only changes state and last_event_at.
class Part3(Part2):
    VALID = {
        "none": {"create"},
        "created": {"authorize"},
        "authorized": {"authorize", "capture"},
        "partially_captured": {"capture"},
        "successful": {"refund"},
        "refunded": set(),
    }

    def handle(self, t, action, pid, merchant, amount):
        if action != "refund":
            super().handle(t, action, pid, merchant, amount)
            return
        p = self.payments.get(pid)
        if self.allowed(p, action):
            p.state = "refunded"
            p.last = t


# Part 4: merchant risk. A merchant_update only records blocked merchants; allowed()
# then rejects authorize for them before the usual state check. Blocking never undoes.
class Part4(Part3):
    def __init__(self):
        super().__init__()
        # merchants that ever had a risk score >= 80
        self.blocked = set()

    def handle(self, t, action, pid, merchant, amount):
        if action != "merchant_update":
            super().handle(t, action, pid, merchant, amount)
            return
        if int(amount) >= 80:
            self.blocked.add(merchant)

    def allowed(self, p, action):
        if action == "authorize" and p is not None and p.merchant in self.blocked:
            return False
        return super().allowed(p, action)


# The judged entry point: the last part passes every part's tests.
def process_events(events):
    return Part4().run(events)


def test(label, part, events, want):
    print(label, "PASS" if part().run(events) == want else "FAIL")


def main():
    test("p1-lifecycle", Part1,
         ["e1,0,payment,p1,create,m1,-", "e2,1,payment,p2,create,m2,-",
          "e3,2,payment,p1,authorize,-,10", "e4,3,payment,p1,authorize,-,5",
          "e4,4,payment,p1,authorize,-,5", "e5,5,payment,p1,capture,-,15"],
         ["p1,m1,successful,15,15,5", "p2,m2,created,0,0,1"])
    test("p1-partial", Part1,
         ["e1,0,payment,p1,create,m1,-", "e2,1,payment,p1,authorize,-,10",
          "e3,2,payment,p1,capture,-,4"],
         ["p1,m1,partially_captured,10,4,2"])
    test("p2-skip-invalid", Part2,
         ["e1,0,payment,p1,create,m1,-", "e2,1,payment,p1,capture,-,5",
          "e3,2,payment,p1,authorize,-,10", "e4,3,payment,p1,capture,-,4",
          "e5,4,payment,p1,authorize,-,5", "e2,5,payment,p1,capture,-,5",
          "e6,6,payment,px,authorize,-,1"],
         ["p1,m1,partially_captured,10,4,3"])
    test("p3-refund", Part3,
         ["e1,0,payment,p1,create,m1,-", "e2,1,payment,p1,authorize,-,5",
          "e3,2,payment,p1,capture,-,5", "e4,3,refund,p1,-,-,-",
          "e5,4,refund,p1,-,-,-", "e6,5,payment,p2,create,m1,-",
          "e7,6,payment,p2,authorize,-,5", "e8,7,payment,p2,capture,-,1",
          "e9,8,refund,p2,-,-,-"],
         ["p1,m1,refunded,5,5,3", "p2,m1,partially_captured,5,1,7"])
    test("p4-blocked", Part4,
         ["e1,0,payment,p1,create,m1,-", "e2,1,payment,p1,authorize,-,5",
          "e3,2,merchant_update,-,-,m1,80", "e4,3,payment,p1,capture,-,5",
          "e5,4,payment,p2,create,m1,-", "e6,5,merchant_update,-,-,m1,0",
          "e7,6,payment,p2,authorize,-,5"],
         ["p1,m1,successful,5,5,3", "p2,m1,created,0,0,4"])


main()
