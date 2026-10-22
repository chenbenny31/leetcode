// LC 1166 Design File System: createPath(path, value) creates "/a/b" with a value and
// returns true only if the path does not exist and its parent does (the root always
// exists); get(path) returns the value, or -1 if the path does not exist.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

class FileSystem {
    // full path -> value, for every path created so far
    unordered_map<string, int> values;

public:
    // the parent is the path up to the last '/'; "" is the root. T: O(len), S: O(paths).
    bool createPath(const string& path, int value) {
        if (values.count(path)) { return false; }
        string parent = path.substr(0, path.rfind('/'));
        if (!parent.empty() && !values.count(parent)) { return false; }
        values[path] = value;
        return true;
    }

    int get(const string& path) {
        auto it = values.find(path);
        if (it == values.end()) { return -1; }
        return it->second;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    FileSystem fs;
    check(!fs.createPath("/a/b", 1), "missing-parent");
    check(fs.createPath("/a", 1) && fs.createPath("/a/b", 2), "nested");
    check(!fs.createPath("/a", 5), "duplicate");
    check(fs.get("/a/b") == 2 && fs.get("/c") == -1, "get");
}
