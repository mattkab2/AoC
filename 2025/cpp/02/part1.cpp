#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool check_repeat(const std::string &inp) {
    if (inp.size() % 2 == 1) {return true;}
    if (inp.substr(0, inp.size()/2) == inp.substr(inp.size()/2, inp.size()/2)) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    getline(input, line);

    std::vector<std::pair<long, long>> ranges;
    size_t lo = 0;
    size_t hi = 0;
    while (lo < line.size()) {
        hi = line.find('-', lo);
        long p1 = std::stol(line.substr(lo, hi-lo));
        lo = hi+1;
        hi = line.find(',', lo);
        if (hi == std::string::npos) {
            hi = line.size();
        }
        long p2 = std::stol(line.substr(lo, hi-lo));
        ranges.emplace_back(p1, p2);
        lo = hi+1;
    }
    input.close();

    long ret = 0;
    for (const auto &[lo, hi] : ranges) {
        for (long i = lo; i <= hi; i++) {
            if (!check_repeat(std::to_string(i))) {
                ret += i;
            }
        }
    }
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
