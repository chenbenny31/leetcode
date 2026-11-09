// Search API for a Unix-like file tree (Amazon OOD): return the paths of all files under
// a starting directory that match a filter: name contains a string (case-sensitive),
// size > minSize, and AND-combinations of filters. Only files match, never directories;
// no symbolic links.
// If the round is framed as plain coding, one class with optional fields is enough; when
// more criteria or OR / NOT come up, split into the Filter classes below.
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

// One class per criterion behind one interface, so a new criterion is a new class and
// search never changes (open/closed). Combinators hold other filters, so they nest.
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
    // every part must match; the caller owns the parts (shared_ptr in production)
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

// DFS over the tree from root. T: O(entries * (path length + filter cost)),
// S: O(depth) besides output.
vector<string> search(const Node& root, const Filter& filter) {
    vector<string> result;
    dfs(root, "/" + root.name, filter, result);
    return result;
}

// follow-up: OR, same shape as AndFilter
class OrFilter : public Filter {
    // at least one part must match
    vector<const Filter*> parts;

public:
    OrFilter(const vector<const Filter*>& parts) : parts(parts) {}
    bool match(const Node& file) const override {
        for (const Filter* f : parts) {
            if (f->match(file)) { return true; }
        }
        return false;
    }
};

// follow-up: NOT wraps one filter
class NotFilter : public Filter {
    const Filter* inner;

public:
    NotFilter(const Filter* inner) : inner(inner) {}
    bool match(const Node& file) const override { return !inner->match(file); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node src{"src", true, 0, {{"z.cpp", false, 300, {}}}};
    Node root{"r", true, 0, {{"x.cpp", false, 10, {}}, {"y.h", false, 200, {}}, src}};

    NameContains cpp(".cpp");
    MinSize big(100);
    AndFilter bigCpp({&cpp, &big});
    check(search(root, cpp) == vector<string>{"/r/x.cpp", "/r/src/z.cpp"}, "name");
    check(search(root, big) == vector<string>{"/r/y.h", "/r/src/z.cpp"}, "size");
    check(search(root, bigCpp) == vector<string>{"/r/src/z.cpp"}, "and");
    check(search(root, NameContains(".md")).empty(), "none");
    check(search(root, MinSize(1000)).empty(), "too-big");
}

// follow-up: huge tree -> stream results through a callback instead of a vector
