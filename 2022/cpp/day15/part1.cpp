#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;
void range_insert(list<pair<long,long>> &lst, pair<long,long> range) {
    auto marker = lst.begin();
    while (marker!=lst.end() and range.first > marker->first) {marker++;}

    if (marker==lst.end()) {lst.emplace_back(range);}
    else {lst.insert(marker,range);}
    return;
}

using namespace std;
int main(int argc, char *argv[]) {
    long ylevel = 2000000;
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    long xsens,ysens,xbeac,ybeac;
    long lo,hi;
    list<pair<long,long>> ranges;
    auto beacons_at_level = list<long>();
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> xsens >> ysens >> xbeac >> ybeac;
        if (ybeac==ylevel) {
            auto loc = find(beacons_at_level.begin(),beacons_at_level.end(),xbeac);
            if (loc==beacons_at_level.end()) {beacons_at_level.emplace_back(xbeac);}
        }
        long dist = abs(xbeac-xsens) + abs(ybeac-ysens);
        if (abs(ysens-ylevel) > dist) {continue;}

        lo = xsens - (dist - abs(ylevel - ysens));
        hi = xsens + (dist - abs(ylevel - ysens));
        cout << "Inserting: " << lo << "-" << hi << '\n';
        range_insert(ranges, pair<long,long>(lo,hi));
    }
    input.close();

    long ret=0;
    long prev=-1e9;
    for (auto p : ranges) {
        if (prev >= p.second) {continue;}
        cout << p.first << "-" << p.second << " with prev: " << prev << '\n';
        cout << "  adding: " << p.second - max(p.first,prev) + 1 << '\n';
        ret += p.second - max(p.first,prev+1) + 1;
        prev = p.second;
    }
    ret = ret-beacons_at_level.size();


    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
