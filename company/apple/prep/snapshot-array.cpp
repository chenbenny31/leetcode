// SnapshopArray(length) of zeros; set(), snap() returns the snap id
// get() return the value at index that snap was taken

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::pair;

class SnapshotArray {
private:
    int snapCount = 0; // id for next snap() returns
    vector<vector<pair<int, int>>> data; // data[i] = [{snapId, val}, ...]

public:
    SnapshotArray(int length)
        : data(length, vector<pair<int, int>>{{0, 0}}) {}

    // overwrite if this index changes since last snap, T: O(1)
    void set(int index, int val) {
        auto& row = data[index];
        if (row.back().first == snapCount) {
            row.back().second = val;
        } else {
            row.push_back({snapCount, val});
        }
    }

    int snap() {
        snapCount++;
        return snapCount - 1;
    }

    // T: O(logk), k=set-size, binary-search for last change at/before snapId
    int get(int index, int snapId) {
        const auto& row = data[index];
        int lo = 0;
        int hi = row.size();
        while (lo < hi) { // upperBound: first snap > snapId
            int mid = lo + (hi - lo) / 2;
            if (row[mid].first <= snapId) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return row[lo - 1].second; // lo >= 1 always, row[0] is snap 0
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
