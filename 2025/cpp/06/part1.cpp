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
    std::vector<std::vector<int>> operands;
    std::vector<char> operations;
    while (getline(input, line)) {
        std::vector<int> tmp;
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            try {
                tmp.push_back(std::stoi(word));
            } catch (std::invalid_argument) {
                operations.push_back(word[0]);
            }
        }
        if (!tmp.empty()) {
            operands.push_back(tmp);
        }
    }

    long ret = 0;
    for (size_t problem = 0; problem < operations.size(); problem++) {
        long tmp = operands[0][problem];
        for (size_t oIdx = 1; oIdx < operands.size(); oIdx++) {
            switch(operations[problem]) {
            case '+':
                tmp += operands[oIdx][problem];
                break;
            case '*':
                tmp *= operands[oIdx][problem];
                break;
            }
        }
        ret += tmp;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
