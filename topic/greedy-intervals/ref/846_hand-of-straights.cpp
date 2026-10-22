// LC 846 Hand of Straights: can the cards be rearranged into groups of groupSize
// consecutive values, using every card exactly once?
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::string;
using std::vector;
using std::map;

// Greedy: the smallest remaining card must start a group, so take count of it groups
// at once. T: O(n log n), S: O(n).
bool isNStraightHand(const vector<int>& hand, int groupSize) {
    int n = hand.size();
    if (n % groupSize != 0) { return false; }
    map<int, int> cnt;   // card value -> copies still unused, in value order
    for (int card : hand) { cnt[card]++; }
    for (auto& [card, c] : cnt) {
        if (c == 0) { continue; }
        for (int k = 1; k < groupSize; k++) {
            auto it = cnt.find(card + k);
            if (it == cnt.end() || it->second < c) { return false; }
            it->second -= c;
        }
    }
    return true;
}

void test(const string& label, const vector<int>& hand, int groupSize, bool want) {
    cout << label << (isNStraightHand(hand, groupSize) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap-groups", {1, 2, 2, 3}, 2, true);
    test("gap", {1, 2, 3, 5}, 2, false);
    test("not-divisible", {1, 2, 3}, 2, false);
    test("size-one", {5, 1}, 1, true);
}
