// Top Rated Similar Movies (Google, interview report): movies 0..n-1 with rating[id] and
// an undirected similarity graph (similarity is transitive through paths). From `start`,
// return the k highest-rated movies reachable from it, excluding start itself, ordered by
// higher rating first, then smaller id. Fewer than k reachable -> return all of them.

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::priority_queue;
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
// T: O(V + E + V log k), S: O(V).
vector<int> topRatedSimilarHeap(const vector<double>& rating,
                                const vector<vector<int>>& graph, int start, int k) {
    auto better = [&](int a, int b) {
        return rating[a] != rating[b] ? rating[a] > rating[b] : a < b;
    };
    // the best k seen so far; top = lowest rating, then largest id
    priority_queue<int, vector<int>, decltype(better)> heap(better);
    for (int id : reachableMovies(graph, start)) {
        heap.push(id);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}

void test(const string& label, const vector<double>& rating,
          const vector<vector<int>>& graph, int start, int k, const vector<int>& want) {
    bool ok = topRatedSimilar(rating, graph, start, k) == want &&
              topRatedSimilarHeap(rating, graph, start, k) == want;
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
}

// follow-up: movies as objects with string titles -> same BFS over pointers / a map
//   title -> neighbors, visited set of titles
// follow-up: only movies within d hops -> BFS level by level and stop after level d
// follow-up: many queries on a static graph -> precompute connected components once
//   (union-find or BFS), keep each component sorted by rating; answer = first k of the
//   start's component skipping start, O(k) per query
