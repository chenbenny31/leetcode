# Stripe OA: Request Routing System (HackerRank, 3 parts; Part 2 was never shared).
# Function: solve_request_routing_system(input: str) -> list[str]
#
# Input: the whole stdin as one string, one command per line (skip empty lines).
# Output: one result string per command, in order.
# Each datacenter has a unique name, latitude, longitude, positive integer capacity,
# health (new = healthy) and current load (new = 0). Coordinates are valid when
# latitude is in [-90, 90] and longitude is in [-180, 180].
#
# Part 1 - Registry and distance:
#   REGISTER name lat lon capacity -> "OK" and add it when the name is new, the
#     coordinates are valid and capacity > 0; otherwise "ERROR".
#   SET_HEALTHY name value -> for a known name and value true / false (any case), set
#     its health and return "OK"; otherwise "ERROR".
#   DISTANCE lat1 lon1 lat2 lon2 -> Haversine great-circle distance with Earth radius
#     6371 km, rounded to the nearest integer; "ERROR" if any coordinate is invalid.
#   Any other command -> "ERROR".
# Part 2 - NOT SHARED; this is a reconstruction so Part 3 has something to build on:
#   ROUTE lat lon -> the nearest healthy datacenter by exact distance (exact tie ->
#     smaller name) as "name km"; "None" when no datacenter is healthy. No capacity.
# Part 3 - Capacity (shared):
#   ROUTE lat lon -> candidates = all healthy datacenters sorted by (exact distance,
#     name). The first candidate with load < capacity takes the request (load += 1):
#     "name km c1,c2,...". All candidates full -> "None c1,c2,...". No healthy
#     datacenter -> exactly "None" (no trailing space). Distances are rounded only for
#     display; the order uses the exact value.
# Assumed: ROUTE with invalid coordinates -> "ERROR" (not stated by the source).
#
# Stacking notes:
# - Store every field the statement lists (capacity, health, load) in the Datacenter
#   record from Part 1, even though Part 1 never reads load.
# - handle() is one if / elif per command. A later part adds its command in its own
#   handle() and passes everything else to super().
# - Keep the exact distance for sorting and round only when printing.
# Each PartN extends PartN-1 and overrides only what that part changes. In the OA you
# keep editing one class; the subclasses here only show the diff per part.

import math

EARTH_RADIUS_KM = 6371


class Datacenter:
    def __init__(self, lat, lon, capacity):
        self.lat = lat
        self.lon = lon
        self.capacity = capacity
        self.healthy = True
        self.load = 0


def valid(lat, lon):
    return -90 <= lat <= 90 and -180 <= lon <= 180


def haversine(lat1, lon1, lat2, lon2):
    phi1 = math.radians(lat1)
    phi2 = math.radians(lat2)
    d_phi = phi2 - phi1
    d_lambda = math.radians(lon2 - lon1)
    a = math.sin(d_phi / 2) ** 2
    a += math.cos(phi1) * math.cos(phi2) * math.sin(d_lambda / 2) ** 2
    return 2 * EARTH_RADIUS_KM * math.asin(math.sqrt(a))


# Round half up; Python's round() rounds half to even (round(0.5) == 0).
def km(dist):
    return int(dist + 0.5)


# Part 1: a registry of datacenters plus a stateless distance query.
# Every command validates first and only then changes state, so an ERROR never
# leaves a half-applied change.
class Part1:
    def __init__(self):
        # centers[name] = its datacenter
        self.centers = {}

    # Every Part 1 command is O(1). T: O(q) for q commands, S: O(k) for k datacenters.
    def run(self, text):
        out = []
        for line in text.split("\n"):
            parts = line.split()
            if not parts:
                continue
            out.append(self.handle(parts))
        return out

    def handle(self, parts):
        command = parts[0]
        if command == "REGISTER":
            return self.register(parts)
        if command == "SET_HEALTHY":
            return self.set_healthy(parts)
        if command == "DISTANCE":
            return self.distance(parts)
        return "ERROR"

    def register(self, parts):
        name = parts[1]
        lat = float(parts[2])
        lon = float(parts[3])
        capacity = int(parts[4])
        if name in self.centers:
            return "ERROR"
        if not valid(lat, lon) or capacity <= 0:
            return "ERROR"
        self.centers[name] = Datacenter(lat, lon, capacity)
        return "OK"

    def set_healthy(self, parts):
        name = parts[1]
        value = parts[2].lower()
        if name not in self.centers:
            return "ERROR"
        if value != "true" and value != "false":
            return "ERROR"
        self.centers[name].healthy = value == "true"
        return "OK"

    def distance(self, parts):
        lat1 = float(parts[1])
        lon1 = float(parts[2])
        lat2 = float(parts[3])
        lon2 = float(parts[4])
        if not valid(lat1, lon1) or not valid(lat2, lon2):
            return "ERROR"
        return str(km(haversine(lat1, lon1, lat2, lon2)))


