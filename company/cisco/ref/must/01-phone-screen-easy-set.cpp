// 01 · Phone Screen Easy Set (Cisco phone screen 2026-11, Easy) — about 4 of these
// (1) is s a palindrome            (2) values that appear once, in input order
// (3) first letter to appear twice (LC 2351)
// (4) middle node of a list (LC 876) (5) reverse a str in place
// (6) valid parentheses (LC 20)     reverse a list -> ref/must/02
//
// clarify:
// (1) case / non-letters -> compared as is; LC 125 rules are a follow-up
// (2) "once" -> exactly one occurrence in the whole array
// (3) no letter repeats -> return '\0' (LC guarantees one)
// (4) even len -> the second middle (LC 876)
// (6) brackets -> ()[]{}; other chars -> invalid input, not given

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::swap;

// ==================== (1) ====================
// is s a palindrome

// two pointers meet in the middle. T: O(n), S: O(1)
bool isPalindrome(const string& s) {
    int i = 0;
    int j = (int)s.size() - 1;
    while (i < j) {
        if (s[i] != s[j]) { return false; }
        i++;
        j--;
    }
    return true;
}

// ==================== (2) ====================
// values that appear exactly once, in input order

// count first, then keep values with count 1 in the original order. T: O(n), S: O(n)
vector<int> appearOnce(const vector<int>& nums) {
    // value -> occurrences
    unordered_map<int, int> cnt;
    for (int x : nums) { cnt[x]++; }
    vector<int> out;
    for (int x : nums) {
        if (cnt[x] == 1) { out.push_back(x); }
    }
    return out;
}

// ==================== (3) ====================
// first letter to appear twice

// the first char already seen is the answer. T: O(n), S: O(1)
char firstRepeated(const string& s) {
    vector<bool> seen(128, false);
    for (char c : s) {
        if (seen[(unsigned char)c]) { return c; }
        seen[(unsigned char)c] = true;
    }
    return '\0';
}

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// ==================== (4) ====================
// middle node of a list (the 2nd middle when len is even)

// slow moves 1, fast moves 2; slow is the middle when fast runs out. T: O(n), S: O(1)
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// ==================== (5) ====================
// reverse a str in place

// swap the two ends inward. T: O(n), S: O(1)
void reverseString(string& s) {
    int i = 0;
    int j = (int)s.size() - 1;
    while (i < j) {
        swap(s[i], s[j]);
        i++;
        j--;
    }
}

// ==================== (6) ====================
// valid brackets ()[]{}

// stack of the closers we still expect. T: O(n), S: O(n)
bool isValid(const string& s) {
    vector<char> expect;
    for (char c : s) {
        if (c == '(') {
            expect.push_back(')');
        } else if (c == '[') {
            expect.push_back(']');
        } else if (c == '{') {
            expect.push_back('}');
        } else {
            if (expect.empty() || expect.back() != c) { return false; }
            expect.pop_back();
        }
    }
    return expect.empty();
}

// ==================== tests ====================

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    bool pal = isPalindrome("racecar") && isPalindrome("") && !isPalindrome("ab");
    check(pal, "palindrome");
    check(appearOnce({4, 1, 2, 1, 3, 4}) == vector<int>({2, 3}), "appear-once-order");
    bool twice = firstRepeated("abccbaacz") == 'c' && firstRepeated("abc") == '\0';
    check(twice, "first-twice");

    vector<ListNode> nodes = {ListNode(1), ListNode(2), ListNode(3), ListNode(4)};
    for (int i = 0; i + 1 < 4; i++) { nodes[i].next = &nodes[i + 1]; }
    bool even = middleNode(&nodes[0])->val == 3;
    nodes[2].next = nullptr;
    check(even && middleNode(&nodes[0])->val == 2, "middle-even-and-odd");

    string word = "cisco";
    reverseString(word);
    check(word == "ocsic", "reverse-string");
    check(isValid("([]{})") && !isValid("(]") && !isValid("(("), "valid-parens");
    return 0;
}

// follow-up:
// 1. palindrome, alnum only, any case (LC 125) -> skip non-alnum, compare tolower
// 2. palindrome after one deletion (LC 680) -> on the 1st mismatch try skipping each side
// 3. first unique char (LC 387) -> same count pass, return the first idx with count 1
// 4. middle without two pointers -> count n, walk n / 2
// 5. reverse words in place -> reverse the whole str, then each word
// 6. in C -> char* + strlen, no std::str; the same idx loops work
