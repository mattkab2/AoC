#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
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
        int max = 0;
        for (size_t i = 0; i < bat.size()-1; i++) {
            for (size_t j = i+1; j < bat.size(); j++) {
                max = std::max(max, bat[i]*10 + bat[j]);
            }
        }
        std::cout << max << std::endl;
        ret += max;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
