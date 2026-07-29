// iter

#include <vector>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
private:
    ListNode* merge2Lists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }

public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) { return nullptr; } 
        int n = static_cast<int>(lists.size());

        for (int step = 1; step < n; step *= 2) {
            for (int i = 0; i + step < n; i += 2 * step) {  // i += 2 * step to skip paired partner
                lists[i] = merge2Lists(list[i], list[i + step]); // (i, i + step) is partner
            }
        }
        return lists[0];
    }
};

// recur

#include <vector>

class Solution {
private:
    ListNode* merge2Lists(ListNode* l1, ListNode* l2) {
        if (!l1) { return l2; }
        if (!l2) { return l1; }
        if (l1->val <= l2->val) {
            l1->next = merge2Lists(l1->next, l2);
            return l1;
        } else {
            l2->next = merge2Lists(l1, l2->next);
            return l2;
        }
    }

    ListNode* mergeRange(std::vector<ListNode*>& lists, int lo, int hi) {
        if (lo == hi) { return lists[lo]; }
        int mid = lo + (hi - lo) / 2;
        ListNode* left = mergeRange(lists, lo, mid);
        ListNode* right = mergeRange(lists, mid + 1, hi);
        return merge2Lists(left, right);
    }

public:
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        if (lists.empty()) { return nullptr; }
        return mergeRange(lists, 0, static_cast<int>(lists.size()) - 1);
    }
};
