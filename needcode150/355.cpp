// heap-based feed, T: O(FlogF + klogF) getNewsFeed, F = followees, S: O(nt)

#include <vector>
#include <unordered_map> 
#include <unordered_set>
#include <queue> // std::priority_queue
#include <tuple>
#include <utility> // std::pair

class Twitter {
private:
    int timestamp;
    std::unordered_map<int, std::vector<std::pair<int,int>>> tweets; // userId -> {time, tweetId}
    std::unordered_map<int, std::unordered_set<int>> follows; // userId -> followees

public:
    Twitter() : timestamp(0) {}

    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({timestamp, tweetId});
        timestamp++;
    }

    std::vector<int> getNewsFeed(int userId) {
        // {time, tweetId, userId, idx} min-heap on time
        using T = std::tuple<int, int, int, int>;
        std::priority_queue<T, std::vector<T>, std::less<T>> maxHeap(std::less<T>{});

        auto push_latest = [&](int uid) {
            auto it = tweets.find(uid);
            if (it == tweets.end() || it->second.empty()) { return; }
            int idx = static_cast<int>(it->second.size()) - 1;
            auto& [t, tid] = it->second[idx];
            maxHeap.push({t, tid, uid, idx});
        };

        follows[userId].insert(userId); // user sees own tweets
        for (int uid : follows[userId]) { push_latest(uid); }

        std::vector<int> out;
        out.reserve(10);

        while (!maxHeap.empty() && static_cast<int>(out.size()) < 10) {
            auto [t, tid, uid, idx] = maxHeap.top(); maxHeap.pop();
            out.push_back(tid);
            if (idx > 0) {
                auto& [pt, ptid] = tweets[uid][idx - 1];
                maxHeap.push({pt, ptid, uid, idx - 1});
            }
        }
        return out;
    }

    void follow(int followerId, int followeeId) {
        follows[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        follows[followerId].erase(followeeId);
    }
};

// k-way merge vs min-heap: one entry per followee in heap
// follows[userId].insert(userId): self-flow implementation
// timestamp as global counter: monotonically increasing
