// ByteDance intern OA (1p3a 1189035, Q1): in a text, convert only the snake_case
// identifiers that appear inside backtick spans (`...`) to camelCase; upper-case
// constants such as MAX_SIZE stay unchanged, and text outside backticks is untouched.
// Assumptions (the post gave no full spec):
// - Inside a span, an identifier is a maximal run of letters, digits and '_'.
// - It is snake_case if it has an '_', no upper-case letter, does not start or end
//   with '_' and has no "__"; camelCase drops each '_' and upper-cases the next letter.
//   Every other identifier (constants, mixed case, plain words) is kept as is.
// - Backticks are kept; an opening backtick with no closing one is ordinary text.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
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

// Rewrite the identifiers of one span body; other characters are copied.
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
    if (isSnakeCase(word)) {
        out += toCamel(word);
    } else {
        out += word;
    }
    return out;
}

// Find each opening backtick and its closing partner; only the text between them is
// rewritten. T: O(n), S: O(n).
string convertBackticks(const string& text) {
    string out;
    int i = 0;
    int n = text.size();
    while (i < n) {
        int open = text.find('`', i);
        if (open == (int)string::npos) {
            out += text.substr(i);
            break;
        }
        int close = text.find('`', open + 1);
        if (close == (int)string::npos) {
            out += text.substr(i);
            break;
        }

        out += text.substr(i, open - i);
        out += '`';
        out += convertSpan(text.substr(open + 1, close - open - 1));
        out += '`';
        i = close + 1;
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
    test("plain-word", "`value`", "`value`");
}
