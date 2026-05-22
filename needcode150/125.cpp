// two-pointers, T: O(n), S: O(1)

#include <string>
#include <cctype> // std::isalnum, std::tolower

class Solution {
public:
    bool isPalindrome(std::string s) {
        int left = 0;
        int right = static_cast<int>(s.size()) - 1;

        while (left < right) {
            while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) { left++; }
            while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) { right--; }

            if (std::tolower(static_cast<unsigned char>(s[left])) !=
                std::tolower(static_cast<unsigned char>(s[right]))) { return false; }
            left++; right--; 
        }
        return true;
    }
};

// unsigned char cast mandatory: std::isalnum and std::tolower are only defined for [0, 127] and EOF, signed char could > 127
// Unicode: use std::iswalnum with wchar_t cast
