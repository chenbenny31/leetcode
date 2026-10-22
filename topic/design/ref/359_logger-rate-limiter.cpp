// LC 359 Logger Rate Limiter: shouldPrintMessage(timestamp, message) returns true if
// the message was not printed in the last 10 seconds. Timestamps never decrease.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

class Logger {
    // message -> first timestamp it may print again (last printed + 10)
    unordered_map<string, int> nextOk;

public:
    // hash lookup; only printed messages are recorded. T: O(1) average, S: O(messages).
    bool shouldPrintMessage(int timestamp, const string& message) {
        auto it = nextOk.find(message);
        if (it != nextOk.end() && timestamp < it->second) { return false; }
        nextOk[message] = timestamp + 10;
        return true;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    Logger log;
    check(log.shouldPrintMessage(1, "foo"), "first");
    check(!log.shouldPrintMessage(10, "foo"), "too-soon");
    check(log.shouldPrintMessage(10, "bar"), "other-message");
    check(log.shouldPrintMessage(11, "foo"), "edge-11");
}
