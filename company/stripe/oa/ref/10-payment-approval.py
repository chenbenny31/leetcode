# Stripe OA: Payment Approval and Partial Capture (single part, no follow-ups).
# Function: process_payments(commands: list[list[str]]) -> list[str]
#
# Input: an ordered batch of commands, each exactly five strings:
#   ["CREATE",  id, payer, payee, amount]
#   ["APPROVE", id, "",    "",    amount]
#   ["CAPTURE", id, "",    "",    amount]
#   ["CANCEL",  id, "",    "",    ""]
# Amounts are decimal integers (may be <= 0, which is invalid); unused fields are "".
# Output: one string per command: the payment's new state if the command is valid,
# otherwise "ERROR". An invalid command changes nothing.
#
# Rules (every payment starts absent):
#   CREATE  - id never used before (even a cancelled id stays used), non-empty payer
#             and payee, amount > 0 -> CREATED.
#   APPROVE - only in CREATED; 0 < amount <= created amount; records the approved
#             amount -> APPROVED. A second approval is invalid (state moved on).
#   CAPTURE - only in APPROVED or PARTIALLY_CAPTURED; amount > 0 and the captured total
#             may not exceed the approved amount -> SUCCEEDED when the total equals the
#             approved amount, else PARTIALLY_CAPTURED.
#   CANCEL  - only in CREATED -> CANCELLED.
#   Unknown ids, over-capture and anything after SUCCEEDED / CANCELLED are invalid.
#   No balances, refunds or fees.
#
# Stacking notes (no later parts, but the same shape as the multi-part Stripe tasks):
# - Validate fully before changing anything, so "an invalid command changes nothing"
#   holds automatically.
# - Keep cancelled payments in the dict: the id must stay used.


class Payment:
    def __init__(self, amount):
        self.amount = amount
        self.state = "CREATED"
        self.approved = 0
        self.captured = 0


# One class: a state machine per payment id.
# valid() answers "may this command run now?" (state check + amount check) without
# touching anything; handle() applies the change only after that.
class Part1:
    # FROM[command] = states the command is valid in ("none" = id never used)
    FROM = {
        "CREATE": {"none"},
        "APPROVE": {"CREATED"},
        "CAPTURE": {"APPROVED", "PARTIALLY_CAPTURED"},
        "CANCEL": {"CREATED"},
    }

    def __init__(self):
        # payments[id] = its payment, kept after cancellation so the id stays used
        self.payments = {}

    # One dict lookup per command. T: O(n), S: O(n).
    def run(self, commands):
        out = []
        for cmd, pid, payer, payee, amount in commands:
            out.append(self.handle(cmd, pid, payer, payee, amount))
        return out

    def handle(self, cmd, pid, payer, payee, amount):
        p = self.payments.get(pid)
        # CANCEL has an empty amount; 0 fails every amount check
        if amount == "":
            amount = 0
        else:
            amount = int(amount)
        if not self.valid(p, cmd, payer, payee, amount):
            return "ERROR"

        if cmd == "CREATE":
            p = Payment(amount)
            self.payments[pid] = p

        elif cmd == "APPROVE":
            p.approved = amount
            p.state = "APPROVED"

        elif cmd == "CAPTURE":
            p.captured += amount
            if p.captured == p.approved:
                p.state = "SUCCEEDED"
            else:
                p.state = "PARTIALLY_CAPTURED"

        else:
            p.state = "CANCELLED"

        return p.state

    def valid(self, p, cmd, payer, payee, amount):
        if p is None:
            state = "none"
        else:
            state = p.state
        if state not in self.FROM[cmd]:
            return False

        if cmd == "CREATE":
            return payer != "" and payee != "" and amount > 0
        if cmd == "APPROVE":
            return 0 < amount <= p.amount
        if cmd == "CAPTURE":
            # the captured total may not pass the approved amount
            return 0 < amount <= p.approved - p.captured
        return True


# The judged entry point.
def process_payments(commands):
    return Part1().run(commands)


def test(label, commands, want):
    print(label, "PASS" if Part1().run(commands) == want else "FAIL")


def main():
    test("p1-full-capture",
         [["CREATE", "p", "a", "b", "50"], ["APPROVE", "p", "", "", "50"],
          ["CAPTURE", "p", "", "", "50"], ["CAPTURE", "p", "", "", "1"]],
         ["CREATED", "APPROVED", "SUCCEEDED", "ERROR"])
    test("p1-partial-over",
         [["CREATE", "p", "a", "b", "10"], ["APPROVE", "p", "", "", "6"],
          ["CAPTURE", "p", "", "", "4"], ["CAPTURE", "p", "", "", "3"],
          ["CAPTURE", "p", "", "", "2"]],
         ["CREATED", "APPROVED", "PARTIALLY_CAPTURED", "ERROR", "SUCCEEDED"])
    test("p1-bad-create",
         [["CREATE", "p", "", "b", "5"], ["CREATE", "p", "a", "b", "0"],
          ["CREATE", "p", "a", "b", "5"], ["APPROVE", "q", "", "", "1"],
          ["APPROVE", "p", "", "", "0"]],
         ["ERROR", "ERROR", "CREATED", "ERROR", "ERROR"])
    test("p1-cancel",
         [["CREATE", "p", "a", "b", "5"], ["CANCEL", "p", "", "", ""],
          ["APPROVE", "p", "", "", "5"], ["CREATE", "p", "a", "b", "5"],
          ["CREATE", "q", "a", "b", "5"], ["APPROVE", "q", "", "", "1"],
          ["CANCEL", "q", "", "", ""]],
         ["CREATED", "CANCELLED", "ERROR", "ERROR", "CREATED", "APPROVED", "ERROR"])


main()
