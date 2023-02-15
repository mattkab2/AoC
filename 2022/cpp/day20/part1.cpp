#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    list<int> file;
    list<int> instr;
    

    while (getline(input, line)) {
        file.emplace_back(stoi(line));
    }
    instr = list<int>(file);

    for (auto p : instr) {
        if (p==0) {continue;}
        auto init_loc = find(file.begin(),file.end(),p);
        auto loc = init_loc;
        loc++;
        int tmp = *init_loc;
        file.erase(init_loc);

        if (p > 0) {
            while (p > 0) {
                if (loc==file.end()) {
                    loc = file.begin();
                } 
                loc++;
                p--;
            }
        } else {
            while (p < 0) {
                if (loc==file.begin()) {
                    loc = file.end();
                }
                loc--;
                p++;
            }
        }
        file.insert(loc,tmp);
        /*
        for (auto p : file) {
            cout << p << endl;
        }
        cout << "====" << endl;
        */
    }

    int ret = 0;
    auto loc = find(file.begin(),file.end(),0);
    int i=1000;
    while (i > 0) {
        if (loc==file.end()) {
            loc = file.begin();
        } 
        loc++;
        i--;
    }
    cout << "First coord: " << *loc << endl;
    ret += *loc;
    i=1000;
    while (i > 0) {
        if (loc==file.end()) {
            loc = file.begin();
        } 
        loc++;
        i--;
    }
    cout << "Second coord: " << *loc << endl;
    ret += *loc;
    i=1000;
    while (i > 0) {
        if (loc==file.end()) {
            loc = file.begin();
        } 
        loc++;
        i--;
    }
    cout << "Third coord: " << *loc << endl;
    ret += *loc;

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
