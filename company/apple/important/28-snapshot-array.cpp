// LC 1146: SnapshotArray(length) of zeros; set(index, val); snap() returns the snap id
// (0, 1, ...); get(index, snapId) returns the value at index when that snap was taken.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

class SnapshotArray {
    int snapCount = 0;   // snaps taken so far = id of the snap currently being edited
    // data[i] = [{snapId, val}, ...] changes of index i, snapId strictly increasing
    vector<vector<pair<int, int>>> data;

public:
    // every index starts as 0 from snap 0. S: O(length + sets).
    SnapshotArray(int length) : data(length, vector<pair<int, int>>{{0, 0}}) {}

    // Overwrite if this index already changed in the current snap. T: O(1).
    void set(int index, int val) {
        auto& row = data[index];
        if (row.back().first == snapCount) {
            row.back().second = val;
        } else {
            row.push_back({snapCount, val});
        }
    }

    // T: O(1).
    int snap() {
        snapCount++;
        return snapCount - 1;
    }

    // Binary search for the last change at or before snapId. T: O(log sets).
    int get(int index, int snapId) {
        const auto& row = data[index];
        int lo = 0, hi = row.size();   // first index with snap > snapId is in [lo, hi]
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (row[mid].first <= snapId) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        // lo >= 1 always: row[0] is snap 0
        return row[lo - 1].second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SnapshotArray a(3);
    a.set(0, 5);
    check(a.snap() == 0, "snap-id");
    a.set(0, 6);
    check(a.get(0, 0) == 5, "old");
    a.set(0, 7);
    check(a.snap() == 1 && a.get(0, 1) == 7, "overwrite");
    a.snap();
    check(a.get(0, 2) == 7 && a.get(1, 2) == 0, "untouched");
}
