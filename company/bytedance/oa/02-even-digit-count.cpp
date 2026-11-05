// ByteDance OA: numbers holds positive integers (1 <= numbers[i] <= 1e4). Count how
// many of them have an even number of decimal digits.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::to_string;

// Digit count = length of the decimal string. T: O(n log max), S: O(log max).
int countEvenDigitNumbers(const vector<int>& numbers) {
    int count = 0;
    for (int x : numbers) {
        if (to_string(x).size() % 2 == 0) {
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
    test("power-of-ten", {99, 100}, 1);
}

// follow-up: no strings -> count digits by division
//   int digits = 0;
//   while (x > 0) { digits++; x /= 10; }
// follow-up: O(1) per number -> range check: (10 <= x <= 99) || (1000 <= x <= 9999)
// follow-up: count in [lo, hi] without scanning -> sum even-length blocks
//   [10^(2k-1), 10^(2k)) clipped to [lo, hi], O(digits)
