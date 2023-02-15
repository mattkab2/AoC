#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    list<pair<int>> elves;

    int i = 0;
    while (getline(input, line)) {
        for (int j=0; j<line.length(); j++) {
            elves.emplace_back(pair<int,int>(i,j));
        }
        i++;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
