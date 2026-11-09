# LC 855 Exam Room (Medium). Pattern: ordered set, scan gaps.
# n seats in a row. seat() takes the seat that maximizes the distance to the closest
# student (ties -> lowest index; seat 0 if the room is empty); leave(p) frees seat p.

import bisect


# Candidates: seat 0, the middle of each gap between neighbors, seat n - 1.
# Sorted list + bisect stands in for C++'s set (O(p) insert / delete).
# T: O(p) seat, O(p) leave; p = students seated. S: O(p).
class ExamRoom:
    def __init__(self, seats):
        # seats are 0..n-1
        self.n = seats
        # occupied seats, sorted
        self.taken = []

    def seat(self):
        best_seat = 0
        if self.taken:
            # sitting at 0: distance to the first student
            best_dist = self.taken[0]
            prev = self.taken[0]
            for cur in self.taken:
                # sit in the middle of the gap (prev, cur)
                dist = (cur - prev) // 2
                # strict > keeps the lowest index on ties
                if dist > best_dist:
                    best_dist = dist
                    best_seat = prev + dist
                prev = cur
            # sitting at n - 1: distance to the last student
            last_dist = self.n - 1 - prev
            if last_dist > best_dist:
                best_seat = self.n - 1
        bisect.insort(self.taken, best_seat)
        return best_seat

    def leave(self, p):
        i = bisect.bisect_left(self.taken, p)
        self.taken.pop(i)


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    room = ExamRoom(8)
    test("empty-room", room.seat(), 0)
    test("right-end", room.seat(), 7)
    test("middle", room.seat(), 3)
    test("bigger-gap", room.seat(), 5)
    room.leave(0)
    test("left-end", room.seat(), 0)
    test("tie-lowest", room.seat(), 1)


main()

# follow-up: O(log p) seat -> heap of gaps keyed (-dist, start) with lazy deletion plus
#   left/right neighbor dicts; leave merges the two gaps around p. Hard to write live
