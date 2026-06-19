// expand around center, T: O(n2), S: O(1)

#include <string>

class Solution {
public:
    int countSubstrings(std::string s) {
        int n = static_cast<int>(s.size());
        int count = 0;

        for (int i = 0; i < n; i++) {
            // odd length
            int lo = i, hi = i;
            while (lo >= 0 && hi < n && s[lo] == s[hi]) { count++; lo--; hi++; }

            // even length
            lo = i, hi = i + 1;
            while (lo >= 0 && hi < n && s[lo] == s[hi]) { count++; lo--; hi++; }
        }
        return count;
    }
};

// dp precomputation, T: O(n2), S: O(n2)

#include <string>
#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    int countSubstrings(std::string s) {
        int n = static_cast<int>(s.size());
        std::vector<std::vector<uint8_t>> isPalin(n, std::vector<uint8_t>(n, 0));
        int count = 0;

        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i <= 2 || isPalin[i+1][j-1])) {
                    isPalin[i][j] = 1; count++;
                }
            }
        }
        return count;
    }
};

// Manacher's O(n): p[i] is radius at each center, sum all p[i]+1 (each radius contributes radius+1 palindromes)
