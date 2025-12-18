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
    std::vector<std::vector<bool>> map;
    while (getline(input, line)) {
        std::vector<bool> tmp{false};
        for (const auto &c : line) {
            tmp.push_back(c == '@');
        }
        tmp.push_back(false);
        if (map.empty()) {
            map.emplace_back(tmp.size(), false);
        }
        map.push_back(tmp);
    }
    map.emplace_back(map.front().size(), false);

    long ret = 0;
    std::vector<std::vector<char>> toPrint;
    for (size_t i = 1; i < map.size()-1; i++) {
        std::vector<char> tmp;
        for (size_t j = 1; j < map.front().size()-1; j++) {
            if (map[i][j]) {
                // Check adjacent
                int sum = 0;
                sum = sum + (map[i-1][j-1] ? 0 : 1);
                sum = sum + (map[i-1][j  ] ? 0 : 1);
                sum = sum + (map[i-1][j+1] ? 0 : 1);
                sum = sum + (map[i  ][j-1] ? 0 : 1);
                sum = sum + (map[i  ][j+1] ? 0 : 1);
                sum = sum + (map[i+1][j-1] ? 0 : 1);
                sum = sum + (map[i+1][j  ] ? 0 : 1);
                sum = sum + (map[i+1][j+1] ? 0 : 1);
                if (sum > 4) {
                    ret++;
                    tmp.push_back('x');
                } else {
                    tmp.push_back('@');
                }
            } else {
                tmp.push_back('.');
            }
        }
        toPrint.push_back(tmp);
    }
    for (const auto &line : toPrint) {
        for (const auto &c : line) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
