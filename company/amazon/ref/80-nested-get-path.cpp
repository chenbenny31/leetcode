// Implement get(obj, path) on nested maps and lists. The path uses ".key" for map keys
// and "[i]" for list indices, e.g. "a[0].b[1][2]". Return the value found, or nothing if
// any step is missing (unknown key, index out of range, or wrong container type).

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::isdigit;
using std::stoi;

// A value is a map, a list, or a string leaf.
struct Value {
    enum Kind { MAP, LIST, LEAF } kind;
    // fields of a MAP, items of a LIST, text of a LEAF
    vector<pair<string, Value>> fields;
    vector<Value> items;
    string text;
};

Value leaf(const string& s) { return {Value::LEAF, {}, {}, s}; }
Value list(const vector<Value>& items) { return {Value::LIST, {}, items, ""}; }
Value object(const vector<pair<string, Value>>& fields) {
    return {Value::MAP, fields, {}, ""};
}

const Value* child(const Value* v, const string& key) {
    if (v->kind != Value::MAP) { return nullptr; }
    for (const auto& [k, val] : v->fields) {
        if (k == key) { return &val; }
    }
    return nullptr;
}

const Value* item(const Value* v, int i) {
    if (v->kind != Value::LIST || i < 0 || i >= (int)v->items.size()) { return nullptr; }
    return &v->items[i];
}

// Walk the path once: read a key up to '.' or '[', or an index up to ']', and step into
// it. T: O(path length * map width), S: O(1).
const Value* get(const Value& obj, const string& path) {
    const Value* cur = &obj;
    int n = path.size(), i = 0;
    while (i < n && cur) {
        if (path[i] == '.') {
            i++;
        } else if (path[i] == '[') {
            int j = i + 1;
            while (j < n && isdigit(path[j])) { j++; }
            if (j == i + 1 || j >= n || path[j] != ']') { return nullptr; }
            cur = item(cur, stoi(path.substr(i + 1, j - i - 1)));
            i = j + 1;
        } else {
            int j = i;
            while (j < n && path[j] != '.' && path[j] != '[') { j++; }
            cur = child(cur, path.substr(i, j - i));
            i = j;
        }
    }
    return cur;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // {"a": [{"b": ["x", ["y", "z"]]}, "w"], "k": "v"}
    Value inner = object({{"b", list({leaf("x"), list({leaf("y"), leaf("z")})})}});
    Value root = object({{"a", list({inner, leaf("w")})}, {"k", leaf("v")}});
    const Value* deep = get(root, "a[0].b[1][1]");
    check(deep && deep->text == "z", "deep");
    check(get(root, "k") && get(root, "k")->text == "v", "key");
    check(get(root, "a[5]") == nullptr, "out-of-range");
    check(get(root, "a.b") == nullptr, "wrong-type");
    check(get(root, "q") == nullptr, "missing");
}
