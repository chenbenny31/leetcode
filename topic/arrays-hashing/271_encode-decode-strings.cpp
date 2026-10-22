// length-prefix + string_view, T: O(n), S: O(n)

#include <vector>
#include <string> // std::to_string
#include <string_view>
#include <charconv> // std::from_chars

class Codec {
private:
    static constexpr char DELIM = '#';
    static constexpr int MAX_DIGITS = 3; // strs[i].length <= 300

public:
    std::string encode(std::vector<std::string>& strs) {
        int total = 0;
        for (const std::string& s : strs) { total += s.size() + MAX_DIGITS + 1; } // digits + delim
        std::string out;
        out.reserve(total);

        for (const std::string& s : strs) {
            out += std::to_string(s.size());
            out += DELIM;
            out += s;
        }
        return out;
    }

    std::vector<std::string> decode(std::string s) {
        std::vector<std::string> out;
        std::string_view buf(s);
        int i = 0;

        while (i < buf.size()) {
            int j = i;

            while (buf[j] != DELIM) { j++; }

            int len = 0;
            std::from_chars(buf.data() + i, buf.data() + j, len);

            int start = j + 1;
            out.emplace_back(buf.substr(start, len));
            i = start + len;
        }
        return out;
    }
};
