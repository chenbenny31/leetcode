// boxIds is a string of digits. One operation removes a digit d and inserts min(d + 1, 9)
// anywhere; it can be repeated any number of times. Return the lexicographically
// smallest string you can reach.

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::sort;
using std::min;

// A digit with a smaller digit somewhere to its right blocks it, so move it (once is
// enough, since it can be placed anywhere); digits with nothing smaller to their right
// form a non-decreasing sequence and stay. All final digits can then be laid out in
// sorted order. T: O(n log n) (O(n) with counting sort), S: O(n).
string smallestIds(const string& s) {
    string result = s;
    char suffixMin = '9' + 1;
    int n = s.size();
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] > suffixMin) {
            result[i] = min(s[i] + 1, (int)'9');
        } else {
            suffixMin = s[i];
        }
    }
    sort(result.begin(), result.end());
    return result;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (smallestIds(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("falling", "321", "134");
    test("cap-nine", "909", "099");
    test("sorted", "1234", "1234");
    test("two", "10", "02");
}
