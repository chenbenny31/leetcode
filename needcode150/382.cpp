// reservoir sampling, T: O(n) per call, S: O(1)

#include <cstdlib> // std::rand

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
    ListNode* head;

public:
    Solution(ListNode* head) : head(head) {}

    int getRandom() {
        int res = 0;
        int i = 1;
        ListNode* curr = head;

        while (curr) {
            if (std::rand() % i == 0) { res = curr->val; }
            curr = curr->next;
            ++i;
        }
        return res;
    }
};

// reservoir sampling correctness: after proc k nodes, each has 1/k prob
// std::rand() % i == 0: prob 1/i
// extend to k random nodes:
std::vector<int> getKRandom(int k) {
    std::vector<int> reservoir;
    reservoir.reserve(k);
    ListNode* curr = head;
    int i = 1;
    
    while (curr) {
        if (static_cast<int>(reservoir.size()) < k) {
            reservoir.push_back(curr->val);
        } else {
            int j = std::rand() % i;
            if (j < k) { reservoir[j] = curr->val; }
        }

        curr = curr->next;
        i++;
    }
    return reservoir;
}
// j < k: j is uniform in [0, i-1], prob of replacement is k/i
