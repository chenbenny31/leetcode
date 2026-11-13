// Top Rated Similar Movies (Google, interview report)
// (1) rating[id], undirected similarity graph; k top-rated movies reachable from start
// (2) many {start, k} queries
//
// clarify:
// edges -> both ways
// similarity transitive -> whole component, not only neighbors
// start itself -> excluded
// fewer than k reachable -> return all
// rating ties -> smaller id first

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;
using std::greater;
using std::sort;
using std::reverse;

// BFS from start; returns every reachable movie except start
vector<int> reachableMovies(const vector<vector<int>>& graph, int start) {
    vector<bool> seen(graph.size(), false);
    seen[start] = true;
    vector<int> found;
    queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nxt : graph[cur]) {
            if (seen[nxt]) { continue; }
            seen[nxt] = true;
            found.push_back(nxt);
            q.push(nxt);
        }
    }
    return found;
}

// (1) solution 1: BFS + sort; simplest
// core idea: BFS collects start's component minus start; sort by (rating desc, id asc)
// T: O(V + E + V log V), S: O(V)
vector<int> topRatedSimilar(const vector<double>& rating,
                            const vector<vector<int>>& graph, int start, int k) {
    vector<int> movies = reachableMovies(graph, start);
    sort(movies.begin(), movies.end(), [&](int a, int b) {
        return rating[a] != rating[b] ? rating[a] > rating[b] : a < b;
    });
    if ((int)movies.size() > k) { movies.resize(k); }
    return movies;
}

// (1) solution 2: BFS + size-k min-heap; use when k << V
// core idea: heap of (rating, -id), top = worst kept; pop when size > k; reverse at end
// T: O(V + E + V log k), S: O(V)
vector<int> topRatedSimilarHeap(const vector<double>& rating,
                                const vector<vector<int>>& graph, int start, int k) {
    // best k seen so far as (rating, -id); top = worst kept movie
    priority_queue<pair<double, int>, vector<pair<double, int>>,
                   greater<pair<double, int>>> heap;
    for (int id : reachableMovies(graph, start)) {
        heap.push({rating[id], -id});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(-heap.top().second);
        heap.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}

// (2) many queries
// core idea: label each component once by BFS, sort its movies best first
// a query walks its component's list, skipping start, until it has k
// T: O(V + E + V log V + sum of k), S: O(V)
vector<vector<int>> topRatedQueries(const vector<double>& rating,
                                    const vector<vector<int>>& graph,
                                    const vector<vector<int>>& queries) {
    int n = graph.size();
    // comp[movie] = index of its component in members, -1 until labeled
    vector<int> comp(n, -1);
    // members[c] = movies of component c, best first after the sort
    vector<vector<int>> members;
    for (int first = 0; first < n; first++) {
        if (comp[first] != -1) { continue; }
        int c = members.size();
        comp[first] = c;
        members.push_back({first});
        queue<int> q;
        q.push(first);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (comp[nxt] != -1) { continue; }
                comp[nxt] = c;
                members[c].push_back(nxt);
                q.push(nxt);
            }
        }
    }
    for (vector<int>& group : members) {
        sort(group.begin(), group.end(), [&](int a, int b) {
            return rating[a] != rating[b] ? rating[a] > rating[b] : a < b;
        });
    }
    vector<vector<int>> result;
    for (const vector<int>& query : queries) {
        int start = query[0];
        int k = query[1];
        vector<int> top;
        for (int movie : members[comp[start]]) {
            if ((int)top.size() == k) { break; }
            if (movie != start) { top.push_back(movie); }
        }
        result.push_back(top);
    }
    return result;
}

void test(const string& label, const vector<double>& rating,
          const vector<vector<int>>& graph, int start, int k, const vector<int>& want) {
    bool ok = topRatedSimilar(rating, graph, start, k) == want &&
              topRatedSimilarHeap(rating, graph, start, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testQueries(const string& label, const vector<double>& rating,
                 const vector<vector<int>>& graph, const vector<vector<int>>& queries,
                 const vector<vector<int>>& want) {
    bool ok = topRatedQueries(rating, graph, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<double> rating = {7.5, 8.0, 9.0, 6.0, 9.5};
    // graph[movie] = movies directly similar to it; 0-1-2-3-0 is a cycle, 4 is alone
    vector<vector<int>> graph = {{1, 3}, {0, 2}, {1, 3}, {2, 0}, {}};
    test("top-two", rating, graph, 0, 2, {2, 1});
    test("k-too-big", rating, graph, 0, 10, {2, 1, 3});
    test("isolated", rating, graph, 4, 2, {});
    test("tie-smaller-id", {5.0, 8.0, 8.0}, {{1, 2}, {0}, {0}}, 0, 1, {1});
    testQueries("queries-mixed", rating, graph, {{0, 2}, {4, 1}, {3, 1}},
                {{2, 1}, {}, {2}});
    testQueries("queries-skip-start", rating, graph, {{2, 2}}, {{1, 0}});
    testQueries("queries-k-zero", rating, graph, {{1, 0}}, {{}});
}

// follow-up:
// movies as objects w/ string titles -> same BFS over map title -> neighbors
// only movies within d hops -> BFS by levels, stop after d levels
