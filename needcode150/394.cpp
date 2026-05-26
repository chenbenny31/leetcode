// iterative stack, T: O(nk), k = max repeat, S: O(n)

#include <string>
#include <stack> // std::stack
#include <cctype> // std::isdigit

class Solution {
public:
    std::string decodeString(std::string s) {
        std::stack<int> cnt_stk;
        std::stack<std::string, std::vector<string>> str_stk;
        std::string curr;
        int k = 0;

        for (char c : s) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                k = k * 10 + (c - '0');
            } else if (c == '[') {
                cnt_stk.push(k);
                str_stk.push(std::move(curr)); // curr init to ""
                curr.clear();
                k = 0; // k init to 0
            } else if (c == ']') {
                int repeat = cnt_stk.top(); cnt_stk.pop();
                std::string prev = std::move(str_stk.top()); str_stk.pop();
                prev.reserve(prev.size() + repeat * curr.size());
                for (int i = 0; i < repeat; i++) { prev += curr; }
                curr = std::move(prev);
            } else {
                curr += c;
            }
        }
        return curr;
    }
};

// recursive, T: O(nk), S: O(depth) stack frames

#include <string>
#include <cctype> // std::isdigit

class Solution {
public:
    std::string decodeString(std::string s) {
        int idx = 0;
        return decode(s, idx);
    }

private:
    std::string decode(const std::string s, int& idx) {
        std::string out;

        while (idx < static_cast<int>(s.size()) && s[idx] != ']') {
            if (std::isdigit(static_cast<unsigned char>(s[idx]))) {
                int k = 0;
                while (std::isdigit(static_cast<unsigned char>(s[idx]))) {
                    k = k * 10 + (s[idx] - '0');
                    idx++;
                }
                idx++; // skip '['
                std::string inner = decode(s, idx);
                idx++; // skip ']'
                out.reserve(out.size() + k * inner.size());        
                for (int i = 0; i < k; i++) { out += inner; }
            } else {
                out += s[idx];
                idx++;
            }
        }
        return out;
    }
};

// std::move on stack and pop: O(1) buf trans, avoid copy of accum string
// prev.reserve before repeat loop: single alloc
// k = k * 10 + (c - '0'): handles multi-digit repeat counts
// recursive idx by ref: single idx shared across frames, adv global
// iterative vs recursive: iterative has no stack overflow risk, recursive is preferred for small nesting depth
