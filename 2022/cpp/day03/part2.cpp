#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

int priority(const char c) {
    int ret = c;
    if (ret > 96) {
        ret = ret - 96;
    } else {
        ret = ret - 64 + 26;
    }
    return ret;
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<vector<char>> inventory;
    int n,idx;
    while (getline(input, line)) {
        n = line.length();
        inventory.push_back(vector<char>(n));
        idx = inventory.size()-1;
        for (int i=0; i<n; i++) {
            inventory[idx][i] = line[i];
        }
    }

    int ret=0;
    for (int idx=0; idx<inventory.size(); idx+=3) {
        int loc;
        for (int i=0; i<inventory[idx].size(); i++) {
            loc = *find(inventory[idx+1].begin(),inventory[idx+1].end(),inventory[idx][i]);
            if (loc == inventory[idx][i]) {
                loc = *find(inventory[idx+2].begin(),inventory[idx+2].end(),inventory[idx][i]);
                if (loc == inventory[idx][i]) {ret += priority(inventory[idx][i]); break;}
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
