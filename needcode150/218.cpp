// sweep-line + max-heap, T: O(nlogn), S: O(n)

#include <vector>
#include <set> // std::multiset
#include <utility> // std::pair
#include <algorithm> // std::sort

class Solution {
public:
    std::vector<std::vector<int>> getSkyline(std::vector<std::vector<int>>& buildings) {
        std::vector<std::pair<int, int>> events;
        events.reserve(buildings.size() * 2);
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]}); // start: negative height
            events.push_back({b[1], b[2]}); // end: positive height
        }
        std::sort(events.begin(), events.end());

        std::multiset<int> active;
        active.insert(0);
        std::vector<std::vector<int>> out;
        int prev_max = 0;

        for (auto& [x, h] : events) {
            if (h < 0) { active.insert(-h); } // start: insert height
            else { active.erase(active.find(h)); } // end: remove height

            int cur_max = *active.rbegin();
            if (cur_max != prev_max) {
                out.push_back({x, cur_max});
                prev_max = cur_max;
            }
        }
        return out;
    }
};

// lazy-deletion heap, T: O(nlogn), S: O(n)

#include <vector>
#include <queue> // std::priority_queue
#include <unordered_map>
#include <algorithm> // std::sort
#include <utility> // std::pair

class Solution {
public:
    std::vector<std::vector<int>> getSkyline(std::vector<std::vector<int>>& buildings) {
        std::vector<std::pair<int, int>> events;
        events.reserve(buildings.size() * 2);
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]});
            events.push_back({b[1], b[2]});
        }
        std::sort(events.begin(), events.end());

        std::vector<int> buf;
        buf.reserve(buildings.size());
        std::priority_queue<int, std::vector<int>> heap(std::less<int>{}, std::move(buf)); // max-heap

        std::unordered_map<int, int> pending;
        pending.reserve(buildings.size());
        pending.max_load_factor(0.25f);
        
        heap.push(0);
        std::vector<std::vector<int>> out;
        int prev_max = 0;

        for (auto& [x, h] : events) {
            if (h < 0) { heap.push(-h); }
            else { pending[h]++; } // mark for lazy deletion

            while (pending.count(heap.top()) && pending[heap.top()] > 0) {
                pending[heap.top()]--;
                heap.pop();
            }

            int cur_max = heap.top();
            if (cur_max != prev_max) {
                out.push_back({x, cur_max});
                prev_max = cur_max;
            }
        }
        return out;
    }
};

// negative height for start: start event sorts before end event at same x, ensure building add first then remove
// multi-set not priority-queue: multi-set provides O(logn) removing, heap needs lazy-deletion
// cur_max != prev_max: deduplication
// cache std::multiset: each node separate heap allocation, pointer chasing per operation
// while (pending.count(...) && pending[..] > 0): stale zero entries detection
