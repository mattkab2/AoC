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
    std::vector<std::vector<char>> inp;
    std::vector<char> operations;
    while (getline(input, line)) {
        inp.emplace_back(line.begin(), line.end());
    }
    for (char op : inp.back()) {
        if (op != ' ') {
            operations.push_back(op);
        }
    }
    inp.erase(inp.end()-1);
    // Now pad the input
    size_t maxLen = 0;
    for (const auto &cArr : inp) {
        maxLen = std::max(maxLen, cArr.size());
    }
    for (auto &cArr : inp) {
        while (cArr.size() < maxLen) {
            cArr.push_back(' ');
        }
    }

    std::vector<int> divisions{-1};
    for (size_t i = 0; i < inp.front().size(); i++) {
        bool isDivision = true;
        for (auto &cArr : inp) {
            if (cArr[i] != ' ') {
                isDivision = false;
                break;
            }
        }
        if (isDivision) {
            divisions.push_back(i);
        }
    }
    divisions.push_back(inp.front().size());

    long ret = 0;
    // Start the problem
    for (size_t div = 0; div < divisions.size()-1; div++) {
        long tmp = 0;
        for (size_t col = divisions[div]+1; col < divisions[div+1]; col++) {
            int operand = 0;
            for (size_t row = 0; row < inp.size(); row++) {
                if (inp[row][col] != ' ') {
                    operand = 10*operand + (inp[row][col] - '0');
                }
            }
            std::cout << operand << std::endl;
            if (tmp == 0) {
                tmp = operand;
            } else {
                switch(operations[div]) {
                case '+':
                    tmp += operand;
                    break;
                case '*':
                    tmp *= operand;
                    break;
                }
            }
        }
        std::cout << tmp << std::endl;
        ret += tmp;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
