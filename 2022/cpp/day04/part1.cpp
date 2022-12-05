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
    int lo1,lo2,hi1,hi2;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> lo1 >> hi1 >> lo2 >> hi2;
        if (lo1 >= lo2 and hi1 <= hi2) {
            ret++;
        } else if (lo2 >= lo1 and hi2 <= hi1) {
            ret++;
        }
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
