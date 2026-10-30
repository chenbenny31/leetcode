# Stripe OA: WebSocket Load Balancer (HackerRank, 5 parts).
# Function: route_requests(num_targets: int, max_connections_per_target: int,
#                          requests: list[str]) -> list[str]
#
# Input: targets are numbered 1..num_targets. Requests, processed in order:
#   CONNECT,connectionId,userId,objectId
#   DISCONNECT,connectionId,userId,objectId
#   SHUTDOWN,targetIndex
# Output: a log with one "connectionId,userId,targetIndex" per successful assignment.
# Rejected connections and requests that assign nothing log nothing.
#
# Part 1 - Least-loaded routing: a CONNECT goes to the target with the fewest active
#   connections; ties go to the smaller index.
# Part 2 - Disconnect: DISCONNECT removes the active connection with that id and frees
#   its slot. A disconnect for an id that is not active is a no-op.
# Part 3 - Object affinity: while an object has active connections, every new
#   connection for that objectId goes to the same target. When its last active
#   connection is gone, the affinity is cleared.
# Part 4 - Capacity and duplicate ids: no target may hold more than
#   max_connections_per_target. If the target chosen by the rules above is full, the
#   connection is rejected (no other target is tried). A CONNECT that repeats an active
#   connectionId changes nothing and logs that connection's original user and target.
# Part 5 - Shutdown and rerouting: SHUTDOWN,t first removes every active connection on
#   target t (clearing affinities whose last connection was removed), then reroutes
#   the evicted connections in ascending connectionId order with Parts 1-4 while t is
#   not eligible. Each successful reroute logs a new line; an unplaceable one is
#   dropped silently. Afterwards t is available again; a later SHUTDOWN of the same
#   target is a new event.
#
# Stacking notes:
# - Keep a record per connection (user, object, target) from Part 1. Part 2 needs the
#   target to free, Part 3 the object, Part 4 the original user and target.
# - Pick the least-loaded target with a heap from Part 1; a linear scan over 1e5
#   targets per request is too slow, and the heap also serves Parts 2 and 5.
# - Split connect into choose(obj) -> target, can_use(target) and assign(...). Part 3
#   overrides choose, Part 4 can_use, and both extend assign / disconnect via super().
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

import heapq
from collections import defaultdict


# Part 1: send each connection to the least-loaded target (ties -> smaller index).
# A min-heap of (load, target) with lazy deletion: every load change pushes a fresh
# entry, and an entry whose load no longer matches load[target] is stale and skipped
# when it reaches the top. (load, target) order already breaks ties by index.
class Part1:
    def __init__(self, num_targets, capacity):
        # load[t] = active connections on target t (index 0 unused)
        self.load = [0] * (num_targets + 1)
        # min-heap of (load, target); an entry is stale when load[target] differs
        self.heap = []
        for t in range(1, num_targets + 1):
            self.heap.append((0, t))
        # conns[conn] = (user, object, target) of active connections
        self.conns = {}
        self.log = []

    # Each request pushes O(1) heap entries and each entry is popped once.
    # T: O(R log R + num_targets) for R requests, S: O(R + num_targets).
    def run(self, requests):
        for line in requests:
            self.handle(line.split(","))
        return self.log

    def handle(self, f):
        if f[0] == "CONNECT":
            self.connect(f[1], f[2], f[3])

    def connect(self, conn, user, obj):
        t = self.choose(obj)
        if self.can_use(t):
            self.assign(conn, user, obj, t)

    def choose(self, obj):
        return self.least_loaded()

    def can_use(self, t):
        return t is not None

    def assign(self, conn, user, obj, t):
        self.conns[conn] = (user, obj, t)
        self.change_load(t, 1)
        self.log.append(f"{conn},{user},{t}")

    def change_load(self, t, delta):
        self.load[t] += delta
        heapq.heappush(self.heap, (self.load[t], t))

    def least_loaded(self):
        while self.heap:
            load, t = self.heap[0]
            # the top is current: it is the least-loaded target
            if load == self.load[t]:
                return t
            heapq.heappop(self.heap)
        return None


# Part 2: disconnect. Drop the record and lower the target's load; change_load pushes
# the new (smaller) entry, so the heap sees the freed slot at once.
class Part2(Part1):
    def handle(self, f):
        if f[0] == "DISCONNECT":
            self.disconnect(f[1])
        else:
            super().handle(f)

    def disconnect(self, conn):
        if conn not in self.conns:
            return
        user, obj, t = self.conns.pop(conn)
        self.change_load(t, -1)


