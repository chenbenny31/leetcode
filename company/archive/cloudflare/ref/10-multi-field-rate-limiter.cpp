// Cloudflare (reported): Rate Limiter with Multi-Field Filter Rules (Medium).
// Pattern: sliding-window log per rule; check every matched rule, then record in all.
// Request: firstName, lastName, ip, country, timestamp (integer seconds, nondecreasing).
// Rule: filters {field -> value} on any subset of fields (all must match, {} = global)
// and at most `limit` requests per `window` seconds. Inclusive window: a past t counts
// if now - t <= window. allow(request) is false if ANY matched rule is at its limit, and
// then no rule records the request; otherwise every matched rule records `now`.
// Assumption: a filter is a specific value (ip == "1.2.3.4"), not "per distinct ip".

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

using std::cout;
using std::deque;
using std::string;
using std::unordered_map;
using std::vector;

struct Request {
    // field name ("firstName", "lastName", "ip", "country") -> value
    unordered_map<string, string> fields;
    int timestamp;
};

struct Rule {
    // field name -> required value; empty = matches every request
    unordered_map<string, string> filters;
    int limit;
    int window;
};

// One deque of accepted timestamps per rule. Pass 1 evicts expired entries and checks
// room in every matched rule; pass 2 records only if all had room.
// T: O(R * F) to match + O(M) amortized eviction, S: O(sum of limits), since a rule
// only records while it has room. (R rules, F filters per rule, M matched rules.)
class RateLimiter {
    vector<Rule> rules;
    // history[ruleId] = accepted timestamps that matched this rule, oldest first
    vector<deque<int>> history;

    bool matches(const Rule& rule, const Request& request) {
        for (const auto& [field, value] : rule.filters) {
            auto it = request.fields.find(field);
            if (it == request.fields.end() || it->second != value) { return false; }
        }
        return true;
    }

public:
    RateLimiter(const vector<Rule>& allRules)
        : rules(allRules), history(allRules.size()) {}

    bool allow(const Request& request) {
        int now = request.timestamp;
        vector<int> matched;
        for (int ruleId = 0; ruleId < (int)rules.size(); ruleId++) {
            if (matches(rules[ruleId], request)) { matched.push_back(ruleId); }
        }

        // pass 1: drop expired timestamps, block if any matched rule is full
        // (evicting before a block is safe: those entries are expired either way)
        for (int ruleId : matched) {
            deque<int>& times = history[ruleId];
            // inclusive window: keep t with now - t <= window
            while (!times.empty() && now - times.front() > rules[ruleId].window) {
                times.pop_front();
            }
            if ((int)times.size() >= rules[ruleId].limit) { return false; }
        }

        // pass 2: every matched rule has room, record the request in all of them
        for (int ruleId : matched) {
            history[ruleId].push_back(now);
        }
        return true;
    }
};

Request req(const string& first, const string& last, const string& ip,
            const string& country, int t) {
    return {{{"firstName", first}, {"lastName", last}, {"ip", ip}, {"country", country}},
            t};
}

void test(const string& label, const vector<Rule>& rules,
          const vector<Request>& requests, const vector<bool>& want) {
    RateLimiter limiter(rules);
    vector<bool> got;
    for (const Request& request : requests) {
        got.push_back(limiter.allow(request));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("window", {{{{"ip", "1.2.3.4"}}, 2, 60}},
         {req("alice", "smith", "1.2.3.4", "US", 0),
          req("alice", "smith", "1.2.3.4", "US", 10),
          req("alice", "smith", "1.2.3.4", "US", 20),
          req("alice", "smith", "1.2.3.4", "US", 60),
          req("alice", "smith", "1.2.3.4", "US", 61)},
         {true, true, false, false, true});
    test("all-or-none", {{{{"ip", "1.2.3.4"}}, 2, 60}, {{{"firstName", "alice"}}, 1, 60}},
         {req("alice", "smith", "1.2.3.4", "US", 0),
          req("alice", "smith", "1.2.3.4", "US", 1),
          req("bob", "lee", "1.2.3.4", "CA", 2),
          req("bob", "lee", "1.2.3.4", "CA", 3)},
         {true, false, true, false});
    test("subset", {{{{"country", "US"}, {"lastName", "smith"}}, 1, 60}, {{}, 3, 60}},
         {req("alice", "smith", "1.2.3.4", "US", 0),
          req("bob", "lee", "5.6.7.8", "CA", 1),
          req("carol", "smith", "1.2.3.4", "US", 2),
          req("dave", "lee", "5.6.7.8", "CA", 3)},
         {true, true, false, true});
}

// follow-up: many rules -> index each rule under its first filter, so a request scans
//   only rules sharing one of its 4 (field, value) pairs; {} rules go in a matchAll list:
//     // index["ip=1.2.3.4"] = ids of rules whose first filter is ip == "1.2.3.4"
//     unordered_map<string, vector<int>> index;
// follow-up: per-distinct-key rules ("each ip 2 per 60s") -> rule lists field names only;
//   history keyed by ruleId + the request's values on those fields:
//     unordered_map<string, deque<int>> history;   // "0|1.2.3.4" -> timestamps
//   idle keys are dropped lazily or by a periodic sweep.
// follow-up: memory -> sliding-window counter per rule, O(1) instead of O(limit):
//   estimate = prevCount * (window - elapsed) / window + curCount (approximate).
// follow-up: concurrency -> one mutex around pass 1 + pass 2 (check-and-record atomic);
//   across edges -> one Redis Lua script per request, or per-colo limits synced async.
