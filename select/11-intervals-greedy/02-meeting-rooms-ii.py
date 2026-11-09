# LC 253 Meeting Rooms II (Medium). Pattern: sort by start + min-heap of end times.
# Meetings are [start, end); return the min number of rooms so every meeting has one.
# A room freed at time t can host a meeting starting at t.

import heapq


# Sort by start; before placing a meeting free every room that has ended. The most
# rooms in use at once is the answer. T: O(n log n), S: O(n).
def min_meeting_rooms(intervals):
    intervals = sorted(intervals)
    # end times of the meetings currently holding a room, earliest on top
    ends = []
    best = 0
    for meeting in intervals:
        # <=: a room freed at t is free for a start at t
        while ends and ends[0] <= meeting[0]:
            heapq.heappop(ends)
        heapq.heappush(ends, meeting[1])
        best = max(best, len(ends))
    return best


def test(label, intervals, want):
    print(label, "PASS" if min_meeting_rooms(intervals) == want else "FAIL")


def main():
    test("reuse-at-end", [[1, 5], [2, 6], [5, 7]], 2)
    test("long-one", [[1, 10], [2, 3], [4, 5], [6, 7]], 2)
    test("same-time", [[1, 4], [1, 4], [1, 4]], 3)
    test("empty", [], 0)


main()

# follow-up: sweep line -> events (time, +1 / -1); sorting tuples puts -1 first at
#   equal times; the max running sum is the answer, O(n log n)
# follow-up: can one person attend all (LC 252) -> sort, check start < previous end
