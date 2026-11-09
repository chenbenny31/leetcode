# Cluster first-fit scheduler (Medium).
# Pattern: event simulation + global min-heap of end times.
# cap[i] = CPUs of node i; jobs[id] = [submit, duration, cpu], FIFO by (submit, id).
# The head job takes the lowest-index node with enough free CPUs, else it and all behind
# it wait until running jobs finish. Every job fits on some node. Return start times.

import heapq
from collections import deque


# lowest-index node with at least `need` idle CPUs, or -1
def first_fit(free_cpu, need):
    num_nodes = len(free_cpu)
    for i in range(num_nodes):
        if free_cpu[i] >= need:
            return i
    return -1


# Time jumps to the next submit or completion; at each time release finished jobs, admit
# submits, then place waiting jobs in FIFO order. T: O(J * (N + log J)), S: O(J + N).
def schedule(cap, jobs):
    num_jobs = len(jobs)
    # submit order: (submit, job_id)
    order = []
    for job_id in range(num_jobs):
        order.append((jobs[job_id][0], job_id))
    order.sort()
    # free_cpu[i] = idle CPUs on node i
    free_cpu = list(cap)
    # start[job_id] = start time of that job
    start = [0] * num_jobs
    # node_of[job_id] = node that runs that job
    node_of = [0] * num_jobs
    # running jobs across the whole cluster, earliest end first: (end time, job_id)
    running = []
    # job ids submitted but not placed yet, FIFO
    waiting = deque()
    # order[nxt] = next job to submit
    nxt = 0
    now = 0
    while nxt < num_jobs or waiting:
        while running and running[0][0] <= now:
            end, job_id = heapq.heappop(running)
            free_cpu[node_of[job_id]] += jobs[job_id][2]
        while nxt < num_jobs and order[nxt][0] <= now:
            waiting.append(order[nxt][1])
            nxt += 1
        while waiting:
            job_id = waiting[0]
            cpu = jobs[job_id][2]
            node = first_fit(free_cpu, cpu)
            # head-of-line blocking: nobody behind the head may start
            if node == -1:
                break
            waiting.popleft()
            free_cpu[node] -= cpu
            node_of[job_id] = node
            start[job_id] = now
            heapq.heappush(running, (now + jobs[job_id][1], job_id))
        # a blocked head always has a running job to wait for, so next_time stays finite
        next_time = float("inf")
        if running:
            next_time = running[0][0]
        if nxt < num_jobs:
            next_time = min(next_time, order[nxt][0])
        now = next_time
    return start


def test(label, cap, jobs, want):
    print(label, "PASS" if schedule(cap, jobs) == want else "FAIL")


def main():
    test("wait-for-release", [4, 4], [[0, 13, 3], [0, 5, 3], [1, 1, 2], [1, 2, 2]],
         [0, 0, 5, 5])
    test("fits-on-arrival", [4, 4], [[0, 3, 4], [2, 1, 4]], [0, 2])
    test("head-of-line", [4], [[0, 5, 3], [1, 1, 4], [2, 1, 1]], [0, 5, 6])


main()

# follow-up: head-of-line blocking -> backfill: a later job may start now if it fits and
#   ends before the head's earliest possible start
# follow-up: best fit -> sorted list of (free, node) + bisect_left((cpu, -1)) = tightest
#   node; O(N) per insert/remove without sortedcontainers
