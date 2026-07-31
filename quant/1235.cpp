// sort + dp + binary-search

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        const int n = static_cast<int>(startTime.size()); 
        std::vector<int> indices(n);
        for (int i = 0; i < n; i++) { indices[i] = i; }

        std::sort(indices.begin(), indices.end(), [&](int a, int b) {
            return endTime[a] < endTime[b];
        });

        std::vector<int> ends(n);
        for (int i = 0; i < n; i++) { ends[i] = endTime[indices[i]]; }

        std::vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            int jobIdx = indices[i - 1];
            int curStart = startTime[jobIdx];
            int curProfit = profit[jobIdx];

            int lo = 0;
            int hi = i - 1;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (ends[mid] <= curStart) { lo = mid + 1; }
                else { hi = mid; }
            }

            dp[i] = std::max(dp[i - 1], dp[lo] + curProfit);
        }
        return dp[n];
    }
};

// heap

#include <vector>
#include <queue> // std::priority_queue
#include <algorithm> // std::sort, std::max

struct Job {
    int start;
    int end;
    int profit;
};

class Solution {
private:
    int getMaxProfit(std::vector<Job>& jobs) {
        int maxProfit = 0;
        auto cmp = [](const Job& a, const Job& b) {
            return a.end > b.end;
        };
        std::vector<Job> buf;
        buf.reserve(jobs.size());
        std::priority_queue<Job, std::vector<Job>, decltype(cmp)> minHeap(cmp, std::move(buf));

        for (const Job& job : jobs) {
            while (!minHeap.empty() && job.start >= minHeap.top().end) {
                maxProfit = std::max(maxProfit, minHeap.top().profit);
                minHeap.pop();
            }
            minHeap.push({job.start, job.end, job.profit + maxProfit});
        }

        while (!minHeap.empty()) {
            maxProfit = std::max(maxProfit, minHeap.top().profit);
            minHeap.pop();
        }
        return maxProfit;
    }

public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        const int n = static_cast<int>(startTime.size());
        std::vector<Job> jobs(n);

        for (int i = 0; i < n; i++) {
            jobs[i] = {startTime[i], endTime[i], profit[i]};
        }

        std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.start < b.start;
        });

        return getMaxProfit(jobs);
    }
};
