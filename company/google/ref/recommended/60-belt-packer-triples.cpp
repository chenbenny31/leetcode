// Belt Packer Triples (Google, interview report)
// (1) addItem(size): if 3 stored items have max - min < threshold, remove + return them
// (2) 2D: items are points; a triple is valid when all 3 pairwise distances < threshold
//
// clarify:
// strict < threshold -> yes
// dup sizes -> separate items
// several triples qualify -> assumed the smallest sizes
// return value -> removed sizes ascending, else {}
// num of adds -> need O(log n) per add

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

// (1) 1D sizes
// core idea: no valid triple is stored after any add -> a new one contains the new item x
// tightest triple = 3 sorted neighbors: (x-2, x-1, x), (x-1, x, x+1), (x, x+1, x+2)
// T: O(log n) per add, S: O(n)
class BeltPacker {
    // sizes of the stored items, ascending, dups allowed
    multiset<int> sizes;
    int threshold;

public:
    BeltPacker(int threshold) : threshold(threshold) {}

    vector<int> addItem(int size) {
        auto pos = sizes.insert(size);
        auto it = pos;
        int before = 0;
        while (before < 2 && it != sizes.begin()) {
            it = prev(it);
            before++;
        }
        // up to 2 items before the new one, the new one, up to 2 after it
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

// squared distance of a and b is below threshold^2 (no sqrt)
bool withinDistance(const pair<int, int>& a, const pair<int, int>& b, int threshold) {
    long long dx = (long long)a.first - b.first;
    long long dy = (long long)a.second - b.second;
    return dx * dx + dy * dy < (long long)threshold * threshold;
}

// (2) 2D points
// core idea: a new triple contains the new point -> try every pair of stored points near
// T: O(log n + c^2) per add, S: O(n) — c = stored points w/ |dx| < threshold
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

// follow-up:
// groups of g instead of 3 -> windows of g inside the 2g - 1 items around x, O(g + log n)
// Python list + bisect.insort -> O(n) shift per insert; multiset does it in O(log n)
