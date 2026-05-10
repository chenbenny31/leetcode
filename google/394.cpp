// stack, T: O(nk), K = max repeat count, S: O(n)

#include <string>
#include <stack>
#include <cctype> // std::isdigit

class Solution {
public:
    std::string decodeString(std::string s) {
        std::stack<int> cntStk;
        std::stack<std::string> strStk;
        std::string curr;
        int k = 0;

        for (char c : s) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                k = k * 10 + (c - '0');
            } else if (c == '[') {
                cntStk.push(k);
                strStk.push(std::move(curr));
                curr.clear();
                k = 0;
            } else if (c == ']') {
                int repeat = cntStk.top();
                cntStk.pop();
                std::string prev = std::move(strStk.top());
                prev.reserve(prev.size() + repeat * curr.size());
                strStk.pop();
                for (int i = 0; i < repeat; ++i) { prev += curr; }
                curr = std::move(prev);
            } else {
                curr += c;
            }
        }
        return curr;
    }
};

// recursion with index, T: O(nk), S: O(n + d), d: nesting depth

#include <string>
#include <cctype> // std::isdigit

class Solution {
public:
    std::string decodeString(std::string s) {
        int idx = 0;
        return decode(s, idx);
    }

private:
    std::string decode(const std::string& s, int& idx) {
        std::string res;

        while (idx < static_cast<int>(s.size()) && s[idx] != ']') {
            if (std::isdigit(static_cast<unsigned char>(s[idx]))) {
                int k = 0;
                while (std::isdigit(static_cast<unsigned char>(s[idx]))) {
                    k = k * 10 + (s[idx++] - '0');
                }
                
                ++idx; // skip '['
                std::string inner = decode(s, idx);
                ++idx; // skip ']'

                res.reserve(res.size() + k * inner.size());
                for (int i = 0; i < k; ++i) { res += inner; }
            } else {
                res += s[idx++];
            }
        }
        return res;
    }
};

// std::move(curr): no copy
// std::stack<std::string> overhead: std::deque use heap allocation
// prev += curr not curr = prev + curr: amortized O(|curr|) instead of creating temporaty copy of O(|prev| + |curr|)
