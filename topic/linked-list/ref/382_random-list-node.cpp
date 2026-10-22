// LC 382 Linked List Random Node: given a singly linked list, getRandom returns the value
// of a uniformly random node; each node must be equally likely.
// Follow-up: the list is very long and its length is unknown, without extra space.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using std::cout;
using std::rand;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Reservoir sampling of size 1: the i-th node replaces the pick with probability 1/i,
// so after n nodes each one is kept with probability 1/i * i/(i+1) * ... = 1/n.
class RandomNode {
    ListNode* head;   // list to sample from, not owned

public:
    RandomNode(ListNode* h) : head(h) {}

    // T: O(L), S: O(1).
    int getRandom() {
        int pick = head->val;
        int seen = 0;   // nodes visited so far
        for (ListNode* cur = head; cur; cur = cur->next) {
            seen++;
            if (rand() % seen == 0) { pick = cur->val; }
        }
        return pick;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<ListNode> one(1, ListNode(5));
    RandomNode single(&one[0]);
    bool same = true;
    for (int i = 0; i < 10; i++) {
        same = same && single.getRandom() == 5;
    }
    check(same, "single");

    vector<ListNode> three(3, ListNode(0));
    for (int i = 0; i < 3; i++) {
        three[i].val = i + 1;
        three[i].next = i + 1 < 3 ? &three[i + 1] : nullptr;
    }
    RandomNode pick(&three[0]);
    vector<int> cnt(4, 0);   // cnt[v] = times value v was returned
    for (int i = 0; i < 3000; i++) {
        int v = pick.getRandom();
        if (v >= 1 && v <= 3) { cnt[v]++; }
    }
    check(cnt[1] + cnt[2] + cnt[3] == 3000, "in-list");
    check(cnt[1] > 800 && cnt[1] < 1200 && cnt[2] > 800 && cnt[2] < 1200
              && cnt[3] > 800 && cnt[3] < 1200, "uniform");
}
