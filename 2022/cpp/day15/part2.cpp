#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>

int dist(int i, int j, int x, int y) {
    return abs(i-x) + abs(j-y);
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<array<long,3>> sensors;
    vector<array<long,2>> beacons;

    long lo=0; long hi=4000000;

    long xsens,ysens,xbeac,ybeac;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> xsens >> ysens >> xbeac >> ybeac;
        sensors.push_back(array<long,3>({xsens,ysens,dist(xsens,ysens,xbeac,ybeac)}));
        if (find(beacons.begin(),beacons.end(),array<long,2>({xbeac,ybeac}))==beacons.end()) {
            beacons.push_back({xbeac,ybeac});
        }
    }
    for (size_t i=0; i<sensors.size(); i++) {
        cout << sensors[i][0] << "," << sensors[i][1] << ": " << sensors[i][2] << '\n';
    }

    bool covered;
    for (long i=lo; i<=hi; i++) {
        cout << "Row " << i << " / " << hi <<'\n';
        for (long j=lo; j<=hi; j++) {
            covered = false;
            for (size_t k=0; k<sensors.size(); k++) {
                if (dist(i,j,sensors[k][0],sensors[k][1]) <= sensors[k][2]) {
                    covered = true;
                    break;
                }
            }
            if (not covered) {
                auto loc = find(beacons.begin(),beacons.end(),array<long,2>({i,j}));
                if (loc==beacons.end()) {
                    cout << "FOUND " << i << "," << j << '\n';
                    cout << i*hi+j << '\n';
                    return 0;
                }
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << 0 << '\n';
    cout << "#####################\n";
    return 0;
}
