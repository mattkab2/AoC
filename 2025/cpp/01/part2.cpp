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
    int acc = 50;
    int tmp = 0;
    int ret = 0;
    while (getline(input, line)) {
        istringstream iss(line.substr(1, line.size()-1));
        iss >> tmp;
        ret += tmp/100;
        if (line.data()[0] == 'L') {
            for (int i = 0; i < tmp%100; i++) {
                acc--;
                if (acc < 0) {
                    acc += 100;
                }
                if (acc == 0) {
                    ret++;
                }
            }
        } else {
            for (int i = 0; i < tmp%100; i++) {
                acc++;
                if (acc > 99) {
                    acc -= 100;
                }
                if (acc == 0) {
                    ret++;
                }
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
