// greedy math, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::max_element
#include <cstring> // std::memset

class Solution {
public:
    int leastInterval(std::vector<char>& tasks, int n) {
        constexpr int R = 26;
        constexpr char BASE = 'A';

        int freq[R];
        std::memset(freq, 0, sizeof(freq));

        for (char c : tasks) { freq[c - BASE]++; }

        int maxFreq = *std::max_element(freq, freq + R);
        int maxCount = 0;
        for (int i = 0; i < R; i++) {
            if (freq[i] == maxFreq) { maxCount++; }
        }

        int minCycles = (maxFreq - 1) * (n + 1) + maxCount;
        return std::max(minCycles, static_cast<int>(tasks.size()));
    }
};

// max-heap simulation, T: O(nlogn), S: O(1)

#include <vector>
#include <queue> // std::priority_queue
#include <cstring> // std::memset
#include <algorithm> // std::max

class Solution {
public:
    int leastInterval(std::vector<char>& tasks, int n) {
        constexpr int R = 26;
        constexpr char BASE = 'A';

        int freq[R];
        std::memset(freq, 0, sizeof(freq));
        for (char c : tasks) { freq[c - BASE]++; }

        std::vector<int> buf;
        buf.reserve(R);
        std::priority_queue<int, std::vector<int>, std::less<int>> maxHeap(std::less<int>{}, std::move(buf));
        for (int i = 0; i < R; i++) {
            if (freq[i] > 0) { maxHeap.push(freq[i]); }
        }

        int cycles = 0;
        std::vector<int> tmp;
        tmp.reserve(n + 1);

        while (!maxHeap.empty()) {
            tmp.clear();
            for (int i = 0; i <= n; i++) {
                if (!maxHeap.empty()) {
                    tmp.push_back(maxHeap.top());
                    maxHeap.pop();
                }
            }
            for (int cnt : tmp) {
                if (cnt - 1 > 0) { maxHeap.push(cnt - 1); }
            }
            cycles += maxHeap.empty() ? static_cast<int>(tmp.size()) : n + 1;
        }
        return cycles;
    }
};

// formula derivation: most frequent tasks create maxFreq-1 gaps of size n+1, if multiple char with same freq, add maxCount
