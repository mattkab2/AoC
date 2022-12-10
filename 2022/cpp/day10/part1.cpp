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

    vector<int> cycle_vals = {20, 60, 100, 140, 180, 220};
    //vector<int> cycle_vals = {6};
    int measure_idx = 0;
    int current_cycle = 1;
    int reg_value = 1;
    int ret = 0;

    ifstream input;
    input.open(argv[1], ios::in);
    string line,instr;
    int val;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> instr;
        if (arg_count[instr] == 1) {iss >> val;}

        current_cycle += durations[instr];
        if (current_cycle > cycle_vals[measure_idx]) {
            ret += cycle_vals[measure_idx]*reg_value;
            measure_idx++;
        }
        if (instr.compare("addx")==0) {reg_value += val;}
        if (measure_idx >= cycle_vals.size()) {break;}
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
