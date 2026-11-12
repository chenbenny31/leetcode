// AdService: insertAd(content, score) adds an ad; getAd() returns the content of the ad
// with the highest score, never the same ad twice in a row, and lowers the returned ad's
// score by 1. Ties go to the ad inserted first. getAd() returns "" when no ad can be
// served (no ads, or the only ad was served last time).
//
// Clarify:
// - ties -> the ad inserted first wins; two ads with the same text are still two ads
// - does a "" call unblock the last ad? -> assume no: the ad served last stays blocked
//   until another ad is served, so a lone ad gives "shoe", "", "" (if yes, reset lastId
//   to -1 when returning "")
// - scores may drop below 0 -> still served; ads are never removed (follow-up)
// - up to ~1e5 calls and ads -> O(log n) per call; the O(n) scan is the brute force
// Idea: max-heap of (score, -id): the top is the best ad, earliest insert on ties. If the
//   top was served last time, serve the runner-up and push the top back. Each ad keeps
//   exactly one heap entry, so there are no stale entries to skip.
// Pitfalls:
// - priority_queue<pair> is a max-heap on both fields: store -id so the lower id wins
// - pop the top before reading the runner-up, then push the skipped top back
// - only one ad and it was served last: push it back before returning "", or it is lost
// - copy heap.top() (auto, not auto&) before pop; push the served ad with score - 1
// Tests to write: no repeat with a low runner-up; tie -> earliest; decrement lets a
//   runner-up catch up; single ad -> "shoe", "", ""; empty.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <utility>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::queue;
using std::set;
using std::prev;

// straightforward: scan all ads, skipping the one served last. T: getAd O(n), S: O(n).
class AdServiceScan {
    // content[id] / score[id] of ad id, ids in insertion order
    vector<string> content;
    vector<int> score;
    // id served by the previous getAd, -1 if none
    int lastId = -1;

public:
    void insertAd(const string& text, int value) {
        content.push_back(text);
        score.push_back(value);
    }

    string getAd() {
        int n = content.size();
        int best = -1;
        for (int id = 0; id < n; id++) {
            if (id == lastId) { continue; }
            // strict > keeps the earliest insert on ties
            if (best == -1 || score[id] > score[best]) { best = id; }
        }
        if (best == -1) { return ""; }
        score[best]--;
        lastId = best;
        return content[best];
    }
};

// improved: max-heap of (score, -id). Pop the top; if it was served last time, serve the
// runner-up instead and push the top back. Every ad keeps exactly one heap entry, so no
// stale entries. T: O(log n) per call, S: O(n).
class AdService {
    // content[id] = text of ad id, ids in insertion order
    vector<string> content;
    // (score, -id): top = highest score, earliest insert on ties
    priority_queue<pair<int, int>> heap;
    // id served by the previous getAd, -1 if none
    int lastId = -1;

public:
    void insertAd(const string& text, int value) {
        int id = content.size();
        content.push_back(text);
        heap.push({value, -id});
    }

    string getAd() {
        if (heap.empty()) { return ""; }
        auto top = heap.top();
        heap.pop();
        auto chosen = top;
        if (-top.second == lastId) {
            if (heap.empty()) {
                heap.push(top);
                return "";
            }
            chosen = heap.top();
            heap.pop();
            heap.push(top);
        }
        auto [value, negId] = chosen;
        heap.push({value - 1, negId});
        lastId = -negId;
        return content[lastId];
    }
};

// follow-up: removeAd(id), ids 0, 1, ... in insertion order. An ordered set of
// (score, -id) can erase any ad's exact entry, so no lazy deletion; the best ad is the
// last element and the runner-up the one before it. T: O(log n) per call, S: O(n).
// Clarify: removing a removed id -> no-op; ids are valid.
// Pitfalls: copy *it before erasing it; erase by the exact (score, -id) key, so keep
//   score[id] current; a removed lastId is simply absent and blocks nothing.
// Tests to write: remove the best ad; remove the ad served last; remove twice.
class AdServiceRemovable {
    // content[id] / score[id] of ad id, ids in insertion order
    vector<string> content;
    vector<int> score;
    // (score, -id) of every ad not removed; best ad = last element
    set<pair<int, int>> ranked;
    // id served by the previous getAd, -1 if none
    int lastId = -1;

public:
    // returns the new ad's id
    int insertAd(const string& text, int value) {
        int id = content.size();
        content.push_back(text);
        score.push_back(value);
        ranked.insert({value, -id});
        return id;
    }

    void removeAd(int id) {
        // erase by key is safe: each (score, -id) is unique
        ranked.erase({score[id], -id});
    }

    string getAd() {
        if (ranked.empty()) { return ""; }
        auto it = prev(ranked.end());
        if (-it->second == lastId) {
            if (it == ranked.begin()) { return ""; }
            it--;
        }
        auto [value, negId] = *it;
        ranked.erase(it);
        ranked.insert({value - 1, negId});
        lastId = -negId;
        score[lastId] = value - 1;
        return content[lastId];
    }
};

