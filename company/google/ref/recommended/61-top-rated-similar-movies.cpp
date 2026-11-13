// Top Rated Similar Movies (Google, interview report): movies 0..n-1 with rating[id] and
// an undirected similarity graph (similarity is transitive through paths). From `start`,
// return the k highest-rated movies reachable from it, excluding start itself, ordered by
// higher rating first, then smaller id. Fewer than k reachable -> return all of them.
//
// Clarify:
// - graph[movie] lists similar movies both ways (undirected) -> assume; from an edge
//   list, add both directions
// - "similar" is transitive -> the whole connected component, not only direct neighbors
// - start is excluded; fewer than k others -> all of them; k = 0 -> empty
// - rating ties -> smaller id first (stated); ratings are doubles taken from the input,
//   so comparing them with != and > is exact
// - V, E up to ~1e5 -> one BFS, O(V + E); then sort O(V log V) or size-k heap O(V log k)
// Idea: BFS from start collects its component minus start. Then pick the best k by
//   (rating desc, id asc): sort them all, or keep a size-k heap whose top is the worst
//   kept movie, so each better movie replaces it in O(log k).
// Pitfalls:
// - the heap keeps the best k only if its top is the WORST kept: a min-heap of
//   (rating, -id) puts the lowest rating, then the largest id, on top; pops then come
//   worst first, so reverse at the end, and turn -id back into id
// - mark start seen before the loop, but never add it to found
// - the sort comparator must be strict: break rating ties by id, never >= or <=
// - resize only when there are more than k movies
// Tests to write: top two of a cycle; start excluded; k bigger than the component;
//   isolated start -> empty; equal ratings -> smaller id first.

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

// BFS from start; returns every reachable movie except start. T: O(V + E), S: O(V).
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

// BFS, then sort all reachable movies and keep the first k.
// T: O(V + E + V log V), S: O(V).
vector<int> topRatedSimilar(const vector<double>& rating,
                            const vector<vector<int>>& graph, int start, int k) {
    vector<int> movies = reachableMovies(graph, start);
    sort(movies.begin(), movies.end(), [&](int a, int b) {
        return rating[a] != rating[b] ? rating[a] > rating[b] : a < b;
    });
    if ((int)movies.size() > k) { movies.resize(k); }
    return movies;
}

// BFS, then a size-k heap whose top is the worst kept movie; a new movie pushes it out.
// A min-heap of (rating, -id) puts the lowest rating, then the largest id, on top.
// T: O(V + E + V log k), S: O(V).
vector<int> topRatedSimilarHeap(const vector<double>& rating,
                                const vector<vector<int>>& graph, int start, int k) {
    // the best k seen so far as (rating, -id); top = the worst kept movie
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

// follow-up: many queries {start, k} on a static graph. Label each component once by
// BFS and sort its movies best first; a query walks its component's list, skipping
// start, until it has k. T: O(V + E + V log V + sum of k), S: O(V).
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
    test("skip-self", rating, graph, 2, 1, {1});
    test("k-too-big", rating, graph, 0, 10, {2, 1, 3});
    test("isolated", rating, graph, 4, 2, {});
    test("tie-smaller-id", {5.0, 8.0, 8.0}, {{1, 2}, {0}, {0}}, 0, 1, {1});
    testQueries("queries-mixed", rating, graph, {{0, 2}, {4, 1}, {3, 1}},
                {{2, 1}, {}, {2}});
    testQueries("queries-skip-start", rating, graph, {{2, 2}}, {{1, 0}});
    testQueries("queries-k-zero", rating, graph, {{1, 0}}, {{}});
}

// follow-up: movies as objects with string titles -> same BFS over a map
//   title -> neighbors, visited set of titles
// follow-up: only movies within d hops -> BFS by levels in reachableMovies:
//   for (int level = 0; level < d && !q.empty(); level++) {
//       int levelSize = q.size();
//       for (int i = 0; i < levelSize; i++) { /* pop cur, push unseen nxt as now */ }
//   }
// follow-up: many queries on a static graph -> topRatedQueries above
