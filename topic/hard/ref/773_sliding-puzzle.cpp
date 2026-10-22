// LC 773 Sliding Puzzle: a 2 x 3 board holds tiles 1..5 and an empty square 0. A move
// swaps 0 with an adjacent tile. Return the fewest moves to reach [[1,2,3],[4,5,0]],
// or -1 if that is impossible.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_set;
using std::swap;

// BFS over board states flattened to a string. T: O(6!), S: O(6!).
int slidingPuzzle(vector<vector<int>>& board) {
    // graph[i] = flat positions adjacent to position i in the 2 x 3 board
    vector<vector<int>> graph = {{1, 3}, {0, 2, 4}, {1, 5}, {0, 4}, {1, 3, 5}, {2, 4}};
    string start;
    for (const auto& row : board) {
        for (int x : row) { start += char('0' + x); }
    }
    queue<string> q;                // states in non-decreasing move count
    unordered_set<string> seen;     // states already queued
    q.push(start);
    seen.insert(start);
    for (int moves = 0; !q.empty(); moves++) {
        int width = q.size();       // the states at this depth
        for (int w = 0; w < width; w++) {
            string cur = q.front();
            q.pop();
            if (cur == "123450") { return moves; }
            int zero = cur.find('0');
            for (int nxt : graph[zero]) {
                string next = cur;
                swap(next[zero], next[nxt]);
                if (seen.insert(next).second) { q.push(next); }
            }
        }
    }
    return -1;
}

void test(const string& label, vector<vector<int>> board, int want) {
    cout << label << (slidingPuzzle(board) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("solved", {{1, 2, 3}, {4, 5, 0}}, 0);
    test("one-move", {{1, 2, 0}, {4, 5, 3}}, 1);
    test("odd-swap", {{2, 1, 3}, {4, 5, 0}}, -1);
    test("two-moves", {{1, 2, 3}, {0, 4, 5}}, 2);
}
