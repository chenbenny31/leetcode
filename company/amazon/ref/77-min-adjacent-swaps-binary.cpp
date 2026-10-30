// Given a 0/1 array, return the min number of adjacent swaps to make it 000...111 or
// 111...000, whichever is cheaper. (LC 2938 asks only for 000...111.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::count;

// Each swap moves one 1 past one 0. To push all 1s right, every 1 must pass every 0 on
// its right; to push them left, every 0 on its left. T: O(n), S: O(1).
long long minSwaps(const vector<int>& arr) {
    long long toRight = 0, toLeft = 0;
    // zeros seen so far (to the left of i)
    long long zerosLeft = 0;
    long long zeros = count(arr.begin(), arr.end(), 0);
    for (int x : arr) {
        if (x == 0) {
            zerosLeft++;
        } else {
            toLeft += zerosLeft;
            toRight += zeros - zerosLeft;
        }
    }
    return min(toLeft, toRight);
}

void test(const string& label, const vector<int>& arr, long long want) {
    cout << label << (minSwaps(arr) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("alt", {0, 1, 0, 1}, 1);
    test("left-done", {1, 1, 0}, 0);
    test("both-equal", {1, 0, 0, 1}, 2);
    test("middle", {1, 0, 1, 0, 1}, 3);
    test("one", {0}, 0);
}