# Part 2 (reconstructed): ROUTE goes to the nearest healthy datacenter.
# Split ROUTE into "rank the healthy ones" and "pick from the ranking": the ranking is
# the same in Part 3, only the picking rule changes.
class Part2(Part1):
    def handle(self, parts):
        if parts[0] == "ROUTE":
            return self.route(float(parts[1]), float(parts[2]))
        return super().handle(parts)

    # Sorts the healthy datacenters on every ROUTE. T: O(k log k) per ROUTE.
    def route(self, lat, lon):
        if not valid(lat, lon):
            return "ERROR"

        # ranked = [(exact distance, name)] nearest first; the name breaks exact ties
        ranked = []
        for name, center in self.centers.items():
            if center.healthy:
                dist = haversine(lat, lon, center.lat, center.lon)
                ranked.append((dist, name))
        ranked.sort()

        if not ranked:
            return "None"
        return self.pick(ranked)

    def pick(self, ranked):
        dist, name = ranked[0]
        return f"{name} {km(dist)}"


# Part 3: datacenters have capacity. Walk the ranking and take the first one that still
# has room; the output also lists every healthy candidate, full or not.
class Part3(Part2):
    def pick(self, ranked):
        names = [name for _, name in ranked]
        candidates = ",".join(names)
        for dist, name in ranked:
            center = self.centers[name]
            if center.load < center.capacity:
                center.load += 1
                return f"{name} {km(dist)} {candidates}"
        return "None " + candidates


# The judged entry point: the last part passes every part's tests.
def solve_request_routing_system(text):
    return Part3().run(text)


def test(label, part, text, want):
    print(label, "PASS" if part().run(text) == want else "FAIL")


def main():
    test("p1-register", Part1,
         "REGISTER a 0 0 1\nREGISTER a 1 1 1\nREGISTER b 0 181 1\n"
         "REGISTER c 0 0 0\nPING", ["OK", "ERROR", "ERROR", "ERROR", "ERROR"])
    test("p1-health", Part1,
         "REGISTER a 0 0 1\nSET_HEALTHY a FALSE\nSET_HEALTHY a maybe\n"
         "SET_HEALTHY z true", ["OK", "OK", "ERROR", "ERROR"])
    test("p1-distance", Part1, "DISTANCE 0 0 0 1\n\nDISTANCE 0 0 0 0\nDISTANCE 0 0 -91 0",
         ["111", "0", "ERROR"])
    test("p2-nearest-healthy", Part2,
         "REGISTER near 0 0 1\nREGISTER far 0 1 1\nROUTE 0 0\nROUTE 0 0\n"
         "SET_HEALTHY near false\nROUTE 0 0\nSET_HEALTHY far false\nROUTE 0 0",
         ["OK", "OK", "near 0", "near 0", "OK", "far 111", "OK", "None"])
    test("p2-tie-by-name", Part2, "REGISTER b 0 1 1\nREGISTER a 0 -1 1\nROUTE 0 0",
         ["OK", "OK", "a 111"])
    test("p3-capacity", Part3,
         "REGISTER near 0 0 1\nREGISTER far 0 1 5\nROUTE 0 0\nROUTE 0 0",
         ["OK", "OK", "near 0 near,far", "far 111 near,far"])
    test("p3-full-then-none", Part3,
         "REGISTER a 0 0 1\nROUTE 0 0\nROUTE 0 0\nSET_HEALTHY a false\nROUTE 0 0",
         ["OK", "a 0 a", "None a", "OK", "None"])


main()
