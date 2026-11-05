// ByteDance phone (FT): comment i has ids[i], parentIds[i] ("" = root) and texts[i];
// a child may come before its parent. Return the forest as compact JSON: objects with
// id, parent_id, text, children in that order; roots and siblings keep input order.
//   buildCommentTree(ids, parentIds, texts) -> string

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// JSON string literal: quote it and escape ", \ and newline / tab.
string jsonString(const string& s) {
    string out = "\"";
    for (char c : s) {
        if (c == '"') {
            out += "\\\"";
        } else if (c == '\\') {
            out += "\\\\";
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\t') {
            out += "\\t";
        } else {
            out += c;
        }
    }
    out += "\"";
    return out;
}

// Map id -> index first (child may precede parent), link children, then DFS to emit.
// T: O(n + output), S: O(n).
class CommentTree {
    // input columns, comment i = (ids[i], parentIds[i], texts[i])
    const vector<string>& ids;
    const vector<string>& parentIds;
    const vector<string>& texts;
    // children[i] = indices of comment i's replies, in input order
    vector<vector<int>> children;
    // indices of root comments, in input order
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

// follow-up: very deep thread (stack overflow) -> iterative DFS, stack of (node, next
//   child idx); emit "{...[" on push, "]}" on pop, O(n)
// follow-up: siblings by likes / time -> sort children[i] by that key, O(n log n)
// follow-up: missing parent / cycle -> indexOf.find() == end(): treat as root or reject;
//   cycle = nodes never reached from roots (visited count < n)
// follow-up: other control chars -> emit "\u00XX" for c < 0x20
