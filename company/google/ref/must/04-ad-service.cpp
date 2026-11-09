// AdService: insertAd(content, score) adds an ad; getAd() returns the content of the ad
// with the highest score, never the same ad twice in a row, and lowers the returned ad's
// score by 1. Ties go to the ad inserted first. getAd() returns "" when no ad can be
// served (no ads, or the only ad was served last time).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;

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

int main() {
    test("no-repeat", {{"shoe", 10}, {"hat", 1}}, 3, {"shoe", "hat", "shoe"});
    test("tie-first", {{"shoe", 3}, {"hat", 3}}, 4, {"shoe", "hat", "shoe", "hat"});
    test("decrement", {{"shoe", 5}, {"hat", 4}, {"bag", 4}}, 4,
         {"shoe", "hat", "shoe", "bag"});
    test("single", {{"shoe", 1}}, 2, {"shoe", ""});
    test("empty", {}, 1, {""});
}

// follow-up: removeAd(id) -> mark removed and skip such entries when popping (lazy
//   deletion), or keep a set<pair<int, int>> and erase the exact entry
// follow-up: an ad may not repeat within the last k gets -> hold served ads in a queue
//   for k calls before pushing them back into the heap
