// ByteDance intern OA (1p3a 1189035, Q2): count pairs i < j of positive integers in
// nums that have the same number of digits and where one can be turned into the other
// by a cyclic shift of its digits (546 -> 654 is a shift; 546 -> 456 is not, and 120
// does not pair with 12 since the digit count must stay the same).
// Assumed: equal numbers count as a pair (shift by 0); the post did not say.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::to_string;

// Smallest of all rotations of s; two digit strings are rotations of each other iff
// they have the same smallest rotation. T: O(L^2) for L digits.
string minRotation(const string& s) {
    string best = s;
    for (int i = 1; i < (int)s.size(); i++) {
        string rotated = s.substr(i) + s.substr(0, i);
        if (rotated < best) {
            best = rotated;
        }
    }
    return best;
}

// Group numbers by canonical rotation (the string length keeps the digit count);
// each new member pairs with every earlier member of its group.
// T: O(n * L^2), S: O(n * L).
long long countCyclicShiftPairs(const vector<int>& nums) {
    // seen[key] = numbers so far whose smallest rotation is key
    unordered_map<string, long long> seen;
    long long pairs = 0;
    for (int x : nums) {
        string key = minRotation(to_string(x));
        pairs += seen[key];
        seen[key]++;
    }
    return pairs;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (countCyclicShiftPairs(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-related", {13, 31, 13}, 3);
    test("rotation-not-perm", {546, 654, 456}, 1);
    test("digit-count", {120, 12}, 0);
    test("zero-inside", {102, 210, 21}, 1);
    test("single", {7}, 0);
}
