// ByteDance phone (FT): turn a flat comment list into a nested JSON forest. Comment i
// has ids[i], parentIds[i] and texts[i]; an empty parentId means a root, otherwise the
// comment hangs under the comment with that id. Roots and siblings keep input order.
// Return a compact JSON array; every object has the fields id, parent_id, text,
// children in that order (children follow the same rule recursively). Strings are
// JSON-escaped. A child may appear before its parent in the input.
//   buildCommentTree(ids, parentIds, texts) -> string
// Constraints: n <= 1e5, ids unique and non-empty, the input is a forest.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// JSON string literal: quote it and escape ", \ and control characters.
string jsonString(const string& s) {
    const char* hex = "0123456789abcdef";
    string out = "\"";
    for (char ch : s) {
        unsigned char c = ch;
        if (c == '"') {
            out += "\\\"";
        } else if (c == '\\') {
            out += "\\\\";
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\t') {
            out += "\\t";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c < 0x20) {
            // other control characters as \u00XX
            out += "\\u00";
            out += hex[c / 16];
            out += hex[c % 16];
        } else {
            out += ch;
        }
    }
    out += "\"";
    return out;
}

class CommentTree {
    const vector<string>& ids;
    const vector<string>& parentIds;
    const vector<string>& texts;
    // children[i] = indices of comment i's replies, in input order
    vector<vector<int>> children;
    // roots in input order
    vector<int> roots;

    // Append comment i and its whole subtree as one JSON object.
    void emit(int i, string& out) {
        out += "{\"id\":" + jsonString(ids[i]);
        out += ",\"parent_id\":" + jsonString(parentIds[i]);
        out += ",\"text\":" + jsonString(texts[i]);
        out += ",\"children\":";
        emitList(children[i], out);
        out += "}";
    }

    void emitList(const vector<int>& nodes, string& out) {
        out += "[";
        for (int k = 0; k < (int)nodes.size(); k++) {
            if (k > 0) {
                out += ",";
            }
            emit(nodes[k], out);
        }
        out += "]";
    }

public:
    // First map every id to its index, so a child seen before its parent still finds
    // it; then link children in input order. Emitting is a DFS. T: O(n + output),
    // S: O(n) (recursion depth = tree height).
    CommentTree(const vector<string>& ids, const vector<string>& parentIds,
                const vector<string>& texts)
        : ids(ids), parentIds(parentIds), texts(texts), children(ids.size()) {
        // indexOf[id] = position of that comment in the input
        unordered_map<string, int> indexOf;
        for (int i = 0; i < (int)ids.size(); i++) {
            indexOf[ids[i]] = i;
        }
        for (int i = 0; i < (int)ids.size(); i++) {
            if (parentIds[i].empty()) {
                roots.push_back(i);
            } else {
                children[indexOf[parentIds[i]]].push_back(i);
            }
        }
    }

    string toJson() {
        string out;
        emitList(roots, out);
        return out;
    }
};

string buildCommentTree(const vector<string>& ids, const vector<string>& parentIds,
                        const vector<string>& texts) {
    CommentTree tree(ids, parentIds, texts);
    return tree.toJson();
}

void test(const string& label, const vector<string>& ids, const vector<string>& parents,
          const vector<string>& texts, const string& want) {
    bool ok = buildCommentTree(ids, parents, texts) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", {}, {}, {}, "[]");
    test("child-first", {"b", "a"}, {"a", ""}, {"hi", "top"},
         "[{\"id\":\"a\",\"parent_id\":\"\",\"text\":\"top\",\"children\":["
         "{\"id\":\"b\",\"parent_id\":\"a\",\"text\":\"hi\",\"children\":[]}]}]");
    test("two-roots", {"x", "y"}, {"", ""}, {"1", "2"},
         "[{\"id\":\"x\",\"parent_id\":\"\",\"text\":\"1\",\"children\":[]},"
         "{\"id\":\"y\",\"parent_id\":\"\",\"text\":\"2\",\"children\":[]}]");
    test("escape", {"q"}, {""}, {"say \"hi\"\\\n"},
         "[{\"id\":\"q\",\"parent_id\":\"\",\"text\":\"say \\\"hi\\\"\\\\\\n\","
         "\"children\":[]}]");
}
