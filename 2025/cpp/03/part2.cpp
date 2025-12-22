#include <array>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Because c++ is stupid
std::array<long, 12> pow10 = {long(1e11), long(1e10), long(1e9), long(1e8), long(1e7), long(1e6), long(1e5), long(1e4), long(1e3), long(1e2), 10, 1};

long max_recursive(const std::vector<int> &data, int start, int numDigits, std::map<std::pair<int, int>, long> &cache) {
    if (numDigits == 12) {
        return 0;
    }
    if (start == data.size()) {
        return std::numeric_limits<long>::min();
    }
    long result = std::max(
        pow10.at(numDigits)*data[start] + (cache.contains({start+1, numDigits+1}) ? cache.at({start+1, numDigits+1}) : max_recursive(data, start+1, numDigits+1, cache)),
        (cache.contains({start+1, numDigits}) ? cache.at({start+1, numDigits}) : max_recursive(data, start+1, numDigits, cache))
    );
    cache[{start, numDigits}] = result;
    return result;
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    std::vector<std::vector<int>> jolts;
    while (getline(input, line)) {
        std::vector<int> tmp;
        for (const auto &c : line) {
            tmp.push_back(c - '0');
        }
        jolts.push_back(tmp);
    }

    long ret = 0;
    for (const auto &bat : jolts) {
        std::map<std::pair<int, int>, long> cache;
        long max = max_recursive(bat, 0, 0, cache);
        std::cout << max << std::endl;
        ret += max;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
