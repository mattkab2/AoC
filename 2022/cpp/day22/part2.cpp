#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<int> ret;
    int acc = 0;
    int tmp = 0;
    while (getline(input, line)) {
        if (line=="") {
            ret.push_back(acc);
            acc = 0;
            continue;
        }
        istringstream iss(line);
        iss >> tmp;
        acc += tmp;
    }
    input.close();

    sort(ret.begin(), ret.end(), greater<int>());

    input.close();
    cout << "#####################\n";
    cout << ret[0]+ret[1]+ret[2] << '\n';
    cout << "#####################\n";
    return 0;
}
