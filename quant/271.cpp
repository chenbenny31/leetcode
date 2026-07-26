// length-prefix, T: O(n), S: O(n)

#include <vector>
#include <string> // std::to_string, std::stoi

class Codec {
private:
    static constexpr char DELIM = '#';
public:
    std::string encode(std::vector<std::string>& strs) {
        std::string out;
        for (std::string s : strs) {
            out += std::to_string(s.length());
            out += DELIM;
            out += s;
        }
        return out;
    }

    std::vector<std::string> decode(std::string s) {
        std::vector<std::string> out;
        const int n = static_cast<int>(s.length());
        int i = 0;
        while (i < n) {
            int j = i;
            while (s[j] != DELIM) { j++; }
            int len = std::stoi(s.substr(i, j - i));
            int start = j + 1;
            out.push_back(s.substr(start, len));
            i = start + len;
        }
        return out;
    }
};