// follow-up: an ad served at call c is blocked for the next k calls; calls that return ""
// count too (with k = 1 this differs from the base only after a "" call). The heap holds
// the servable ads; a served ad waits in a queue for k calls before going back.
// T: O(log n) per call, S: O(n + k).
// Pitfalls: a call that serves nothing still pushes a marker, or blocked ads wait too
//   long; an ad goes back only once more than k entries are queued (size > k).
// Tests to write: k = 2 with three ads; an empty call in between; k = 1 like the base.
class AdServiceCooldown {
    // content[id] = text of ad id, ids in insertion order
    vector<string> content;
    // (score, -id) of the ads that may be served now
    priority_queue<pair<int, int>> heap;
    // (score, -id) served by each of the last k calls, oldest first; {0, 1} = served
    // nothing (real -ids are <= 0)
    queue<pair<int, int>> cooling;
    // calls an ad stays blocked after it is served
    int k;

public:
    explicit AdServiceCooldown(int cooldown) : k(cooldown) {}

    void insertAd(const string& text, int value) {
        int id = content.size();
        content.push_back(text);
        heap.push({value, -id});
    }

    string getAd() {
        pair<int, int> served = {0, 1};
        if (!heap.empty()) {
            served = heap.top();
            heap.pop();
            served.first--;
        }
        cooling.push(served);
        // the ad served k calls before this one may be served from the next call on
        if ((int)cooling.size() > k) {
            if (cooling.front().second != 1) { heap.push(cooling.front()); }
            cooling.pop();
        }
        if (served.second == 1) { return ""; }
        return content[-served.second];
    }
};

void test(const string& label, const vector<pair<string, int>>& ads, int gets,
          const vector<string>& want) {
    AdServiceScan scan;
    AdService fast;
    for (const auto& [text, value] : ads) {
        scan.insertAd(text, value);
        fast.insertAd(text, value);
    }
    vector<string> gotScan;
    vector<string> gotFast;
    for (int call = 0; call < gets; call++) {
        gotScan.push_back(scan.getAd());
        gotFast.push_back(fast.getAd());
    }
    bool ok = gotScan == want && gotFast == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testCooldown(const string& label, const vector<pair<string, int>>& ads, int k,
                  int gets, const vector<string>& want) {
    AdServiceCooldown service(k);
    for (const auto& [text, value] : ads) { service.insertAd(text, value); }
    vector<string> got;
    for (int call = 0; call < gets; call++) { got.push_back(service.getAd()); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-repeat", {{"shoe", 10}, {"hat", 1}}, 3, {"shoe", "hat", "shoe"});
    test("tie-first", {{"shoe", 3}, {"hat", 3}}, 4, {"shoe", "hat", "shoe", "hat"});
    test("decrement", {{"shoe", 5}, {"hat", 4}, {"bag", 4}}, 4,
         {"shoe", "hat", "shoe", "bag"});
    test("single", {{"shoe", 1}}, 3, {"shoe", "", ""});
    test("empty", {}, 1, {""});

    AdServiceRemovable best;
    int shoe = best.insertAd("shoe", 9);
    best.insertAd("hat", 5);
    best.insertAd("bag", 4);
    best.removeAd(shoe);
    check(best.getAd() == "hat" && best.getAd() == "bag" && best.getAd() == "hat",
          "remove-best");
    AdServiceRemovable last;
    int top = last.insertAd("shoe", 5);
    last.insertAd("hat", 1);
    bool firstOk = last.getAd() == "shoe";
    last.removeAd(top);
    check(firstOk && last.getAd() == "hat" && last.getAd() == "", "remove-served-last");
    AdServiceRemovable twice;
    int only = twice.insertAd("shoe", 3);
    twice.removeAd(only);
    twice.removeAd(only);
    check(twice.getAd() == "", "remove-twice");

    testCooldown("cooldown-two", {{"shoe", 10}, {"hat", 5}, {"bag", 1}}, 2, 4,
                 {"shoe", "hat", "bag", "shoe"});
    testCooldown("cooldown-idle", {{"shoe", 3}, {"hat", 2}}, 2, 4,
                 {"shoe", "hat", "", "shoe"});
    testCooldown("cooldown-one", {{"shoe", 10}, {"hat", 1}}, 1, 3,
                 {"shoe", "hat", "shoe"});
}

// follow-up: removeAd(id) -> AdServiceRemovable above (or lazy deletion in the heap: mark
//   removed ids and skip them at the top and at the runner-up)
// follow-up: an ad may not repeat within the next k gets -> AdServiceCooldown above
// follow-up: updateScore(id, value) -> in AdServiceRemovable erase {score[id], -id},
//   set score[id] = value, insert {value, -id}
