// Employee Hierarchy (Google, interview report)
// add(id, managerId, level): managerId = -1 for the CEO; managers are added first
// (1) removeReassign(id): its direct reports move up to its manager
// (2) removePromote(id): its most senior direct report takes the seat
// (2) most senior = higher level, then joined first; the others report to the successor
// reported: early-career onsite R2 (delete manager, delete CEO, cycles); 1p3a bank
//
// clarify:
// add an id twice, unknown manager, second CEO -> rejected (false)
// cycle -> cannot form: every add needs a new id and an existing manager
// (1) remove the CEO while it has reports -> rejected, nobody to inherit them
// (2) remove someone w/o reports -> just removed
// level -> larger = more senior

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::unordered_map;
using std::pair;

struct Employee {
    // -1 for the CEO
    int manager;
    int level;
    // join order, smaller = earlier
    int joined;
    // ids of direct reports
    set<int> reports;
};

// T: add O(log r), remove O(r log r), S: O(n) — r = direct reports involved
class OrgChart {
    // staff[id] = that employee's record
    unordered_map<int, Employee> staff;
    int ceo = -1;
    int clock = 0;

public:
    bool add(int id, int managerId, int level) {
        if (staff.count(id)) { return false; }
        if (managerId == -1) {
            if (ceo != -1) { return false; }
            ceo = id;
        } else {
            if (!staff.count(managerId)) { return false; }
            staff[managerId].reports.insert(id);
        }
        staff[id] = {managerId, level, clock, {}};
        clock++;
        return true;
    }

    // (1) core idea: hand every direct report to the removed employee's manager
    bool removeReassign(int id) {
        if (!staff.count(id)) { return false; }
        Employee gone = staff[id];
        if (gone.manager == -1 && !gone.reports.empty()) { return false; }
        for (int report : gone.reports) {
            staff[report].manager = gone.manager;
            staff[gone.manager].reports.insert(report);
        }
        if (gone.manager == -1) {
            ceo = -1;
        } else {
            staff[gone.manager].reports.erase(id);
        }
        staff.erase(id);
        return true;
    }

    // (2) core idea: pick the successor, move it into the empty seat, re-parent the rest
    bool removePromote(int id) {
        if (!staff.count(id)) { return false; }
        Employee gone = staff[id];
        // successor = highest level, then earliest joined; -1 if no reports
        int next = -1;
        for (int report : gone.reports) {
            if (next == -1) {
                next = report;
                continue;
            }
            const Employee& a = staff[report];
            const Employee& b = staff[next];
            if (a.level > b.level || (a.level == b.level && a.joined < b.joined)) {
                next = report;
            }
        }
        if (gone.manager != -1) { staff[gone.manager].reports.erase(id); }
        if (next != -1) {
            for (int report : gone.reports) {
                if (report == next) { continue; }
                staff[report].manager = next;
                staff[next].reports.insert(report);
            }
            staff[next].manager = gone.manager;
            if (gone.manager != -1) { staff[gone.manager].reports.insert(next); }
        }
        if (gone.manager == -1) { ceo = next; }
        staff.erase(id);
        return true;
    }

    // manager of id; -1 for the CEO, -2 if id is not in the chart
    int managerOf(int id) {
        if (!staff.count(id)) { return -2; }
        return staff[id].manager;
    }
};

void test(const string& label, const vector<pair<string, vector<int>>>& ops,
          const vector<int>& ids, const vector<int>& want) {
    OrgChart chart;
    for (const auto& [op, args] : ops) {
        if (op == "add") {
            chart.add(args[0], args[1], args[2]);
        } else if (op == "reassign") {
            chart.removeReassign(args[0]);
        } else {
            chart.removePromote(args[0]);
        }
    }
    vector<int> got;
    for (int id : ids) { got.push_back(chart.managerOf(id)); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1 = CEO; 2, 3 report to 1; 4, 5, 6 report to 2
    vector<pair<string, vector<int>>> base = {
        {"add", {1, -1, 9}}, {"add", {2, 1, 5}}, {"add", {3, 1, 5}},
        {"add", {4, 2, 3}}, {"add", {5, 2, 4}}, {"add", {6, 2, 4}}};

    vector<pair<string, vector<int>>> ops = base;
    ops.push_back({"reassign", {2}});
    test("reassign-to-grand-manager", ops, {2, 4, 5, 6}, {-2, 1, 1, 1});

    ops = base;
    ops.push_back({"reassign", {1}});
    test("reassign-ceo-rejected", ops, {1, 2, 3}, {-1, 1, 1});

    ops = base;
    ops.push_back({"promote", {2}});
    test("promote-level-then-joined", ops, {2, 5, 4, 6}, {-2, 1, 5, 5});

    ops = base;
    ops.push_back({"promote", {1}});
    test("promote-new-ceo", ops, {1, 2, 3, 4}, {-2, -1, 2, 2});

    ops = base;
    ops.push_back({"add", {4, 3, 1}});
    ops.push_back({"add", {7, 99, 1}});
    ops.push_back({"add", {8, -1, 1}});
    test("bad-adds-ignored", ops, {4, 7, 8}, {2, -2, -2});
}

// follow-up:
// move(id, newManager) -> the one op that can make a cycle: walk up from newManager
// that walk meets id -> reject
// remove a whole team -> DFS from id, erase every node, detach id from its manager
// num of people under x -> DFS; or keep subtree sizes and fix the path up on each change
// successor in O(log r) -> per manager a set ordered by (-level, joined)
// print the chart by level -> BFS from the CEO
// lowest common manager of a and b -> depth + walk up (LCA w/ parent pointers)
