#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;
class Dir {
    public:

    string name;
    int size=0;
    Dir *parent=NULL;
    unordered_map<string,Dir*> members;
    //vector<Dir*> members;
    bool isFile;

    Dir(string name, Dir *parent, int size) {
        this->parent = parent;
        this->name = name;
        this->size = size;
        this->isFile = (size!=0);
        if (parent!=NULL) {parent->members[name] = this;}
    }

    void print() {
        this->print_recursive("");
    }
    void print_recursive(string prefix) {
        cout << prefix << this->size << "    " << this->name << '\n';
        prefix.append("  ");
        for (auto const& [key, val] : this->members) {
            val->print_recursive(prefix);
        }
    }

    void compute_sizes() {
        if (this->isFile) {return;}

        for (auto const& [key, val] : this->members) {
            val->compute_sizes();
            this->size += val->size;
        }
    }

    int get_large_dirs(int ret) {
        if (this->isFile) {return ret;}

        for (auto const& [key, val] : this->members) {ret = val->get_large_dirs(ret);}
        if (this->size <= 100000) {ret += this->size;}
        return ret;
    }

    void clear() {
        if (this->isFile) {return;}
        for (auto const& [key, val] : this->members) {
            val->clear();
            delete val;
        }
    }
};

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    Dir* working_dir = NULL;
    Dir* root = NULL;
    string r1,r2;
    while (getline(input, line)) {
        if (line[0]=='$') {
            // Reading command
            if (line=="$ cd ..") {
                working_dir = working_dir->parent;
            } else if (line.rfind("$ cd ",0)==0) {
                if (working_dir == NULL) {
                    root = new Dir("/", NULL, 0);
                    working_dir = root;
                } else {
                    working_dir = working_dir->members[line.substr(5,string::npos)];
                }
            } else if (line=="$ ls") {
                // Do Nothing
            } else {return 1;}
        } else {
            // Reading ls output
            istringstream iss(line);
            iss >> r1 >> r2;
            if (r1.compare("dir")==0) {
                working_dir->members[r2] = new Dir(r2, working_dir, 0);
            } else {
                working_dir->members[r2] = new Dir(r2, working_dir, stoi(r1));
            }
        }
    }
    root->print();
    root->compute_sizes();

    int ret = root->get_large_dirs(0);

    root->clear();
    delete root;
    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
 