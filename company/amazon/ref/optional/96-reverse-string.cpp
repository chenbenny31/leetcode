// LC 344: reverse a character array in place.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// swap the two ends and move inward. T: O(n), S: O(1).
void reverseString(vector<char>& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        swap(s[i], s[j]);
        i++;
        j--;
    }
}

void test(const string& label, vector<char> s, const vector<char>& want) {
    reverseString(s);
    cout << label << (s == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", {'a', 'b', 'c'}, {'c', 'b', 'a'});
    test("even", {'a', 'b'}, {'b', 'a'});
    test("empty", {}, {});
}