# Part 3: object affinity. pin[object] = target while the object has active
# connections; choose() follows the pin before asking the heap. A count per object
# tells when its last connection leaves, which clears the pin.
class Part3(Part2):
    def __init__(self, num_targets, capacity):
        super().__init__(num_targets, capacity)
        # pin[object] = target its active connections use
        self.pin = {}
        # pin_count[object] = number of its active connections
        self.pin_count = defaultdict(int)

    def choose(self, obj):
        if obj in self.pin:
            return self.pin[obj]
        return super().choose(obj)

    def assign(self, conn, user, obj, t):
        super().assign(conn, user, obj, t)
        self.pin[obj] = t
        self.pin_count[obj] += 1

    def disconnect(self, conn):
        if conn not in self.conns:
            return
        user, obj, t = self.conns[conn]
        super().disconnect(conn)
        self.pin_count[obj] -= 1
        if self.pin_count[obj] == 0:
            del self.pin[obj]
            del self.pin_count[obj]


# Part 4: capacity and duplicate ids. can_use() also rejects a full target; the chosen
# target is final (no second try). If the least-loaded target is full, every target is.
# A repeated active id re-logs its original line and changes nothing.
class Part4(Part3):
    def __init__(self, num_targets, capacity):
        super().__init__(num_targets, capacity)
        self.capacity = capacity

    def connect(self, conn, user, obj):
        if conn in self.conns:
            old_user, old_obj, t = self.conns[conn]
            self.log.append(f"{conn},{old_user},{t}")
            return
        super().connect(conn, user, obj)

    def can_use(self, t):
        if not super().can_use(t):
            return False
        return self.load[t] < self.capacity


# Part 5: shutdown. Evict = disconnect every connection on the target (this also clears
# pins whose last connection was there), then reconnect them in id order through the
# normal connect(), so Parts 1-4 apply to reroutes for free. While rerouting, the target
# is "closed": least_loaded() sets its heap entries aside and puts them back afterwards.
class Part5(Part4):
    def __init__(self, num_targets, capacity):
        super().__init__(num_targets, capacity)
        # on_target[t] = conn ids currently on target t
        self.on_target = defaultdict(set)
        # target being shut down; excluded from routing while set
        self.closed = None

    def handle(self, f):
        if f[0] == "SHUTDOWN":
            self.shutdown(int(f[1]))
        else:
            super().handle(f)

    def assign(self, conn, user, obj, t):
        super().assign(conn, user, obj, t)
        self.on_target[t].add(conn)

    def disconnect(self, conn):
        if conn in self.conns:
            user, obj, t = self.conns[conn]
            self.on_target[t].remove(conn)
        super().disconnect(conn)

    def least_loaded(self):
        # entries of the closed target popped off the top; restored before returning
        held = []
        while True:
            t = super().least_loaded()
            if t is None or t != self.closed:
                break
            held.append(heapq.heappop(self.heap))
        for entry in held:
            heapq.heappush(self.heap, entry)
        return t

    # T: O(E log E + E log R) for E evicted connections.
    def shutdown(self, t):
        self.closed = t
        evicted = []
        for conn in sorted(self.on_target[t]):
            user, obj, old_target = self.conns[conn]
            evicted.append((conn, user, obj))
            self.disconnect(conn)
        for conn, user, obj in evicted:
            self.connect(conn, user, obj)
        self.closed = None


# The judged entry point: the last part passes every part's tests.
def route_requests(num_targets, max_connections_per_target, requests):
    return Part5(num_targets, max_connections_per_target).run(requests)


def test(label, part, n, cap, requests, want):
    print(label, "PASS" if part(n, cap).run(requests) == want else "FAIL")


def main():
    test("p1-least-load", Part1, 2, 5,
         ["CONNECT,c1,u,x", "CONNECT,c2,u,y", "CONNECT,c3,u,z"],
         ["c1,u,1", "c2,u,2", "c3,u,1"])
    test("p2-disconnect", Part2, 2, 5,
         ["CONNECT,c1,u,x", "CONNECT,c2,u,y", "DISCONNECT,c1,u,x", "DISCONNECT,zz,u,x",
          "CONNECT,c3,u,z"],
         ["c1,u,1", "c2,u,2", "c3,u,1"])
    test("p3-pin-and-clear", Part3, 2, 5,
         ["CONNECT,c1,u,x", "CONNECT,c2,u,x", "DISCONNECT,c1,u,x", "DISCONNECT,c2,u,x",
          "CONNECT,c3,u,w", "CONNECT,c4,u,x"],
         ["c1,u,1", "c2,u,1", "c3,u,1", "c4,u,2"])
    test("p4-capacity-dup", Part4, 2, 1,
         ["CONNECT,c1,u1,x", "CONNECT,c2,u2,x", "CONNECT,c1,u9,y", "CONNECT,c3,u3,y",
          "CONNECT,c4,u4,z"],
         ["c1,u1,1", "c1,u1,1", "c3,u3,2"])
    test("p5-shutdown", Part5, 3, 2,
         ["CONNECT,d,u,s", "CONNECT,a,u,q", "CONNECT,c,u,r", "CONNECT,b,u,p",
          "SHUTDOWN,1", "CONNECT,e,u,t"],
         ["d,u,1", "a,u,2", "c,u,3", "b,u,1", "b,u,2", "d,u,3", "e,u,1"])


main()
