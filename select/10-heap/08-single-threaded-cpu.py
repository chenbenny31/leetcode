# LC 1834 Single-Threaded CPU (Medium). Pattern: sort by arrival + min-heap simulation.
# tasks[i] = [enqueue_time, processing_time]. When idle, the CPU starts the available
# task with the shortest processing time (ties: smaller index). Return the order.

import heapq


# Walk tasks in arrival order; push everything that has arrived, run the heap top,
# jump the clock when nothing is waiting. T: O(n log n), S: O(n).
def get_order(tasks):
    n = len(tasks)
    # [enqueue_time, processing_time, index], sorted by arrival
    jobs = []
    for i in range(n):
        jobs.append([tasks[i][0], tasks[i][1], i])
    jobs.sort()
    # (processing_time, index) of arrived tasks; shortest, then smallest index on top
    ready = []
    order = []
    # Python ints never overflow, so no long long concern here
    time = 0
    # jobs[nxt] = next task not yet pushed into ready
    nxt = 0
    while len(order) < n:
        # idle: jump to the next arrival instead of ticking
        if not ready and time < jobs[nxt][0]:
            time = jobs[nxt][0]
        while nxt < n and jobs[nxt][0] <= time:
            heapq.heappush(ready, (jobs[nxt][1], jobs[nxt][2]))
            nxt += 1
        proc, idx = heapq.heappop(ready)
        time += proc
        order.append(idx)
    return order


def test(label, tasks, want):
    print(label, "PASS" if get_order(tasks) == want else "FAIL")


def main():
    test("shortest-first", [[0, 5], [1, 2], [1, 1]], [0, 2, 1])
    test("tie-by-index", [[0, 3], [0, 1], [0, 1]], [1, 2, 0])
    test("idle-gap", [[10, 1], [0, 2]], [1, 0])
    test("big-times", [[0, 1000000000], [0, 1000000000], [1, 1]], [0, 2, 1])


main()

# follow-up: k CPUs (LC 1882 Process Tasks Using Servers) -> second heap of busy
#   (free_time, server); release servers whose free_time <= time before assigning
