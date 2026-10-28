// UnboundMatrix on an unbounded integer grid: addPoint(r, c) places a point;
// isConnected(p1, p2) says whether p2 is reachable from p1; getMinSteps(p1, p2) returns
// the fewest moves (-1 if unreachable). One move jumps from a point to any other point
// in the same row or column. Queries on positions without a point fail.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::map;
using std::set;
using std::pair;

// BFS over points; a row (or column) is expanded only once, because after that every
// point in it is already queued. T: O(P log P) per query, S: O(P).
class UnboundMatrix {
    // all points
    set<pair<int, int>> points;
    // byRow[r] = points in row r; byCol[c] = points in column c
    map<int, vector<pair<int, int>>> byRow, byCol;

public:
    void addPoint(int r, int c) {
        if (!points.insert({r, c}).second) { return; }
        byRow[r].push_back({r, c});
        byCol[c].push_back({r, c});
    }

    int getMinSteps(pair<int, int> start, pair<int, int> end) {
        if (!points.count(start) || !points.count(end)) { return -1; }
        // dist[p] = moves from start to p
        map<pair<int, int>, int> dist;
        set<int> rowDone, colDone;
        queue<pair<int, int>> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            pair<int, int> cur = q.front();
            q.pop();
            if (cur == end) { return dist[cur]; }
            vector<pair<int, int>> candidates;
            if (rowDone.insert(cur.first).second) {
                for (const auto& p : byRow[cur.first]) { candidates.push_back(p); }
            }
            if (colDone.insert(cur.second).second) {
                for (const auto& p : byCol[cur.second]) { candidates.push_back(p); }
            }
            for (const auto& nxt : candidates) {
                if (dist.count(nxt)) { continue; }
                dist[nxt] = dist[cur] + 1;
                q.push(nxt);
            }
        }
        return -1;
    }

    bool isConnected(pair<int, int> p1, pair<int, int> p2) {
        return getMinSteps(p1, p2) != -1;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    UnboundMatrix m;
    m.addPoint(0, 2);
    m.addPoint(1, 2);
    m.addPoint(1, 4);
    m.addPoint(5, 5);
    check(m.getMinSteps({1, 2}, {1, 4}) == 1, "same-row");
    check(m.getMinSteps({0, 2}, {1, 4}) == 2, "via-turn");
    check(m.getMinSteps({0, 2}, {0, 2}) == 0, "self");
    check(!m.isConnected({0, 2}, {5, 5}), "island");
    check(!m.isConnected({0, 0}, {1, 4}), "no-point");
}
