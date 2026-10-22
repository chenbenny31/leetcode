// LC 71 Simplify Path: convert an absolute Unix path to its canonical form: single
// leading '/', no trailing '/', no empty names, "." skipped, ".." goes up one level.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Scan names between slashes; ".." pops the stack, "." is ignored, anything else
// (including "...") is pushed. T: O(n), S: O(n).
string simplifyPath(const string& path) {
    vector<string> stk;   // directory names of the canonical path so far
    int n = path.size();
    int i = 0;
    while (i < n) {
        if (path[i] == '/') {
            i++;
            continue;
        }
        int j = i;
        while (j < n && path[j] != '/') {
            j++;
        }
        string name = path.substr(i, j - i);
        i = j;
        if (name == "..") {
            if (!stk.empty()) { stk.pop_back(); }
        } else if (name != ".") {
            stk.push_back(name);
        }
    }
    string result;
    for (const auto& name : stk) {
        result += "/" + name;
    }
    return result.empty() ? "/" : result;
}

void test(const string& label, const string& path, const string& want) {
    cout << label << (simplifyPath(path) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dots-and-up", "/a/./b/../../c/", "/c");
    test("above-root", "/../", "/");
    test("double-slash", "//a///b", "/a/b");
    test("three-dots-is-a-name", "/a/...", "/a/...");
}
