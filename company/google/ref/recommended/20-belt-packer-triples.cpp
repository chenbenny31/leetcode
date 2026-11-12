// Belt Packer Triples (Google, interview report)
// BeltPacker(threshold); items with integer sizes arrive one at a time, in any order.
// addItem(size) stores the item; if afterwards some three stored items have
// max - min < threshold, remove them and return their sizes ascending, else return {}.
// Assumed: if several triples qualify, remove the one with the smallest sizes.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::multiset;
using std::prev;

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

void test(const string& label, int threshold, const vector<int>& adds,
          const vector<vector<int>>& want) {
    BeltPacker packer(threshold);
    vector<vector<int>> got;
    for (int size : adds) { got.push_back(packer.addItem(size)); }
    bool ok = got == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("in-order", 3, {1, 2, 3}, {{}, {}, {1, 2, 3}});
    test("new-min", 3, {11, 12, 10}, {{}, {}, {10, 11, 12}});
    test("new-middle", 3, {1, 10, 12, 11, 2, 3},
         {{}, {}, {}, {10, 11, 12}, {}, {1, 2, 3}});
    test("strict-gap", 2, {1, 2, 3}, {{}, {}, {}});
    test("duplicates", 1, {5, 5, 5, 5}, {{}, {}, {5, 5, 5}, {}});
}

// follow-up: 2D sizes, a triple is valid when every pairwise Euclidean distance is
//   < threshold -> keep items sorted by x; only items with |dx| < threshold from the new
//   one are candidates; try every pair (a, b) among them with dist(a, b) < threshold too
// follow-up: Python list + bisect.insort -> O(log n) search but O(n) shifting per insert;
//   the multiset (balanced BST) inserts and erases in O(log n)
