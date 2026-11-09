// match on file only
// contains = substr, case-sensitive
// any output order
// filters combines -> AND, OR is a follow-up
// no symbolic links
// tree given by mem

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
    vector<Node> chilren; // entreis of a dir, empty for a file
};

class Filter {
public:
    virtual ~Filter() = default;
    virtual bool match(const Node& file) const = 0;
};

class NameContains : public Filter {
private:
    string part;
public:
    NameContains(const string& part) : part(part) {}
    bool match(const Node& file) const override {
        return file.name.find(part) != string::npos;
    }
};

class MinSize : public Filter {
private:
    long long minSize;

public:
    MinSize(long long minSize) : minSize(minSize) {}
    bool match(const Node& file) const override {
        return file.size > minSize;
    }
};

class AndFilter : public Filter {
private:
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

    for (const Node& child : node.chilren) {
        dfs(child, path + "/" + child.name, filter, result);
    }
}

// DFS over the tree from root, T: O(entries * (path-len + filter-cost)), S: O(depth)
vector<string> search(const Node& root, const Filter& filter) {
    vector<string> result;
    dfs(root, "/" + root.name, filter, result);
    return result;
}

// follow-up: OR filter
class OrFilter : public Filter {
private:
    vector<const Filter*> parts; // at least one part match

public:
    OrFilter(const vector<const Filter*>& parts) : parts(parts) {}
    bool match(const Node& file) const override {
        for (const Filter* f : parts) {
            if (f->match(file)) { return true; }
        }
        return false;
    }
};

// follow-up: NOT filter
class NotFilter : public Filter {
private:
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
