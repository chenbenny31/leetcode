// ByteDance OA: numbers holds positive integers (1 <= numbers[i] <= 1e4). Count how
// many of them have an even number of decimal digits.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Number of decimal digits of a positive x.
int digitCount(int x) {
    int count = 0;
    while (x > 0) {
        count++;
        x /= 10;
    }
    return count;
}

// Count digits of each number by repeated division. T: O(n log max), S: O(1).
int countEvenDigitNumbers(const vector<int>& numbers) {
    int count = 0;
    for (int x : numbers) {
        if (digitCount(x) % 2 == 0) {
            count++;
        }
    }
    return count;
}

void test(const string& label, const vector<int>& numbers, int want) {
    cout << label << (countEvenDigitNumbers(numbers) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed-lengths", {1, 22, 333, 4444, 10000}, 2);
    test("single-odd", {7}, 0);
    test("power-of-ten-edge", {99, 100}, 1);
}
