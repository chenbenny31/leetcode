# LC 621 Task Scheduler (Medium). Pattern: max-heap by remaining count, n+1 cycles.
# Tasks are letters A-Z, one unit each; two runs of the same letter need at least n
# units between them (idle allowed). Return the min total units.

import heapq


# Fill cycles of n + 1 slots with the letters that have the most copies left.
# T: O(T log 26) = O(T) for T tasks, S: O(26).
def least_interval(tasks, n):
    # freq[letter] = copies of that letter
    freq = [0] * 26
    for task in tasks:
        freq[ord(task) - ord("A")] += 1
    # -(copies left) of each letter still to run, largest count on top
    heap = []
    for copies in freq:
        if copies > 0:
            heapq.heappush(heap, -copies)
    time = 0
    while heap:
        # letters run in this cycle that still have copies left
        requeue = []
        ran = 0
        while ran <= n and heap:
            copies = -heapq.heappop(heap)
            ran += 1
            if copies > 1:
                requeue.append(copies - 1)
        for copies in requeue:
            heapq.heappush(heap, -copies)
        # the last cycle needs no idle tail
        if heap:
            time += n + 1
        else:
            time += ran
    return time


# Formula: max_freq - 1 full frames of n + 1 slots, plus a last row of the letters
# tied at max_freq; never less than the task count. T: O(T), S: O(26).
def least_interval_formula(tasks, n):
    # freq[letter] = copies of that letter
    freq = [0] * 26
    for task in tasks:
        freq[ord(task) - ord("A")] += 1
    max_freq = max(freq)
    num_at_max = freq.count(max_freq)
    frame_slots = (max_freq - 1) * (n + 1) + num_at_max
    # more tasks than frame slots: the frames overflow and no idle is needed
    return max(len(tasks), frame_slots)


def test(label, tasks, n, want):
    ok = least_interval(tasks, n) == want and least_interval_formula(tasks, n) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("idle-needed", ["A", "A", "A", "B"], 2, 7)
    test("no-cooldown", ["A", "B", "A"], 0, 3)
    test("tie-at-max", ["A", "A", "B", "B"], 2, 5)
    test("no-idle", ["A", "A", "B", "B", "C", "C", "D"], 1, 7)


main()

# follow-up: return the actual schedule -> same heap loop, append each popped letter
#   and n + 1 - ran idles per non-final cycle
# follow-up: fixed task order (LC 2365) -> map letter -> next allowed day, O(T)
