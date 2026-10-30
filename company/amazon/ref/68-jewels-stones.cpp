// LC 771: jewels lists the jewel letters (case-sensitive). Return how many characters of
// stones are jewels.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// mark jewel letters in a table, then count. T: O(j + s), S: O(128).
int numJewelsInStones(const string& jewels, const string& stones) {
    // isJewel[c] = c is a jewel letter
    bool isJewel[128] = {};
    for (char c : jewels) { isJewel[(int)c] = true; }
    int count = 0;
    for (char c : stones) {
        if (isJewel[(int)c]) { count++; }
    }
    return count;
}

void test(const string& label, const string& jewels, const string& stones, int want) {
    cout << label << (numJewelsInStones(jewels, stones) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("case", "aB", "aAbBB", 3);
    test("none", "z", "abc", 0);
    test("empty", "a", "", 0);
}
