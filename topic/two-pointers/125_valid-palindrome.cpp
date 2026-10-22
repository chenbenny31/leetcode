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
