// ByteDance intern OA (1p3a 1189035, Q1): convert the snake_case identifiers inside
// backtick spans (`...`) to camelCase; constants like MAX_SIZE and all text outside
// spans stay unchanged. Assumed (no full spec): an identifier is a maximal run of
// letters, digits and '_'; snake_case = has '_', no upper case, no leading / trailing
// '_' and no "__". Backticks are kept; an unclosed backtick is ordinary text.

#include <iostream>
#include <string>
#include <cstddef>
#include <cctype>

using std::cout;
using std::string;
using std::size_t;
using std::isalnum;
using std::isupper;
using std::toupper;

bool isSnakeCase(const string& word) {
    if (word.find('_') == string::npos) {
        return false;
    }
    if (word.front() == '_' || word.back() == '_') {
        return false;
    }
    if (word.find("__") != string::npos) {
        return false;
    }
    for (char ch : word) {
        if (isupper(ch)) {
            return false;
        }
    }
    return true;
}

string toCamel(const string& word) {
    string out;
    bool upperNext = false;
    for (char ch : word) {
        if (ch == '_') {
            upperNext = true;
        } else if (upperNext) {
            out += toupper(ch);
            upperNext = false;
        } else {
            out += ch;
        }
    }
    return out;
}

// Rewrite the identifiers of one span body; other characters are copied as is.
string convertSpan(const string& body) {
    string out;
    string word;
    for (char ch : body) {
        if (isalnum(ch) || ch == '_') {
            word += ch;
            continue;
        }
        if (isSnakeCase(word)) {
            out += toCamel(word);
        } else {
            out += word;
        }
        word = "";
        out += ch;
    }
    // flush the identifier that ends the span
    if (isSnakeCase(word)) {
        out += toCamel(word);
    } else {
        out += word;
    }
    return out;
}

// Scan the text; at a backtick with a closing partner, rewrite the span and jump past
// it, else copy the char. T: O(n), S: O(n).
string convertBackticks(const string& text) {
    string out;
    size_t i = 0;
    while (i < text.size()) {
        // closing backtick of a span opening at i, or npos
        size_t close = string::npos;
        if (text[i] == '`') {
            close = text.find('`', i + 1);
        }
        if (close == string::npos) {
            out += text[i];
            i++;
        } else {
            out += '`';
            out += convertSpan(text.substr(i + 1, close - i - 1));
            out += '`';
            i = close + 1;
        }
    }
    return out;
}

void test(const string& label, const string& text, const string& want) {
    cout << label << (convertBackticks(text) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", "call `get_user_id` now", "call `getUserId` now");
    test("outside-untouched", "keep snake_case but `max_len`",
         "keep snake_case but `maxLen`");
    test("constant", "`MAX_SIZE` and `buf_size`", "`MAX_SIZE` and `bufSize`");
    test("expression", "`a_b + c_d_e(x1)`", "`aB + cDE(x1)`");
    test("unclosed", "`first_one` then `open_end", "`firstOne` then `open_end");
}

// follow-up: escaped / multi-backtick spans -> char-by-char scan with inSpan state;
//   Markdown closes a span only on a backtick run of equal length
// follow-up: camel back to snake -> before each upper letter emit '_' + tolower; skip
//   all-caps constants
// follow-up: digit after '_' (a_1) -> becomes a1 (toupper is a no-op); keep the '_'
//   if the conversion must round-trip
