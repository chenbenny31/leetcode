// Sort an array in place, ascending, without the library sort. Compare a simple O(n^2)
// sort with an O(n log n) one.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// straightforward: insertion sort grows a sorted prefix; stable, fast on nearly sorted
// input. T: O(n^2) worst, O(n) best. S: O(1).
void insertionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int x = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

// Lomuto partition around the middle element: returns the pivot's final index
int partition(vector<int>& a, int lo, int hi) {
    swap(a[lo + (hi - lo) / 2], a[hi]);
    int pivot = a[hi];
    int store = lo;
    for (int i = lo; i < hi; i++) {
        if (a[i] < pivot) {
            swap(a[i], a[store]);
            store++;
        }
    }
    swap(a[store], a[hi]);
    return store;
}

void quickSort(vector<int>& a, int lo, int hi) {
    if (lo >= hi) { return; }
    int p = partition(a, lo, hi);
    quickSort(a, lo, p - 1);
    quickSort(a, p + 1, hi);
}

// improved: quicksort; not stable, O(n^2) worst case on bad pivots.
// T: O(n log n) average, S: O(log n) average stack.
void quickSort(vector<int>& a) { quickSort(a, 0, (int)a.size() - 1); }

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    vector<int> a = nums, b = nums;
    insertionSort(a);
    quickSort(b);
    cout << label << (a == want && b == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {4, 1, 3, 2}, {1, 2, 3, 4});
    test("dups", {2, 1, 2, 1}, {1, 1, 2, 2});
    test("sorted", {1, 2, 3}, {1, 2, 3});
    test("reverse", {3, 2, 1}, {1, 2, 3});
    test("empty", {}, {});
}
