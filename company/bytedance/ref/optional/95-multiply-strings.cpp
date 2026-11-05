// ByteDance / LC 43: multiply two non-negative integers given as strings and return the
// product as a string, without converting the inputs to integers.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Grade-school multiply into digit slots, then one carry pass. T: O(m*n), S: O(m+n).
string multiply(const string& num1, const string& num2) {
    int m = num1.size();
    int n = num2.size();
    // pos[k] = digit sum at place k, most significant first
    vector<int> pos(m + n, 0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // digits i and j land at slot i+j+1; i+j takes the carry
            pos[i + j + 1] += (num1[i] - '0') * (num2[j] - '0');
        }
    }
    for (int k = m + n - 1; k > 0; k--) {
        pos[k - 1] += pos[k] / 10;
        pos[k] %= 10;
    }
    string result;
    for (int d : pos) {
        // skip leading zeros
        if (result.empty() && d == 0) { continue; }
        result += char('0' + d);
    }
    return result.empty() ? "0" : result;
}

void test(const string& label, const string& a, const string& b, const string& want) {
    cout << label << (multiply(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", "2", "3", "6");
    test("lc-ex2", "123", "456", "56088");
    test("zero", "0", "52", "0");
}

// follow-up: add two strings (LC 415) -> i, j from the end with carry, O(m+n)
// follow-up: negative inputs -> strip '-', multiply magnitudes, prepend '-' if exactly
//   one is negative and the product != "0"
// follow-up: huge inputs -> Karatsuba O(n^1.585), or FFT/NTT digit convolution O(n log n)
