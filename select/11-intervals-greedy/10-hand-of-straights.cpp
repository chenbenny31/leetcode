// LC 846 Hand of Straights (Medium) — greedy from smallest with an ordered count map
// return if hand splits into groups of exactly groupSize cards w/ consecutive values
//
// clarify:
// every card must be used -> size % groupSize != 0 = false
// dup cards -> ok, they go to different groups
// exactly groupSize, not at least -> else LC 659
// groupSize >= 1 -> no divide by 0
// value range large -> ordered map, not array

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// core idea: smallest remaining card must start a group; take it + next groupSize - 1
// T: O(n log n), S: O(n)
bool isNStraightHand(const vector<int>& hand, int groupSize) {
    if ((int)hand.size() % groupSize != 0) { return false; }
    // card value -> copies not grouped yet, smallest first
    map<int, int> counts;
    for (int card : hand) { counts[card]++; }
    while (!counts.empty()) {
        int start = counts.begin()->first;
        for (int card = start; card < start + groupSize; card++) {
            auto it = counts.find(card);
            if (it == counts.end()) { return false; }
            it->second--;
            // erase at 0 so begin() is always a card still to place
            if (it->second == 0) { counts.erase(it); }
        }
    }
    return true;
}

void test(const string& label, const vector<int>& hand, int groupSize, bool want) {
    cout << label << (isNStraightHand(hand, groupSize) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlapping-runs", {3, 2, 4, 1, 2, 3}, 3, true);
    test("gap", {1, 2, 4}, 3, false);
    test("duplicate-start", {1, 1, 2, 3}, 2, false);
    test("size-not-divisible", {1, 2, 3, 4}, 3, false);
    return 0;
}

// follow-up:
// sets of k consecutive nums (LC 1296) -> same problem
// consecutive subsequences of len >= 3 (LC 659) -> extend a run ending at v - 1 first
// mahjong winning hand (Google) -> try each pair; rest: peel triple / run from smallest
// merge triplets to form target (LC 1899) -> drop triplets above target, max the rest
// O(n) after sorting -> queue of runs opened at each of the last groupSize values
