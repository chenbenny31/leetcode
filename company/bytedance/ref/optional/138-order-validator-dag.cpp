// ByteDance (FastPrep reconstruction): keep validation rules with DAG dependencies and
// process commands, returning one string per command.
//   ADD|id|field|min|deps -> rule "order field #field must be >= min"; deps is a
//     comma-separated list of rule ids it depends on, or "-". "EXISTS" if id is active;
//     "CYCLE" (nothing changes) if it would close a dependency cycle; else "ADDED".
//     A dep id that is not active yet is ignored until a rule with that id is added.
//   REMOVE|id -> "NOT_FOUND" if absent; else remove it, its dependents inherit its
//     active deps (ordering through it survives); "REMOVED".
//   VALIDATE|v0,v1,... -> check active rules in the smallest-id topological order
//     (deps first); "VALID", or "INVALID:id" for the first failing rule. A field index
//     past the values fails. "VALIDATE|" is an order with zero fields.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <functional>

using std::cout;
using std::string;
using std::pair;
using std::vector;
using std::map;
using std::set;
using std::priority_queue;
using std::greater;
using std::stoi;
using std::stoll;

// Split s on sep with a plain character loop ("" gives one empty piece).
vector<string> split(const string& s, char sep) {
    vector<string> parts;
    string cur;
    for (char ch : s) {
        if (ch == sep) {
            parts.push_back(cur);
            cur = "";
        } else {
            cur += ch;
        }
    }
    parts.push_back(cur);
    return parts;
}

struct Rule {
    // index into the order's values
    int field;
    // the value at field must be >= minValue
    long long minValue;
    // ids this rule depends on; only the active ones create edges
    set<string> deps;
};

// Rules in a sorted map; ADD and VALIDATE rerun Kahn's sort with a min-heap.
// T: O((R + E) log R) per command, S: O(R + E).
class OrderValidator {
    // rules[id] = active rule (a map keeps ids sorted)
    map<string, Rule> rules;

public:
    string handle(const string& line) {
        vector<string> parts = split(line, '|');
        if (parts[0] == "ADD") {
            return add(parts[1], stoi(parts[2]), stoll(parts[3]), parts[4]);
        }
        if (parts[0] == "REMOVE") {
            return remove(parts[1]);
        }
        return validate(parts[1]);
    }

private:
    // Active ids in smallest-id topological order; fewer ids than rules = cycle.
    vector<string> topoOrder() {
        // indeg[id] = number of active dependencies of id
        map<string, int> indeg;
        // graph[dep] = rules that depend on dep (dep must be checked first)
        map<string, vector<string>> graph;
        for (const auto& [id, rule] : rules) {
            indeg[id] = 0;
        }
        for (const auto& [id, rule] : rules) {
            for (const string& dep : rule.deps) {
                if (rules.count(dep)) {
                    graph[dep].push_back(id);
                    indeg[id]++;
                }
            }
        }

        // ready rules, smallest id on top
        priority_queue<string, vector<string>, greater<string>> ready;
        for (const auto& [id, d] : indeg) {
            if (d == 0) {
                ready.push(id);
            }
        }
        vector<string> order;
        while (!ready.empty()) {
            string cur = ready.top();
            ready.pop();
            order.push_back(cur);
            for (const string& nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) {
                    ready.push(nxt);
                }
            }
        }
        return order;
    }

    // Add tentatively; if the sort cannot place every rule, the new rule closed a cycle
    // (possibly through rules that were waiting for this id), so undo it.
    string add(const string& id, int field, long long minValue, const string& depList) {
        if (rules.count(id)) {
            return "EXISTS";
        }
        Rule rule;
        rule.field = field;
        rule.minValue = minValue;
        if (depList != "-") {
            for (const string& dep : split(depList, ',')) {
                rule.deps.insert(dep);
            }
        }

        rules[id] = rule;
        if (topoOrder().size() < rules.size()) {
            rules.erase(id);
            return "CYCLE";
        }
        return "ADDED";
    }

    // Rules that listed id now depend on id's active dependencies instead.
    string remove(const string& id) {
        if (rules.count(id) == 0) {
            return "NOT_FOUND";
        }
        set<string> inherited;
        for (const string& dep : rules[id].deps) {
            if (rules.count(dep)) {
                inherited.insert(dep);
            }
        }
        rules.erase(id);

        for (auto& [other, rule] : rules) {
            if (rule.deps.count(id)) {
                rule.deps.erase(id);
                for (const string& dep : inherited) {
                    rule.deps.insert(dep);
                }
            }
        }
        return "REMOVED";
    }

    string validate(const string& valueList) {
        vector<long long> values;
        if (!valueList.empty()) {
            for (const string& v : split(valueList, ',')) {
                values.push_back(stoll(v));
            }
        }

        for (const string& id : topoOrder()) {
            const Rule& rule = rules[id];
            if (rule.field >= (int)values.size()) {
                return "INVALID:" + id;
            }
            if (values[rule.field] < rule.minValue) {
                return "INVALID:" + id;
            }
        }
        return "VALID";
    }
};

vector<string> runOrderValidator(const vector<string>& operations) {
    OrderValidator validator;
    vector<string> out;
    for (const string& op : operations) {
        out.push_back(validator.handle(op));
    }
    return out;
}

void test(const string& label, const vector<string>& operations,
          const vector<string>& want) {
    cout << label << (runOrderValidator(operations) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dep-checked-first",
         {"ADD|b|0|3|-", "ADD|a|1|1|b", "VALIDATE|3,0", "VALIDATE|3,1"},
         {"ADDED", "ADDED", "INVALID:a", "VALID"});
    test("exists-and-cycles",
         {"ADD|x|0|0|y", "ADD|y|0|0|x", "ADD|x|0|0|-", "ADD|z|0|0|z"},
         {"ADDED", "CYCLE", "EXISTS", "CYCLE"});
    test("topo-beats-name", {"ADD|c|0|9|-", "ADD|a|0|9|c", "VALIDATE|0"},
         {"ADDED", "ADDED", "INVALID:c"});
    test("remove-rewires", {"ADD|z|0|1|-", "ADD|m|0|0|z", "ADD|a|0|1|m", "REMOVE|m",
                            "VALIDATE|0", "REMOVE|m"},
         {"ADDED", "ADDED", "ADDED", "REMOVED", "INVALID:z", "NOT_FOUND"});
    test("missing-field", {"ADD|r|2|0|-", "VALIDATE|5"}, {"ADDED", "INVALID:r"});
}

// follow-up: cheaper cycle check on ADD -> DFS from id along graph (dependents); cycle
//   iff it reaches one of id's active deps (or id lists itself); O(R + E), no heap
// follow-up: frequent VALIDATE, rare ADD/REMOVE -> cache topoOrder(), rebuild only when
//   rules change; VALIDATE becomes O(R)
// follow-up: report every failing rule -> don't return early; collect ids in topo order
