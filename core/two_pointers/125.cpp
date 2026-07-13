// filter + reverse-compare, T: O(n), S: O(n)

#include <string>
#include <cctype> // std::isalnum, std::tolower
#include <algorithm> // std::reverse

class Solution {
public:
    bool isPalindrome(std::string s) {
        std::string filtered;
        filtered.reserve(s.length());

        for (int i = 0; i < static_cast<int>(s.length()); i++) {
            unsigned char c = static_cast<unsigned char>(s[i]);
            if (std::isalnum(c)) { filtered += static_cast<char>(std::tolower(c)); }
        }

        std::string reversed = filtered;
        std::reverse(reversed.begin(), reversed.end());
        return filtered == reversed;
    }
};

// two-pointers, T: O(n), S: O(1)

#include <string>
#include <cctype> // std::isalnum, std::tolower

class Solution {
public:
    bool isPalindrome(std::string s) {
        int left = 0;
        int right = static_cast<int>(s.length()) - 1;
        
        while (left < right) {
            unsigned char l = static_cast<unsigned char>(s[left]);
            unsigned char r = static_cast<unsigned char>(s[right]);

            if (!std::isalnum(l)) { left++; continue; }
            if (!std::isalnum(r)) { right--; continue; }
            if (std::tolower(l) != std::tolower(r)) { return false; }
            left++; right--;
        }
        return true;
    }
};

// static_cast<unsigned char> necessity: dodging real Unicode (signed char hold a byte >= 128 gets promoted implicitly to negative int)
