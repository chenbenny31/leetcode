// expand around center, T: O(n2), S: O(1)

#include <string>
#include <algorithm> // std::max

class Solution {
public:
    std::string longestPalindrome(std::string s) {
        int n = static_cast<int>(s.size());
        int start = 0, len = 1;

        for (int i = 0; i < n; i++) {
            // odd length
            int lo = i, hi = i;
            while (lo > 0 && hi < n - 1 && s[lo-1] == s[hi+1]) { lo--; hi++; }
            if (hi - lo + 1 > len) { len = hi - lo + 1; start = lo; }

            // even length
            if (i + 1 < n && s[i] == s[i + 1]) {
                lo = i; hi = i + 1;
                while (lo > 0 && hi < n - 1 && s[lo-1] == s[hi+1]) { lo--; hi++; }
                if (hi - lo + 1 > len) { len = hi - lo + 1; start = lo; }
            }
        }
        return s.substr(start, len);
    }
};

// dp precomputation, T: O(n2), S: O(n2)

#include <string>
#include <cstdint>

class Solution {
public:
    std::string longestPalindrome(std::string s) {
        int n = static_cast<int>(s.size());
        std::vector<std::vector<uint8_t>> isPalin(n, std::vector<uint8_t>(n, 0));

        int start = 0, len = 1;

        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i <= 2 || isPalin[i+1][j-1])) {
                    isPalin[i][j] = 1;
                    if (j - i + 1 > len) {
                        len = j - i + 1;
                        start = i;
                    }
                }
            }
        }
        return s.substr(start, len);
    }
};

// expand around center: n odd-length centers + n-1 even-length gaps = 2n-1 total, each expansion O(n) worst case
// Manacher's O(n): mirror property reuses previously computed radii
