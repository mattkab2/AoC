#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

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
    vector<tuple<int, char*, char*>> inventory;
    char *inv1, *inv2;
    int n;
    while (getline(input, line)) {
        n = line.length();
        inv1 = new char[n/2];
        inv2 = new char[n/2];
        for (int i=0; i<n/2; i++) {
            inv1[i] = line[i];
            inv2[i] = line[n/2+i];
        }
        inventory.push_back(make_tuple(n/2,inv1,inv2));
    }
    int ret=0;
    for (int i=0; i<inventory.size(); i++) {
        // Search for duplicate
        auto [ n, inv1, inv2 ] = inventory[i];
        int j=0; int k=0;
        while (inv1[j]!=inv2[k]) {
            k++;
            if (k>=n) {k=0; j++;}
        }
        ret += priority(inv1[j]);
    }
    /*
    for (int i=0; i<inventory.size(); i++) {
        auto [ n, inv1, inv2 ] = inventory[i];
        for (int j=0; j<n; j++) {
            cout << inv1[j];
        }
        cout << " : ";
        for (int j=0; j<n; j++) {
            cout << inv2[j];
        }
        cout << "\n";
    }
    */

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
