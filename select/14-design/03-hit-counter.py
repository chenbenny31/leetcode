# LC 362 Design Hit Counter (Medium). Pattern: sliding window queue; circular buckets.
# hit(timestamp) records a hit; get_hits(timestamp) = hits in (timestamp - 300,
# timestamp]. Timestamps are seconds and never decrease.

from collections import deque


# Queue of hit times; pop everything 300+ seconds old.
# T: O(1) amortized per op (each hit popped once), S: O(hits in window).
class HitCounter:
    def __init__(self):
        # timestamps of hits still inside the window, oldest at the front
        self.hits = deque()

    def evict(self, timestamp):
        while self.hits and self.hits[0] <= timestamp - 300:
            self.hits.popleft()

    def hit(self, timestamp):
        self.hits.append(timestamp)
        self.evict(timestamp)

    def get_hits(self, timestamp):
        self.evict(timestamp)
        return len(self.hits)


# Follow-up (many hits per second): one bucket per second of the window, reused
# circularly. T: O(1) hit, O(300) get_hits; S: O(300).
class HitCounterBuckets:
    def __init__(self):
        # times[i] = the second bucket i currently counts
        self.times = [0] * 300
        # counts[i] = hits during second times[i]
        self.counts = [0] * 300

    def hit(self, timestamp):
        bucket = timestamp % 300
        # bucket still holds an older second: reset it
        if self.times[bucket] != timestamp:
            self.times[bucket] = timestamp
            self.counts[bucket] = 0
        self.counts[bucket] += 1

    def get_hits(self, timestamp):
        total = 0
        for i in range(300):
            if timestamp - self.times[i] < 300:
                total += self.counts[i]
        return total


def test(label, hits, now, want):
    queue_counter = HitCounter()
    bucket_counter = HitCounterBuckets()
    for time in hits:
        queue_counter.hit(time)
        bucket_counter.hit(time)
    ok = queue_counter.get_hits(now) == want and bucket_counter.get_hits(now) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("same-second", [5, 5, 5], 5, 3)
    test("edge-300", [1, 2, 3, 300], 300, 4)
    test("first-expires", [1, 2, 3, 300], 301, 3)
    test("all-expired", [1, 2], 1000, 0)


main()

# follow-up: concurrent hits -> threading.Lock per counter (CPython's GIL does not make
#   counts[i] += 1 atomic); striped locks per bucket in the bucket version
# follow-up: out-of-order timestamps -> buckets still work as long as delay < window
