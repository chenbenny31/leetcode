// PlusAI (JD): Schema Compat Check (Medium). Pattern: hash map by field number.
// A protobuf-like schema has fields (name, number, type, required) plus `removed`:
// numbers of fields deleted in earlier versions. New is backward compatible with old iff
// every old number still exists with the same type (renames ok, so no old number changes
// type), every field added in new is optional, and no new field reuses a removed number.

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

struct Field {
    string name;
    int number;
    string type;
    bool required;
};

// This is a hash-map join on field number: index both schemas by number, then check each
// rule with O(1) lookups. T: O(n + m + r), S: O(n + m + r).
bool isBackwardCompatible(const vector<Field>& oldFields, const vector<int>& removed,
                          const vector<Field>& newFields) {
    // field number -> type, for each schema
    unordered_map<int, string> oldType;
    unordered_map<int, string> newType;
    for (const Field& field : oldFields) { oldType[field.number] = field.type; }
    for (const Field& field : newFields) { newType[field.number] = field.type; }
    // numbers retired in earlier versions
    unordered_set<int> retired(removed.begin(), removed.end());
    for (const Field& field : oldFields) {
        auto it = newType.find(field.number);
        // names are not compared: a rename keeps the wire format
        if (it == newType.end() || it->second != field.type) { return false; }
    }
    for (const Field& field : newFields) {
        if (retired.count(field.number)) { return false; }
        bool added = oldType.count(field.number) == 0;
        if (added && field.required) { return false; }
    }
    return true;
}

void test(const string& label, const vector<Field>& oldFields, const vector<int>& removed,
          const vector<Field>& newFields, bool want) {
    bool got = isBackwardCompatible(oldFields, removed, newFields);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<Field> old = {{"ts", 1, "int64", true}, {"speed", 2, "float", false}};
    vector<int> removed = {3};
    test("rename-add-optional", old, removed,
         {{"timestamp", 1, "int64", true}, {"speed", 2, "float", false},
          {"gear", 4, "int32", false}}, true);
    test("number-new-type", old, removed,
         {{"ts", 1, "int64", true}, {"lane", 2, "int32", false}}, false);
    test("added-required", old, removed,
         {{"ts", 1, "int64", true}, {"speed", 2, "float", false},
          {"gear", 4, "int32", true}}, false);
    test("reuses-removed", old, removed,
         {{"ts", 1, "int64", true}, {"speed", 2, "float", false},
          {"lane", 3, "int32", false}}, false);
}

// follow-up: allow deleting fields -> an old number missing from new is fine if new adds
//   it to `removed` (protobuf `reserved`); then only the type and reuse checks remain
// follow-up: nested messages -> run the same check recursively per message type;
//   wire-compatible widenings (int32 -> int64) -> a small table of allowed type pairs
