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
    cout << "#####################\n";
    while (getline(input, line)) {
        for (int i=0; i<line.length()-3; i++) {
            if (line[i]!=line[i+1] and line[i]!=line[i+2] and line[i]!=line[i+3]
              and line[i+1]!=line[i+2] and line[i+1]!=line[i+3] and line[i+2]!=line[i+3]) {
                cout << i+4 << '\n';
                break;
            }
        }
    }

    input.close();
    cout << "#####################\n";
    return 0;
}
