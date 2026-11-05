// ByteDance onsite (FT): RECEIVE (taskId, sellerId, tier VIP / STANDARD, priority 1..3)
// or PROCESS -> "sellerId:taskId" ("" if idle). Per seller: lower priority first, then
// FIFO. Active sellers rotate in activation order; a turn serves up to 2 tasks (VIP) or
// 1 (STANDARD); an emptied seller leaves the rotation and rejoins at the back.

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::priority_queue;
using std::tuple;
using std::get;
using std::unordered_map;
using std::greater;

// Deque of active sellers (the rotation) + a min-heap of tasks per seller.
// T: O(log t) per RECEIVE / PROCESS for t tasks of that seller, S: O(total tasks).
class SellerScheduler {
    // (priority, arrival number, task id)
    using Task = tuple<int, int, string>;

    struct Seller {
        // tasks per turn: 2 for VIP, 1 for STANDARD
        int quota = 1;
        // true while the seller is in the rotation
        bool active = false;
        // pending tasks, smallest (priority, arrival) first
        priority_queue<Task, vector<Task>, greater<Task>> tasks;
    };

    // sellers[id] = that seller's state
    unordered_map<string, Seller> sellers;
    // active sellers in rotation order; the front one is being served
    deque<string> rotation;
    // tasks the front seller already got in its current turn
    int servedInTurn = 0;
    // arrival counter, keeps FIFO order among equal priorities
    int arrivals = 0;

public:
    void receive(const string& taskId, const string& sellerId, const string& tier,
                 int priority) {
        Seller& seller = sellers[sellerId];
        if (tier == "VIP") {
            seller.quota = 2;
        } else {
            seller.quota = 1;
        }
        seller.tasks.push({priority, arrivals, taskId});
        arrivals++;

        // an idle seller joins at the back of the rotation
        if (!seller.active) {
            seller.active = true;
            rotation.push_back(sellerId);
        }
    }

    string processNext() {
        if (rotation.empty()) {
            return "";
        }
        string sellerId = rotation.front();
        Seller& seller = sellers[sellerId];
        string taskId = get<2>(seller.tasks.top());
        seller.tasks.pop();
        servedInTurn++;

        if (seller.tasks.empty()) {
            // nothing left: leave the rotation
            seller.active = false;
            rotation.pop_front();
            servedInTurn = 0;
        } else if (servedInTurn == seller.quota) {
            // turn used up but tasks remain: go to the back
            rotation.pop_front();
            rotation.push_back(sellerId);
            servedInTurn = 0;
        }
        return sellerId + ":" + taskId;
    }
};

vector<string> scheduleSellerTasks(const vector<string>& operations,
                                   const vector<string>& taskIds,
                                   const vector<string>& sellerIds,
                                   const vector<string>& sellerTiers,
                                   const vector<int>& priorities) {
    SellerScheduler scheduler;
    vector<string> out;
    for (int i = 0; i < (int)operations.size(); i++) {
        if (operations[i] == "RECEIVE") {
            scheduler.receive(taskIds[i], sellerIds[i], sellerTiers[i], priorities[i]);
        } else {
            out.push_back(scheduler.processNext());
        }
    }
    return out;
}

void test(const string& label, const vector<string>& ops, const vector<string>& tasks,
          const vector<string>& sellers, const vector<string>& tiers,
          const vector<int>& priorities, const vector<string>& want) {
    bool ok = scheduleSellerTasks(ops, tasks, sellers, tiers, priorities) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("priority-then-fifo", {"RECEIVE", "RECEIVE", "RECEIVE", "PROCESS", "PROCESS",
                                "PROCESS"},
         {"t1", "t2", "t3", "", "", ""}, {"A", "A", "A", "", "", ""},
         {"STANDARD", "STANDARD", "STANDARD", "", "", ""}, {2, 1, 2, 0, 0, 0},
         {"A:t2", "A:t1", "A:t3"});
    test("vip-two-per-turn", {"RECEIVE", "RECEIVE", "RECEIVE", "RECEIVE", "PROCESS",
                              "PROCESS", "PROCESS", "PROCESS"},
         {"s1", "s2", "v1", "v2", "", "", "", ""}, {"S", "S", "V", "V", "", "", "", ""},
         {"STANDARD", "STANDARD", "VIP", "VIP", "", "", "", ""}, {1, 1, 1, 1, 0, 0, 0, 0},
         {"S:s1", "V:v1", "V:v2", "S:s2"});
    test("empty-and-rejoin", {"PROCESS", "RECEIVE", "PROCESS", "PROCESS", "RECEIVE",
                              "PROCESS"},
         {"", "a1", "", "", "a2", ""}, {"", "A", "", "", "A", ""},
         {"", "VIP", "", "", "VIP", ""}, {0, 3, 0, 0, 1, 0},
         {"", "A:a1", "", "A:a2"});
}

// follow-up: priorities only 1..3 -> per seller 3 FIFO queues, pop first non-empty, O(1)
// follow-up: cancel a task -> set<Task> per seller + taskId -> (seller, Task), erase in
//   O(log t); empty seller leaves rotation (list + iterator map, reset turn if front)
// follow-up: variable task cost -> deficit round robin: deficit += quantum per turn,
//   serve while cost <= deficit, deficit = 0 when the queue empties
