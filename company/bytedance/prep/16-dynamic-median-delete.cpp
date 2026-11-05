// dup vals? yes, multiset, delete removes one copy
// delete non-exist val? no
// query on empty set? return "null"
// median format -> reduced fraction "num/den", odd "x/1", even "y/2", y=a+b
// val range: int, but sum could overflow
// op count, query freq? up to 1e5

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <unordered_map>
#include <set>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::unordered_map;
using std::to_string;
using std::multiset;
using std::prev;

string medianFraction(bool odd, long long lowTop, long long highTop) {
    if (odd) {
        return to_string(lowTop) + "/1";
    }
    long long sum = lowTop + highTop;
    if (sum % 2 == 0) {
        return to_string(sum / 2) + "/1";
    }
    return to_string(sum) + "/2";
}

class DualHeap {
private:
    priority_queue<long long> low; // max-heap, top is max of smaller half
    priority_queue<long long, vector<long long>, greater<long long>> high; // min-heap, top is min of larger half
    unordered_map<long long, int> delayed; // delayed[x] = deletion of x not yet pop
    unordered_map<long long, int> count; // count[x] = copies of x cur in multiset
    int lowSize = 0;
    int highSize = 0;

    void pruneLow() {
        while (!low.empty() && delayed[low.top()] > 0) {
            delayed[low.top()]--;
            low.pop();
        }
    }
    void pruneHigh() {
        while (!high.empty() && delayed[high.top()] > 0) {
            delayed[high.top()]--;
            high.pop();
        }
    }
    void balance() {
        if (lowSize > highSize + 1) {
            high.push(low.top());
            low.pop();
            lowSize--;
            highSize++;
            pruneLow();
        } else if (lowSize < highSize) {
            low.push(high.top());
            high.pop();
            highSize--;
            lowSize++;
            pruneHigh();
        }
    }

public:
    void add(long long x) {
        count[x]++;
        if (lowSize == 0 || x <= low.top()) {
            low.push(x);
            lowSize++;
        } else {
            high.push(x);
            highSize++;
        }
        balance();
    }

    void remove(long long x) {
        if (count[x] == 0) {
            return;
        }
        count[x]--;
        delayed[x]++;

        if (x <= low.top()) {
            lowSize--;
            pruneLow();
        } else {
            highSize--;
            pruneHigh();
        }
        balance();
    }

    string median() {
        if (lowSize == 0) { return "null"; }
        if (lowSize > highSize) {
            return medianFraction(true, low.top(), 0);
        }
        return medianFraction(false, low.top(), high.top());
    }
};

// follow-up: no lazy del, T: O(logn) per op, S: O(n)
class MedianSet {
private:
    multiset<long long> low; // max of smaller half is *low.rbegin()
    multiset<long long> high; // min of larger half is *high.begin()

    void balance() {
        if (low.size() > high.size() + 1) {
            high.insert(*low.rbegin());
            low.erase(prev(low.end())); // erase only support forward iter
        } else if (low.size() < high.size()) {
            low.insert(*high.begin());
            high.erase(high.begin());
        }
    }

public:
    void add(long long x) {
        if (low.empty() || x <= *low.rbegin()) {
            low.insert(x);
        } else {
            high.insert(x);
        }
        balance();
    }

    void remove(long long x) { // erase(find(x)) NOT erase(x)
        multiset<long long>::iterator it = low.find(x);
        if (it != low.end()) {
            low.erase(it);
        } else {
            it = high.find(x);
            if (it == high.end()) {
                return;
            }
            high.erase(it);
        }
        balance();
    }

    string median() {
        if (low.empty()) { return "null"; }
        if (low.size() > high.size()) {
            return medianFraction(true, *low.rbegin(), 0);
        }
        return medianFraction(false, *low.rbegin(), *high.begin());
    }
};

vector<string> dynamicMedians(const vector<vector<int>>& operations) {
    DualHeap heaps;
    vector<string> result;
    for (const vector<int>& op : operations) {
        if (op[0] == 1) {
            heaps.add(op[1]);
        } else if (op[0] == 2) {
            heaps.remove(op[1]);
        } else {
            result.push_back(heaps.median());
        }
    }
    return result;
}

vector<string> dynamicMediansSets(const vector<vector<int>>& operations) {
    MedianSet sets;
    vector<string> result;
    for (const vector<int>& op : operations) {
        if (op[0] == 1) {
            sets.add(op[1]);
        } else if (op[0] == 2) {
            sets.remove(op[1]);
        } else {
            result.push_back(sets.median());
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& operations,
          const vector<string>& want) {
    bool ok = dynamicMedians(operations) == want;
    bool okSets = dynamicMediansSets(operations) == want;
    cout << label << (ok && okSets ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-even", {{1, 4}, {3}, {1, 7}, {3}, {1, 2}, {3}}, {"4/1", "11/2", "4/1"});
    test("delete-missing", {{1, 3}, {2, 5}, {3}, {2, 3}, {3}}, {"3/1", "null"});
    test("negatives", {{1, -1}, {1, -4}, {3}, {1, -4}, {2, -1}, {3}}, {"-5/2", "-4/1"});
    test("dups", {{1, 2}, {1, 2}, {1, 2}, {2, 2}, {3}}, {"2/1"});
}
