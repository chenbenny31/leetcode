// Reviews arrive as addReview(id, rating) with rating in 1..5. getMedian() returns the
// median rating of the last N reviews (or of all reviews while fewer than N); with an
// even count it is the mean of the two middle ratings. Called only when non-empty.

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::sort;

// straightforward: keep the window, sort a copy per query.
// T: add O(1), getMedian O(N log N). S: O(N).
class ReviewWindow {
    int cap;
    // ratings of the last cap reviews, oldest first
    deque<int> window;

public:
    ReviewWindow(int cap) : cap(cap) {}

    void addReview(int id, int rating) {
        (void)id;
        window.push_back(rating);
        if ((int)window.size() > cap) { window.pop_front(); }
    }

    double getMedian() {
        vector<int> v(window.begin(), window.end());
        sort(v.begin(), v.end());
        int n = v.size();
        if (n % 2 == 1) { return v[n / 2]; }
        return (v[n / 2 - 1] + v[n / 2]) / 2.0;
    }
};

// value of the k-th smallest rating (0-based) from the counts
int kth(const int cnt[], int k) {
    int r = 1;
    while (k >= cnt[r]) {
        k -= cnt[r];
        r++;
    }
    return r;
}

// improved: ratings take only 5 values, so keep counts and walk them.
// T: O(1) per call, S: O(N). (For arbitrary values: two balanced multisets.)
class ReviewWindowCounts {
    int cap;
    // ratings of the last cap reviews, oldest first
    deque<int> window;
    // cnt[r] = reviews with rating r in the window
    int cnt[6] = {};

public:
    ReviewWindowCounts(int cap) : cap(cap) {}

    void addReview(int id, int rating) {
        (void)id;
        window.push_back(rating);
        cnt[rating]++;
        if ((int)window.size() > cap) {
            cnt[window.front()]--;
            window.pop_front();
        }
    }

    double getMedian() {
        int n = window.size();
        if (n % 2 == 1) { return kth(cnt, n / 2); }
        return (kth(cnt, n / 2 - 1) + kth(cnt, n / 2)) / 2.0;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ReviewWindow a(3);
    ReviewWindowCounts b(3);
    a.addReview(1, 5);
    b.addReview(1, 5);
    check(a.getMedian() == 5 && b.getMedian() == 5, "one");
    a.addReview(2, 2);
    b.addReview(2, 2);
    check(a.getMedian() == 3.5 && b.getMedian() == 3.5, "even");
    a.addReview(3, 3);
    b.addReview(3, 3);
    check(a.getMedian() == 3 && b.getMedian() == 3, "full");
    a.addReview(4, 1);
    b.addReview(4, 1);
    check(a.getMedian() == 2 && b.getMedian() == 2, "evict");
}
