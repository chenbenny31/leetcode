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

class Criteria {
private:
    string namePart; // "" = any name
    long long minSize; // -1 = any size
    // bool any; // ext to OR
    // bool negate; // ext to NOT

public:
    Criteria(const string& namePart = "", long long minSize = -1)
        : namePart(namePart), minSize(minSize) {}
    // Criteria(const string& namePart = "", long long minSize = -1,
    //          bool anyOf = false, bool negate = false)
    //     : namePart(namePart), minSize(minSize), anyOf(anyOf), negate(negate) {}

    bool match(const Node& file) const {
        if (!namePart.empty() && file.name.find(namePart) == string::npos) {
            return false;
        }
        if (minSize >= 0 && file.size <= minSize) { return false; }
        return true;
    }

    // ext to OR + NOT
    bool matchExt(const Node& file) const {
        int set = 0;
        int hit = 0;
        if (!namePart.empty()) {
            set++;
            if (file.name.find(namePart) != string::npos) { hit++; }
        }
        if (minSize >= 0) {
            set++;
            if (file.size > minSize) { hit++; }
        }

        bool ok = any ? hit > 0 : hit == set;
        return neg ? !ok : ok;
    }
};

void dfs(const Node& node, const string& path,
         const Criteria& criteria, vector<string>& result) {
    if (!node.isDir) {
        if (criteria.match(node)) { result.push_back(path); }
        return;
    }

    for (const Node& child : node.chilren) {
        dfs(child, path + "/" + child.name, criteria, result);
    }
}

vector<string> search(const Node& root, const Criteria& criteria) {
    vector<string> result;
    dfs(root, "/" + root.name, criteria, result);
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node src{"src", true, 0, {{"z.cpp", false, 300, {}}}};
    Node root{"r", true, 0, {{"x.cpp", false, 10, {}}, {"y.h", false, 200, {}}, src}};

    Criteria cppOnly{".cpp"};
    Criteria bigOnly{"", 100};
    Criteria bigCpp{".cpp", 100};
    check(search(root, cppOnly) == vector<string>{"/r/x.cpp", "/r/src/z.cpp"}, "name");
    check(search(root, bigOnly) == vector<string>{"/r/y.h", "/r/src/z.cpp"}, "size");
    check(search(root, bigCpp) == vector<string>{"/r/src/z.cpp"}, "and");
    check(search(root, Criteria{".md"}).empty(), "none");
}
