// LC 355 Design Twitter: postTweet(userId, tweetId); getNewsFeed(userId) = the 10 most
// recent tweet ids from the user and their followees, newest first;
// follow(followerId, followeeId); unfollow(followerId, followeeId).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::tuple;
using std::unordered_map;
using std::unordered_set;

class Twitter {
    int clock = 0;   // global tweet counter, larger = newer
    // user -> (time, tweetId) of their tweets, oldest first
    unordered_map<int, vector<pair<int, int>>> tweets;
    // user -> users they follow
    unordered_map<int, unordered_set<int>> followees;

public:
    // T: O(1)
    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({clock, tweetId});
        clock++;
    }

    // k-way merge of the authors' tweet lists, newest first, stopping after 10.
    // T: O(F + 10 log F) for F followees, S: O(F).
    vector<int> getNewsFeed(int userId) {
        unordered_set<int> authors;
        auto it = followees.find(userId);
        if (it != followees.end()) { authors = it->second; }
        authors.insert(userId);
        // (time, author, index) = newest tweet of each author not yet in the feed
        priority_queue<tuple<int, int, int>> heap;
        for (int a : authors) {
            auto found = tweets.find(a);
            if (found == tweets.end()) { continue; }
            int last = found->second.size() - 1;
            heap.push({found->second[last].first, a, last});
        }
        vector<int> feed;
        while (!heap.empty() && feed.size() < 10) {
            auto [when, a, i] = heap.top();
            heap.pop();
            feed.push_back(tweets[a][i].second);
            if (i > 0) { heap.push({tweets[a][i - 1].first, a, i - 1}); }
        }
        return feed;
    }

    // T: O(1)
    void follow(int followerId, int followeeId) {
        followees[followerId].insert(followeeId);
    }

    // T: O(1)
    void unfollow(int followerId, int followeeId) {
        followees[followerId].erase(followeeId);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Twitter tw;
    tw.postTweet(1, 10);
    tw.follow(1, 2);
    tw.postTweet(2, 20);
    tw.postTweet(1, 11);
    check(tw.getNewsFeed(1) == vector<int>({11, 20, 10}), "merge-newest-first");
    check(tw.getNewsFeed(2) == vector<int>({20}), "followee-sees-only-own");
    tw.unfollow(1, 2);
    check(tw.getNewsFeed(1) == vector<int>({11, 10}), "unfollow");
    check(tw.getNewsFeed(9).empty(), "unknown-user");
    for (int t = 100; t < 112; t++) { tw.postTweet(3, t); }
    check(tw.getNewsFeed(3).size() == 10 && tw.getNewsFeed(3)[0] == 111, "cap-ten");
}
