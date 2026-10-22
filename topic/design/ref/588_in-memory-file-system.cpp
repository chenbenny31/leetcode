// LC 588 Design In-Memory File System: ls(path) lists a directory's entry names sorted,
// or just the file's own name for a file path; mkdir(path) creates missing directories
// along the path; addContentToFile(filePath, content) creates the file if needed and
// appends; readContentFromFile(filePath) returns its content. Root is "/".
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::map;
using std::string;
using std::vector;

class FileSystem {
    struct Node {
        bool isFile = false;
        string content;              // file content
        map<string, Node*> kids;     // entry name -> child, sorted by name for ls
        ~Node() {
            for (auto& [name, kid] : kids) { delete kid; }
        }
    };
    Node root;

    // trie walk, creating missing nodes on the way. T: O(path length)
    Node* walk(const string& path) {
        Node* cur = &root;
        string name;
        for (char c : path + '/') {
            if (c != '/') {
                name += c;
                continue;
            }
            if (name.empty()) { continue; }
            Node*& kid = cur->kids[name];
            if (!kid) { kid = new Node(); }
            cur = kid;
            name.clear();
        }
        return cur;
    }

public:
    FileSystem() = default;
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

    // T: O(path + entries), S: O(entries)
    vector<string> ls(const string& path) {
        Node* node = walk(path);
        if (node->isFile) { return {path.substr(path.rfind('/') + 1)}; }
        vector<string> names;
        for (const auto& [name, kid] : node->kids) { names.push_back(name); }
        return names;
    }

    void mkdir(const string& path) {
        walk(path);
    }

    // T: O(path + content)
    void addContentToFile(const string& filePath, const string& content) {
        Node* node = walk(filePath);
        node->isFile = true;
        node->content += content;
    }

    string readContentFromFile(const string& filePath) {
        return walk(filePath)->content;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    FileSystem fs;
    check(fs.ls("/").empty(), "empty-root");
    fs.mkdir("/a/b");
    fs.addContentToFile("/a/z", "hi");
    fs.addContentToFile("/a/z", "!");
    check(fs.ls("/a") == vector<string>{"b", "z"}, "sorted-ls");
    check(fs.ls("/a/z") == vector<string>{"z"}, "ls-file");
    check(fs.readContentFromFile("/a/z") == "hi!", "append");
}
