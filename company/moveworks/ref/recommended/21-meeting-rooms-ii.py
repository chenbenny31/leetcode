# 21 · Meeting Rooms II (ServiceNow, experienced hires, Medium) — LC 253
# meetings [start, end); return the fewest rooms so every meeting has one;
# a room freed at t can host a meeting starting at t
#
# clarify:
# end exclusive or inclusive -> exclusive
# input sorted? -> no, sort first
# empty list -> 0
# only the cnt, or which room each gets -> cnt; room ids = follow-up

import heapq


# ==================== solution 1: min-heap of end times (start here) ====================
# extends to "which room" questions

# core idea: go by start; the heap = ends of meetings in progress; pop ends <= the new
# start; the max heap size = answer
# T: O(n log n), S: O(n)
def min_meeting_rooms(intervals):
    meetings = sorted(intervals)
    # end times of meetings holding a room, earliest on top
    ends = []
    best = 0
    for start, end in meetings:
        # <=: a room freed at t is free for a start at t
        while ends and ends[0] <= start:
            heapq.heappop(ends)
        heapq.heappush(ends, end)
        best = max(best, len(ends))
    return best


# ==================== solution 2: sweep line ====================
# +1 at each start, -1 at each end; the max running sum = answer

# T: O(n log n), S: O(n)
def min_rooms_sweep(intervals):
    # (time, +1 start / -1 end); -1 sorts first at equal t, so a freed room is reused
    events = []
    for start, end in intervals:
        events.append((start, 1))
        events.append((end, -1))
    events.sort()
    in_use = 0
    best = 0
    for _, change in events:
        in_use += change
        best = max(best, in_use)
    return best


# ==================== tests ====================

def test(label, intervals, want):
    ok = min_meeting_rooms(intervals) == want and min_rooms_sweep(intervals) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("reuse-at-end", [[1, 5], [2, 6], [5, 7]], 2)
    test("long-one", [[1, 10], [2, 3], [4, 5], [6, 7]], 2)
    test("same-time", [[1, 4], [1, 4], [1, 4]], 3)
    test("empty", [], 0)


main()

# follow-up:
# 1. can one person attend all (LC 252) -> sort by start; start < prev end = clash
# 2. closed intervals (LC 2406) -> pop only ends < start
# 3. which room each gets -> heap of (end, room id); reuse the top or open a new one
# 4. meeting rooms III (LC 2402) -> free-id heap + (end, room) heap; delay if none free
