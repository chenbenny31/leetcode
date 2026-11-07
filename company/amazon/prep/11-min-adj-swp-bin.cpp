#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::count;

long long minSwaps(const vector<int>& arr) {
    long long toRight = 0, toLeft = 0;
    long long zerosLeft = 0;
    long long zeros = count(arr.begin(), arr.end(), 0);

    for (int v : arr) {
        if (v == 0) {
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

