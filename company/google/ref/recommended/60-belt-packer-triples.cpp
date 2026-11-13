// Belt Packer Triples (Google, interview report)
// BeltPacker(threshold); items with integer sizes arrive one at a time, in any order.
// addItem(size) stores the item; if afterwards some three stored items have
// max - min < threshold, remove them and return their sizes ascending, else return {}.
// Assumed: if several triples qualify, remove the one with the smallest sizes.
//
// Clarify:
// - strict? -> max - min < threshold; a gap equal to threshold does not qualify
// - duplicate sizes -> separate items (multiset); three equal sizes qualify if
//   threshold >= 1
// - several triples qualify -> the one with the smallest sizes (assumed above)
// - what to return -> the removed sizes ascending, {} if nothing is removed
// - up to ~1e5 adds -> O(log n) per add; trying all triples or scanning is too slow
// Idea: after every add no valid triple is stored, so a new one must contain the new
//   item. Any valid triple with it spans at least one run of 3 sorted neighbors that
//   includes it and is no wider, so only the 3 windows of neighbors in the 5-item slice
//   around the new item need checking.
// Pitfalls:
// - walk back with prev only while it != begin(); near the ends the slice is shorter
//   than 5, so loop while start + 2 < window.size()
// - erase by iterator: erase(value) removes every duplicate of that size
// - copy the three sizes before erasing their iterators
// - multiset::insert returns an iterator (set::insert returns a pair)
// Tests to write: triple completed by a new max / a new min / a new middle; a gap equal
//   to threshold -> nothing; four equal sizes -> three removed, the fourth stays.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <utility>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::multiset;
using std::prev;
using std::pair;
using std::sort;

class BeltPacker {
    // sizes of the stored items, ascending, duplicates allowed
    multiset<int> sizes;
    int threshold;

public:
    BeltPacker(int threshold) : threshold(threshold) {}

    // before an add no valid triple exists, so a valid one must contain the new item, and
    // the tightest triple around it is three neighbors in sorted order: check the windows
    // (x-2, x-1, x), (x-1, x, x+1), (x, x+1, x+2). T: O(log n) per add, S: O(n).
    vector<int> addItem(int size) {
        auto pos = sizes.insert(size);
        auto it = pos;
        int before = 0;
        while (before < 2 && it != sizes.begin()) {
            it = prev(it);
            before++;
        }
        // up to two items before the new one, the new one, up to two items after it
        vector<multiset<int>::iterator> window;
        while (it != sizes.end() && (int)window.size() < before + 3) {
            window.push_back(it);
            it++;
        }
        for (int start = 0; start + 2 < (int)window.size(); start++) {
            if (*window[start + 2] - *window[start] < threshold) {
                vector<int> removed = {*window[start], *window[start + 1],
                                       *window[start + 2]};
                for (int d = 0; d < 3; d++) { sizes.erase(window[start + d]); }
                return removed;
            }
        }
        return {};
    }
};

// true if the squared Euclidean distance of a and b is below threshold^2 (no sqrt).
// T: O(1), S: O(1).
bool withinDistance(const pair<int, int>& a, const pair<int, int>& b, int threshold) {
    long long dx = (long long)a.first - b.first;
    long long dy = (long long)a.second - b.second;
    return dx * dx + dy * dy < (long long)threshold * threshold;
}

// follow-up (2D): items are points (x, y); a triple is valid when all three pairwise
// distances are < threshold. Same argument: a new valid triple contains the new point,
// so only stored points with |dx| < threshold that are close to it can join it; try
// every pair of them. Several triples -> the first pair in (x, y) order. The new point
// is stored only when no triple is found.
// T: O(log n + c^2) per add with c close points, S: O(n).
// Pitfalls: check the third side (a, b) too, not only the two sides to the new point;
//   compare squared distances in long long; |dx| < threshold is only a prune.
// Tests to write: right triangle -> removed; two points too far apart -> kept until a
//   point joins one of them; a side exactly threshold long -> not valid.
class BeltPacker2D {
    // stored points (x, y), sorted by x then y
    multiset<pair<int, int>> points;
    int threshold;

public:
    BeltPacker2D(int threshold) : threshold(threshold) {}

    vector<pair<int, int>> addItem(int x, int y) {
        pair<int, int> item = {x, y};
        // stored points within threshold of the new one, in (x, y) order
        vector<multiset<pair<int, int>>::iterator> close;
        auto it = points.lower_bound({x - threshold + 1, INT_MIN});
        while (it != points.end() && it->first < x + threshold) {
            if (withinDistance(*it, item, threshold)) { close.push_back(it); }
            it++;
        }
        for (int a = 0; a < (int)close.size(); a++) {
            for (int b = a + 1; b < (int)close.size(); b++) {
                if (!withinDistance(*close[a], *close[b], threshold)) { continue; }
                vector<pair<int, int>> removed = {*close[a], *close[b], item};
                sort(removed.begin(), removed.end());
                points.erase(close[a]);
                points.erase(close[b]);
                return removed;
            }
        }
        points.insert(item);
        return {};
    }
};

void test(const string& label, int threshold, const vector<int>& adds,
          const vector<vector<int>>& want) {
    BeltPacker packer(threshold);
    vector<vector<int>> got;
    for (int size : adds) { got.push_back(packer.addItem(size)); }
    bool ok = got == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void test2D(const string& label, int threshold, const vector<pair<int, int>>& adds,
            const vector<vector<pair<int, int>>>& want) {
    BeltPacker2D packer(threshold);
    vector<vector<pair<int, int>>> got;
    for (auto [x, y] : adds) { got.push_back(packer.addItem(x, y)); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("in-order", 3, {1, 2, 3}, {{}, {}, {1, 2, 3}});
    test("new-min", 3, {11, 12, 10}, {{}, {}, {10, 11, 12}});
    test("new-middle", 3, {1, 10, 12, 11, 2, 3},
         {{}, {}, {}, {10, 11, 12}, {}, {1, 2, 3}});
    test("strict-gap", 2, {1, 2, 3}, {{}, {}, {}});
    test("duplicates", 1, {5, 5, 5, 5}, {{}, {}, {5, 5, 5}, {}});

    test2D("2d-triangle", 5, {{0, 0}, {3, 0}, {0, 3}},
           {{}, {}, {{0, 0}, {0, 3}, {3, 0}}});
    test2D("2d-far-pair", 5, {{0, 0}, {4, 0}, {-4, 0}, {1, 1}},
           {{}, {}, {}, {{0, 0}, {1, 1}, {4, 0}}});
    test2D("2d-side-equal", 5, {{0, 0}, {3, 4}, {0, 1}}, {{}, {}, {}});
}

// follow-up: 2D sizes, a triple is valid when every pairwise Euclidean distance is
//   < threshold -> BeltPacker2D above
// follow-up: groups of g items instead of 3 -> same argument with g sorted neighbors:
//   walk back while before < g - 1, collect while window.size() < before + g, test
//     *window[start + g - 1] - *window[start] < threshold
//   and erase window[start .. start + g - 1]. T: O(g + log n) per add.
// follow-up: Python list + bisect.insort -> O(log n) search but O(n) shifting per insert;
//   the multiset (balanced BST) inserts and erases in O(log n)
