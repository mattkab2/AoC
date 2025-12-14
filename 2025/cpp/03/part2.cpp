#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

ulong callCount = 0;

long max_recursive(const std::vector<int> &data, int start, long total, int numDigits) {
    callCount++;
    if (numDigits == 12) {
        return total;
    }
    if (start == data.size()) {
        return 0;
    }
    return std::max(
        max_recursive(data, start+1, 10*total + data[start], numDigits+1),
        max_recursive(data, start+1,    total,               numDigits)
    );
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
        long max = max_recursive(bat, 0, 0, 0);
        std::cout << max << std::endl;
        std::cout << callCount << std::endl;
        ret += max;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
