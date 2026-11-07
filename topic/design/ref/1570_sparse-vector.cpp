// LC 1570 Dot Product of Two Sparse Vectors: store a mostly-zero vector compactly and
// compute its dot product with another vector of the same length.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::pair;
using std::string;
using std::vector;

class SparseVector {
    // (index, value) of every non-zero entry, index increasing
    vector<pair<int, int>> items;

public:
    // T: O(n)
    SparseVector(const vector<int>& nums) {
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] != 0) { items.push_back({i, nums[i]}); }
        }
    }

    // two pointers over the sorted non-zero lists.
    // T: O(a + b) for a and b non-zeros, S: O(a).
    int dotProduct(const SparseVector& vec) {
        int n = items.size();
        int m = vec.items.size();
        int sum = 0;
        int i = 0, j = 0;

        while (i < n && j < m) {
            if (items[i].first == vec.items[j].first) {
                sum += items[i].second * vec.items[j].second;
                i++;
                j++;
            } else if (items[i].first < vec.items[j].first) {
                i++;
            } else {
                j++;
            }
        }

        return sum;
    }
};

void test(const string& label, const vector<int>& a, const vector<int>& b, int want) {
    SparseVector x(a), y(b);
    cout << label << (x.dotProduct(y) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {1, 0, 0, 2, 3}, {0, 3, 0, 4, 0}, 8);
    test("disjoint", {1, 0, 2}, {0, 5, 0}, 0);
    test("all-zero", {0, 0, 0}, {1, 2, 3}, 0);
}
