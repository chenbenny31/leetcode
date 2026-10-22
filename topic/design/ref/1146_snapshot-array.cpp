// LC 1146 Snapshot Array: SnapshotArray(length) starts as zeros; set(index, val);
// snap() returns the snap id (0, 1, ...); get(index, snapId) returns the value at index
// when that snap was taken.
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::map;
using std::string;
using std::vector;

class SnapshotArray {
    int snapCount = 0;   // snaps taken so far = id of the snap being edited
    // history[i] = snapId -> value index i was set to during that snap (only changes)
    vector<map<int, int>> history;

public:
    SnapshotArray(int length) : history(length) {}

    // T: O(log sets of that index)
    void set(int index, int val) {
        history[index][snapCount] = val;
    }

    // no copying: a snap is just a counter. T: O(1).
    int snap() {
        snapCount++;
        return snapCount - 1;
    }

    // last change at or before snapId (upper_bound, then step back).
    // T: O(log sets), S: O(total sets).
    int get(int index, int snapId) {
        const map<int, int>& h = history[index];
        auto it = h.upper_bound(snapId);
        if (it == h.begin()) { return 0; }
        --it;
        return it->second;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    SnapshotArray a(3);
    a.set(0, 5);
    check(a.snap() == 0, "snap-id");
    a.set(0, 6);
    check(a.get(0, 0) == 5, "old-value");
    a.set(0, 7);
    check(a.snap() == 1 && a.get(0, 1) == 7, "overwrite");
    a.snap();
    check(a.get(0, 2) == 7 && a.get(1, 2) == 0, "untouched");
}
