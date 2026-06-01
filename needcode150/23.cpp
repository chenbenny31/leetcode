// min-heap, T: O(nlogk), S: O(k)

#include <vector>
#include <queue> // std::priority_queue
#include <cstddef> // nullptr
#include <functional> // std::greater

class Solution {
public:
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> pq(cmp);

        for (ListNode* node : lists) {
            if (node) { pq.push(node); }
        }

        ListNode dummy(0);
        ListNode* curr = &dummy;

        while (!pq.empty()) {
            ListNode* node = pq.top(); pq.pop();
            curr->next = node;
            curr = curr->next;
            if (node->next) { pq.push(node->next); }
        }
        return dummy.next;
    }
};

// divide-conquer, T: O(nlogk), S: O(1)

#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        int n = static_cast<int>(lists.size());
        if (n == 0) { return nullptr; }

        for (int step = 1; step < n; step *= 2) {
            for (int i = 0; i + step < n; i += 2 * step) {
                lists[i] = merge_2lists(lists[i], lists[i + step]);
            }
        }
        return lists[0];
    }

private:
    ListNode* merge_2lists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;

        while (l1 && l2) {
            if (l1->val <= l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        curr->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// heap vs div&conq: heap has ptr chasing per comp, d&c has seq merge
// lists[i] = mergeTwoLists(lists[i], list[i+step]): in-place merge
// pre-serve heap:
std::vector<ListNode*> buf;
buf.reserve(lists.size());
std::priority_queue<ListNode* std::vector<ListNode*>, decltype(cmp)> pq(cmp, std::move(buf));
// heap win: large k and short lists, heap proc nodes on-demand and skip empty ones
