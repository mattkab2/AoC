#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {

    map<string,int> durations,arg_count;
    durations["noop"] = 1;
    durations["addx"] = 2;
    arg_count["noop"] = 0;
    arg_count["addx"] = 1;

    int current_cycle = 0;
    int reg_value = 1;

    ifstream input;
    input.open(argv[1], ios::in);
    string line,instr;
    int val = 0;
    string crt = "";
    int pending = 0;

    while (pending >= 0) {
        // If pending==0 fetch the next instruction
        if (pending==0) {
            reg_value += val;
            val = 0;
            if (not getline(input, line)) {
                cout << "wat";
                pending--;
                continue;
            }
            istringstream iss(line);
            iss >> instr;
            if (arg_count[instr] == 1) {iss >> val;}
            pending = durations[instr];
        }
        // Draw character
        if (abs(reg_value - (current_cycle % 40)) <= 1) {
            crt.append("#");
        } else {
            crt.append(".");
        }
        if (crt.length()==40) {
            cout << crt << '\n';
            crt = "";
        }
        current_cycle++;
        pending--;
    }
    input.close();
    return 0;
}
