#include "iostream"
#include "map"
#include "vector"
#include "string"
#include "utility"
#include "sstream"
#include "algorithm"
#include "set"
using namespace std;

struct fileInfo {
    string name;
    size_t size;

    bool operator==(const fileInfo &other) const {
        return name == other.name;
    }
};

map<string, vector<fileInfo>> file; //path -> file


bool parseInput(const string &line, pair<string, string> &from_to) {
    istringstream ss(line);
    string word;
    vector<string> cmd;
    while (ss >> word)
        cmd.push_back(word);

    if (cmd.size() <= 1)
        return false;
    string former, latter;
    former = cmd[0];
    latter = cmd[1];

    if (isdigit(latter[0]) == false) {
        from_to.first = former;

        from_to.second = latter;

        return true; // a movement operation
    }


    size_t size = stoi(latter);
    size_t slashAddr = former.rfind('\\');
    string path = former.substr(0, slashAddr);
    string name = former.substr(slashAddr + 1);

    fileInfo tmp{name, size};
    file[path].push_back(tmp);
//    cout << path << endl;
    return false;
}

bool check_existence(const string &from, const string &to) {
    bool f1 = false, f2 = false;
    for (auto &path: file) {

        if (path.first.find(from) != std::string::npos)
            f1 = true;
        if (path.first.find(to) != std::string::npos)
            f2 = true;
    }

    return f1 && f2;
}

string construct_new_name(const string &s, const int idx) {
    size_t dotAddr = s.find('.');
    string name = s.substr(0, dotAddr);
    string format = s.substr(dotAddr + 1);

    string ret = name + "_" + to_string(idx) + '.' + format;
    return ret;
}

bool move_dictionary(const string &from, const string &to) {
    if (!check_existence(from, to)) {
        cout << "error" << endl;
        return false;
    }

    vector<string> toErase;
    for (auto &path: file) { // traverse each path
        size_t addr = path.first.find(from);
        if (addr == std::string::npos)
            continue;
        if (path.first.find(to) != std::string::npos && to.size()>from.size())
            continue;
        toErase.push_back(path.first);

        string newPath = path.first;
        newPath = newPath.replace(addr, from.size(), to); // replace the dictionary;

        for (auto old: path.second) {
            auto it = std::find(file[newPath].begin(), file[newPath].end(), old);
            if (it != file[newPath].end()) { // found a file with same name
                it->name = construct_new_name(it->name, 1); // modify the names of both
                old.name = construct_new_name(old.name, 2);
            }
            file[newPath].push_back(old);
        }
    }

    for (auto &str: toErase) {
        file[str].clear();
        file.erase(str);
    }
    return true;
}

struct ans_files{
    string full_path;
    size_t size;
    bool operator<(const ans_files&other) const{
        if(size > other.size)
            return true;
        if(size == other.size)
            return full_path<other.full_path;

        return false;
    }
};
void get_ans(const string &to) {

    size_t totSize = 0;

    set<ans_files> ans;
    for (auto &path: file) {
        if (path.first.find(to) == std::string::npos)
            continue;
        for (auto &files: path.second) {
            totSize += files.size;
            string full_path = path.first + '\\' + files.name;
            ans_files tmp{full_path, files.size};
            ans.insert(tmp);
        }
    }

    cout<<totSize<<endl;
    cout<<ans.begin()->full_path<<endl;
}

int main() {
    string str;

    while (getline(std::cin, str)) {
        pair<string, string> from_to;
        bool status = parseInput(str, from_to);
        if (status && move_dictionary(from_to.first, from_to.second)) {
            get_ans(from_to.second);
            return 0;
        }

    }
    return 0;
}
