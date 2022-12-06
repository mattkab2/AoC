#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

void print_state(const std::vector<std::stack<char>> state) {
    auto disp_stack = std::vector<std::stack<char>>(state);
    std::string line;

    for (int i=0; i<state.size(); i++) {
        line = "";
        while(disp_stack[i].size() > 0) {
            line.insert(0,1,disp_stack[i].top());
            disp_stack[i].pop();
        }
        std::cout << line << '\n';
    }
    std::cout << '\n';
}

std::string read_top(const std::vector<std::stack<char>> state) {
    std::string ret = "";
    for (int i=0; i<state.size(); i++) {
        ret.push_back(state[i].top());
    }
    return ret;
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<string> state_init;
    while (getline(input, line) and line[1]!='1') {
        state_init.push_back(line);
    }
    int sz = (line.length()+1)/4;
    vector<stack<char>> state(sz);
    
    for (int i=state_init.size()-1; i>=0; i--) {
        for (int j=0; j<sz; j++) {
            if (state_init[i][j*4+1]!=' ') {
                state[j].push(state_init[i][j*4+1]);
            }
        }
    }
    print_state(state);

    int n,src,dest;
    stack<char> held;
    while (getline(input, line)) {
        if (line=="") {continue;}
        istringstream iss(line);
        iss >> n >> src >> dest;
        for (int i=0; i<n; i++) {
            held.push(state[src-1].top());
            state[src-1].pop();
        }
        for (int i=0; i<n; i++) {
            state[dest-1].push(held.top());
            held.pop();
        }
    }
    print_state(state);

    string ret = read_top(state);

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
