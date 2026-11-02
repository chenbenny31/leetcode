// Each comment record has an id, a parentId (-1 or an unknown id means it is a root) and
// text. Print the hierarchy: every comment on its own line, indented two spaces per
// level, children under their parent, siblings in input order.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

struct Comment {
    int id;
    int parentId;
    string text;
};

void dfs(int i, int depth, const vector<Comment>& comments,
         const unordered_map<int, vector<int>>& children, vector<string>& lines) {
    lines.push_back(string(2 * depth, ' ') + comments[i].text);
    if (!children.count(comments[i].id)) { return; }
    for (int c : children.at(comments[i].id)) {
        dfs(c, depth + 1, comments, children, lines);
    }
}

// group children by parent once, then DFS from each root. T: O(n), S: O(n).
vector<string> render(const vector<Comment>& comments) {
    unordered_set<int> ids;
    for (const Comment& c : comments) { ids.insert(c.id); }
    // children[id] = indices of its child comments, in input order
    unordered_map<int, vector<int>> children;
    vector<int> roots;
    int n = comments.size();
    for (int i = 0; i < n; i++) {
        if (ids.count(comments[i].parentId)) {
            children[comments[i].parentId].push_back(i);
        } else {
            roots.push_back(i);
        }
    }
    vector<string> lines;
    for (int r : roots) { dfs(r, 0, comments, children, lines); }
    return lines;
}

void test(const string& label, const vector<Comment>& comments,
          const vector<string>& want) {
    cout << label << (render(comments) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested", {{1, -1, "a"}, {2, 1, "b"}, {3, 2, "c"}}, {"a", "  b", "    c"});
    test("siblings", {{1, -1, "a"}, {2, 1, "x"}, {3, 1, "y"}}, {"a", "  x", "  y"});
    test("child-first", {{2, 1, "b"}, {1, -1, "a"}}, {"a", "  b"});
    test("two-roots", {{1, -1, "a"}, {2, -1, "b"}, {3, 1, "c"}}, {"a", "  c", "b"});
    test("orphan", {{5, 9, "lost"}}, {"lost"});
}
