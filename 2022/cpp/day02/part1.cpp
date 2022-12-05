#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream input;
    unordered_map<char,int> read_move;
    read_move['A'] = 0;
    read_move['B'] = 1;
    read_move['C'] = 2;
    read_move['X'] = 0;
    read_move['Y'] = 1;
    read_move['Z'] = 2;
    int lookup[3][3] = {{4,8,3},{1,5,9},{7,2,6}};

    input.open(argv[1], ios::in);
    string line;
    char challenge,response;
    int ret = 0;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> challenge >> response;
        ret += lookup[read_move[challenge]][read_move[response]];
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
