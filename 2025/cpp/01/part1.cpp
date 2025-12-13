#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int ret = 0;
    int acc = 0;
    int tmp = 0;
    while (getline(input, line)) {
        if (line=="") {
            ret = (acc > ret) ? acc : ret;
            acc = 0;
            continue;
        }
        istringstream iss(line);
        iss >> tmp;
        acc += tmp;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
