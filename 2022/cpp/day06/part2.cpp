#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool check_unique(const std::string message, int start, int end) {
    if (start+2 == end) {return !(message[start]==message[end]);}

    for (int i=start+1; i<end; i++) {
        if (message[start]==message[i]) {return false;}
    }
    return check_unique(message, start+1, end);
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    cout << "#####################\n";
    while (getline(input, line)) {
        for (int i=0; i<line.length()-13; i++) {
            if (check_unique(line,i,i+14)) {
                cout << i+14 << '\n';
                break;
            }
        }
    }

    input.close();
    cout << "#####################\n";
    return 0;
}
