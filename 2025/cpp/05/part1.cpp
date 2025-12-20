#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    std::vector<std::pair<long,long>> ranges;
    std::vector<long> ids;
    bool readPairs = true;
    while (getline(input, line)) {
        if (line.empty()) {
            readPairs = false;
            continue;
        }
        if (readPairs) {
            size_t dash = line.find('-', 0);
            long lo = std::stol(line.substr(0, dash));
            long hi = std::stol(line.substr(dash+1, line.size() - dash));
            ranges.emplace_back(lo,hi);
        } else {
            ids.push_back(std::stol(line));
        }
    }

    long ret = 0;
    for (const auto &id : ids) {
        bool fresh = false;
        for (const auto &[lo, hi] : ranges) {
            if (id >= lo && id <= hi) {
                fresh = true;
                break;
            }
        }
        if (fresh) {ret++;}
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
