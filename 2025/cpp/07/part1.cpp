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

    std::vector<std::vector<int>> geometry;
    std::vector<std::vector<int>> state;
    while (getline(input, line)) {
        std::vector<int> tmp;
        for (const char &c : line) {
            switch(c) {
            case '.':
                tmp.push_back(0);
                break;
            case 'S':
                tmp.push_back(-1);
                break;
            case '^':
                tmp.push_back(1);
                break;
            }
        }
        geometry.push_back(tmp);
        state.emplace_back(tmp.size(), 0);
    }

    long numSplits = 0;
    for (size_t row = 0; row < geometry.size(); row++) {
        for (size_t col = 0; col < geometry.front().size(); col++) {
            if (geometry[row][col] == -1) {
                state[row][col] = 1;
            }
            if (row > 0 && state[row-1][col] == 1) {
                if (geometry[row][col] == 0) {
                    state[row][col] = 1;
                } else if (geometry[row][col] == 1 && state[row-1][col] == 1) {
                    numSplits++;
                    if (geometry[row][col-1] == 0) {state[row][col-1] = 1;}
                    if (geometry[row][col+1] == 0) {state[row][col+1] = 1;}
                }
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << numSplits << '\n';
    cout << "#####################\n";
    return 0;
}
