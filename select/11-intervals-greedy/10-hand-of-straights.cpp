// LC 846 Hand of Straights (Medium) — greedy from smallest with an ordered count map
// Return whether the cards in hand can be split into groups of exactly groupSize cards
// with consecutive values.
// Clarify: every card must be used; duplicates allowed (they go to different groups);
//   groups have exactly groupSize cards, not at least; groupSize >= 1; value range.

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// Core idea: the smallest remaining card can only be the start of a group, so take it
// and the next groupSize - 1 values out of an ordered count map, and fail as soon as one
// of them is missing. T: O(n log n), S: O(n).
bool isNStraightHand(const vector<int>& hand, int groupSize) {
    if ((int)hand.size() % groupSize != 0) { return false; }
    // card value -> copies not placed in a group yet, smallest value first
    map<int, int> counts;
    for (int card : hand) { counts[card]++; }
    while (!counts.empty()) {
        int start = counts.begin()->first;
        for (int card = start; card < start + groupSize; card++) {
            auto it = counts.find(card);
            if (it == counts.end()) { return false; }
            it->second--;
            // erase at 0 so that begin() is always a card still to place
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

// follow-up (ideas only):
// - divide into sets of k consecutive numbers (LC 1296) -> the same problem
// - split into consecutive subsequences of length >= 3 (LC 659) -> for each value in
//   order, extend a run ending at value - 1 before starting a new one, which needs
//   value + 1 and value + 2 to exist
// - mahjong winning hand (Google) -> try each possible pair; for the rest, the smallest
//   tile must be in a triple or start a run, so peel those from the smallest tile
// - merge triplets to form the target (LC 1899) -> ignore every triplet that exceeds the
//   target in any coordinate, take the max of the rest and compare with the target
// - O(n) after sorting -> walk the distinct values with a queue of how many runs opened
//   at each of the last groupSize values; the count must cover the runs still open
