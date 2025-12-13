#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int acc = 50;
    int tmp = 0;
    int ret = 0;
    while (getline(input, line)) {
        istringstream iss(line.substr(1, line.size()-1));
        iss >> tmp;
        if (line.data()[0] == 'L') {
            tmp = -tmp;
        }
        acc = (acc + tmp) % 100;
        if (acc < 0) {
            acc += 100;
        }
        if (acc == 0) {
            ret++;
        }
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
