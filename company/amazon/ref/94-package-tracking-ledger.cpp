// Shipments have a tracking id and a deadline; scanner events have (tracking id,
// timestamp, location, type), possibly out of order. For each shipment report its latest
// known location (from the newest event) and its status: ON_TIME if it has a DELIVERED
// event no later than the deadline, LATE if delivered after it, PENDING otherwise.
// Return the ledger sorted by tracking id. Timestamps are ISO-8601 strings (UTC), so
// string order is time order. (Simplified: inputs given as structs, not JSON.)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::unordered_map;

struct Event {
    string id, time, location, type;
};

struct Entry {
    string location, status;
    bool operator==(const Entry& o) const {
        return location == o.location && status == o.status;
    }
};

// One pass over events keeps the newest event and the delivery time per id.
// T: O(e + s log s), S: O(s).
map<string, Entry> ledger(const map<string, string>& deadlines,
                          const vector<Event>& events) {
    // newest[id] = latest event; delivered[id] = time of the DELIVERED event
    unordered_map<string, Event> newest;
    unordered_map<string, string> delivered;
    for (const Event& e : events) {
        if (!newest.count(e.id) || e.time > newest[e.id].time) { newest[e.id] = e; }
        if (e.type == "DELIVERED") { delivered[e.id] = e.time; }
    }
    map<string, Entry> result;
    for (const auto& [id, deadline] : deadlines) {
        Entry entry{"", "PENDING"};
        if (newest.count(id)) { entry.location = newest[id].location; }
        if (delivered.count(id)) {
            if (delivered[id] <= deadline) {
                entry.status = "ON_TIME";
            } else {
                entry.status = "LATE";
            }
        }
        result[id] = entry;
    }
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    map<string, string> deadlines = {{"B2", "2026-01-10T18:00:00Z"},
                                     {"A1", "2026-01-10T18:00:00Z"},
                                     {"C3", "2026-01-10T12:00:00Z"},
                                     {"D4", "2026-01-11T00:00:00Z"}};
    vector<Event> events = {{"A1", "2026-01-10T09:00:00Z", "Hub 2", "IN_TRANSIT"},
                            {"A1", "2026-01-10T08:00:00Z", "Hub 1", "IN_TRANSIT"},
                            {"B2", "2026-01-10T17:00:00Z", "Door", "DELIVERED"},
                            {"C3", "2026-01-10T13:00:00Z", "Door", "DELIVERED"}};
    map<string, Entry> got = ledger(deadlines, events);
    check(got["A1"] == Entry{"Hub 2", "PENDING"}, "newest-event");
    check(got["B2"] == Entry{"Door", "ON_TIME"}, "on-time");
    check(got["C3"] == Entry{"Door", "LATE"}, "late");
    check(got["D4"] == Entry{"", "PENDING"}, "no-events");
    check(got.begin()->first == "A1", "sorted");
}
