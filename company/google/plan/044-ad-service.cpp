// AdService — max-heap w/ a no-repeat rule
// (1) insertAd(content, score); getAd() = content of max-score ad, then its score -= 1
// (1) never the same ad twice in a row; ties -> inserted first; "" if none servable
// (2) removeAd(id)
// (3) a served ad is blocked for the next k calls
//
// clarify:
// tie rule -> ad inserted first
// does a "" call unblock the ad served last -> assume no
// scores below 0 -> allowed, keep decrementing
// same text twice -> two ads
// (3) do calls returning "" cnt toward k -> assume yes

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

// (1) insertAd / getAd
// solution 1: brute force; baseline to state first
// core idea: scan all ads, skip the one served last
// T: O(n) getAd, S: O(n)
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

// solution 2: max-heap; the expected answer
// core idea: max-heap of (score, -id) -> lower id wins ties; one heap entry per ad
// top was served last -> serve the runner-up, push the top back
// T: O(log n) per call, S: O(n)
class AdService {
    // content[id] = text of ad id, ids in insertion order
    vector<string> content;
    // heap = (score, -id); top = highest score, earliest insert on ties
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

// (2) removeAd
// core idea: ordered set of (score, -id) can erase any exact entry -> no lazy deletion
// best ad = last element, runner-up = the one before it
// T: O(log n) per call, S: O(n)
class AdServiceRemovable {
    // content[id] / score[id] of ad id, ids in insertion order
    vector<string> content;
    vector<int> score;
    // ranked = (score, -id) of every ad not removed; best ad = last element
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

// (3) cooldown of k calls
// core idea: heap = servable ads; a served ad waits in a queue for k calls, then returns
// a call serving nothing still queues a marker -> blocked ads do not wait too long
// T: O(log n) per call, S: O(n + k)
class AdServiceCooldown {
    // content[id] = text of ad id, ids in insertion order
    vector<string> content;
    // heap = (score, -id) of ads that may be served now
    priority_queue<pair<int, int>> heap;
    // cooling = (score, -id) served by each of the last k calls, oldest first
    // {0, 1} = served nothing (real -ids are <= 0)
    queue<pair<int, int>> cooling;
    // k = calls an ad stays blocked after it is served
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
        // ad served k calls ago is servable again from the next call on
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

// follow-up:
// (2) w/ the heap -> lazy deletion: mark removed ids, skip them at top and runner-up
// updateScore(id, value) -> erase {score[id], -id}, set score[id], insert {value, -id}
// a "" call unblocks the last ad -> reset lastId to -1 when returning ""
