#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>

using namespace std;
string str;
struct fileInfo {
    int size;
    string name;
};

const int insert = 0;
const int transfer = 1;
const int nothing = -1;

map<string, string> parent; // key = current dictionary name, value = parent dictionary
map<string, vector<fileInfo>> files; // key = path, value = files
typedef pair<string, string> from_to;

void constructHierarchy(string path) {
    vector<string> dictionaries;
    string delimiter = "\\";

    if (path.back() == '\\')
        path.pop_back();

    size_t addr = 0;

    while ((addr = path.find(delimiter)) != path.npos) {
        string sub = path.substr(0, addr);
        dictionaries.push_back(sub);
        path.erase(0, addr + delimiter.length());
    }

    if(!path.empty())
        dictionaries.push_back(path);

    for(int i = dictionaries.size()-1 ; i>0; i--)
        parent[dictionaries[i]] = dictionaries[i-1];
}

int parseInput(const string &line, from_to &move) {
    int ret_cmd;
    size_t spaceAddr = line.find(' ');
    size_t scale; // the size of newly inserted file
    string former, latter;

    if (spaceAddr != line.npos) {  // space found in line, thus it's an insertion or movement
        former = line.substr(0, spaceAddr);
        latter = line.substr(spaceAddr + 1);
        if (isdigit(latter[0])) { // an insertion
            scale = stoi(latter);
            ret_cmd = insert;
        } else { // if a movement, return from_to;
            ret_cmd = transfer;
            move.first = former;
            move.second = latter;
//            cout<<"*****"<<move.first<<" "<<move.second<<endl;
            return ret_cmd;
        }
    } else {
        return -1;
    }

    size_t slashAddr = former.rfind('\\');
    string path = former.substr(0, slashAddr);
    fileInfo tmp;
    tmp.name = former.substr(slashAddr + 1);
    tmp.size = scale;

    files[path].push_back(tmp);

//    constructHierarchy(path);
//    cout<<path<<" "<<files[path].name<<" "<<files[path].size<<endl;
    return ret_cmd;
}


void moveDictionary(const string &from, const string &to) {
    vector<string> toErase;

    for (auto &item: files) {
        size_t pos = item.first.find(from);
        if (pos != item.first.npos) {  // the path contains the from
            string newPath = item.first;
            newPath.replace(pos, from.size(), to); // construct the new path
            files[newPath] = item.second;

            toErase.push_back(item.first);
        }
    }

    for (std::string i: toErase)
        files.erase(i);

//    for(auto &item : files)
//        cout<< item.first<<"\t"<<item.second.name<<" "<<item.second.size<<endl;
}

void getAns(const string &dest) {
    string ansPath;
    int scale = 0, totSize = 0;
    fileInfo ansFile;
    for (auto &item: files) {

        if (item.first.find(dest) != item.first.npos) {
            for(auto &File: item.second){
                totSize+=File.size;
                if(File.size>scale){
                    ansPath = item.first;
                    scale = File.size;
                    ansFile = File;
                }
            }
        }
    }
    cout << totSize << endl;
    cout << ansPath << "\\" << ansFile.name << endl;
}

int main() {
    while (std::getline(std::cin, str)) {
        from_to move;
        int status = parseInput(str, move);

        if (status == transfer) {
//            cout<<"*****"<<move.first<<" "<<move.second<<endl;
            moveDictionary(move.first, move.second);
            getAns(move.second);
        }
    }
    return 0;
}
