// Search API for a Unix-like file tree: return the paths of all files under a starting
// directory whose name contains a query string; extend it with "size > minSize" and with
// AND-combinations of filters.
// Follow-up (discussion): symbolic links can create cycles and duplicate results; either
// don't follow them, or follow them and keep a visited set of real nodes (inodes).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct Node {
    string name;
    bool isDir;
    long long size;
    // entries of a directory (empty for a file)
    vector<Node> children;
};

// straightforward (v1): all criteria in one class, an unset field means "don't care";
// every set criterion must match (AND).
class Criteria {
    // "" = any name
    string namePart;
    // -1 = any size
    long long minSize;

public:
    Criteria(const string& namePart = "", long long minSize = -1)
        : namePart(namePart), minSize(minSize) {}

    bool match(const Node& file) const {
        if (!namePart.empty() && file.name.find(namePart) == string::npos) {
            return false;
        }
        if (minSize >= 0 && file.size <= minSize) { return false; }
        return true;
    }
};

void dfs(const Node& node, const string& path, const Criteria& criteria,
         vector<string>& result) {
    if (!node.isDir) {
        if (criteria.match(node)) { result.push_back(path); }
        return;
    }
    for (const Node& child : node.children) {
        dfs(child, path + "/" + child.name, criteria, result);
    }
}

// DFS over the tree from root. T: O(entries * path length), S: O(depth) besides output.
vector<string> search(const Node& root, const Criteria& criteria) {
    vector<string> result;
    dfs(root, "/" + root.name, criteria, result);
    return result;
}

// improved (v2), once new criteria or OR / NOT are asked: one interface per criterion,
// so a new filter is a new class and search never changes (open/closed).
class Filter {
public:
    virtual ~Filter() = default;
    virtual bool match(const Node& file) const = 0;
};

class NameContains : public Filter {
    string part;

public:
    NameContains(const string& part) : part(part) {}
    bool match(const Node& file) const override {
        return file.name.find(part) != string::npos;
    }
};

class MinSize : public Filter {
    long long minSize;

public:
    MinSize(long long minSize) : minSize(minSize) {}
    bool match(const Node& file) const override { return file.size > minSize; }
};

class AndFilter : public Filter {
    // every filter must match
    vector<const Filter*> parts;

public:
    AndFilter(const vector<const Filter*>& parts) : parts(parts) {}
    bool match(const Node& file) const override {
        for (const Filter* f : parts) {
            if (!f->match(file)) { return false; }
        }
        return true;
    }
};

void dfs(const Node& node, const string& path, const Filter& filter,
         vector<string>& result) {
    if (!node.isDir) {
        if (filter.match(node)) { result.push_back(path); }
        return;
    }
    for (const Node& child : node.children) {
        dfs(child, path + "/" + child.name, filter, result);
    }
}

vector<string> search(const Node& root, const Filter& filter) {
    vector<string> result;
    dfs(root, "/" + root.name, filter, result);
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node src{"src", true, 0, {{"z.cpp", false, 300, {}}}};
    Node root{"r", true, 0, {{"x.cpp", false, 10, {}}, {"y.h", false, 200, {}}, src}};

    Criteria cppOnly(".cpp");
    Criteria bigOnly("", 100);
    Criteria bigCpp(".cpp", 100);
    check(search(root, cppOnly) == vector<string>{"/r/x.cpp", "/r/src/z.cpp"}, "v1-name");
    check(search(root, bigOnly) == vector<string>{"/r/y.h", "/r/src/z.cpp"}, "v1-size");
    check(search(root, bigCpp) == vector<string>{"/r/src/z.cpp"}, "v1-and");
    check(search(root, Criteria(".md")).empty(), "v1-none");

    NameContains cpp(".cpp");
    MinSize big(100);
    AndFilter both({&cpp, &big});
    check(search(root, both) == vector<string>{"/r/src/z.cpp"}, "v2-and");
}

// follow-up: OR / NOT -> OrFilter (any part matches), NotFilter (wraps one filter)
// follow-up: huge tree -> stream results through a callback instead of a vector
