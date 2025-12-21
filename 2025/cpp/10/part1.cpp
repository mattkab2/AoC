#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Spec {
    int state;
    std::vector<int> operations;
    std::vector<int> joltage;
};

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    std::vector<Spec> machines;

    while (getline(input, line)) {
        std::istringstream in(line);
        std::string chunk;
        while (getline(in, chunk, ' ')) {
            std::cout << chunk << std::endl;
            if (chunk.front() == '[') {
                chunk = std::string(chunk.begin()+1, chunk.end()-1);
                machines.emplace_back();
                int state = 0;
                int pow = 0;
                for (char c : chunk) {
                    if (c == '#') {
                        state += (1 << pow);
                    }
                    pow++;
                }
                machines.back().state = state;
            } else if (chunk.front() == '(') {
                int op = 0;
                chunk = std::string(chunk.begin()+1, chunk.end()-1);
                std::stringstream ss(chunk);
                while (ss.good()) {
                    std::string substr;
                    getline(ss, substr, ',');
                    op += (1 << std::stoi(substr));
                }
                machines.back().operations.push_back(op);
            } else if (chunk.front() == '{') {
                chunk = std::string(chunk.begin()+1, chunk.end()-1);
                std::stringstream ss(chunk);
                while (ss.good()) {
                    std::string substr;
                    getline(ss, substr, ',');
                    machines.back().joltage.push_back(std::stoi(substr));
                }
            }
        }
    }

    for (const auto &machine : machines) {
        std::cout << "############################" << std::endl;
        std::cout << "Final setting: " << machine.state;
        std::cout << std::endl;
        std::cout << "Allowed Ops: " << std::endl;
        for (const auto &op : machine.operations) {
            std::cout << "  " << op << std::endl;
        }
    }

    long ret = 0;
    for (const auto &machine : machines) {
        // Begin a breadth-first search through space to zero from final state
        std::unordered_set<int> visited;
        std::map<int, int> dist;
        visited.insert(machine.state);
        dist[machine.state] = 0;
        queue<int> q{{machine.state}};
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (const auto &op : machine.operations) {
                int dest = curr ^ op;
                if (!visited.contains(dest)) {
                    dist[dest] = dist[curr]+1;
                    visited.insert(dest);
                    q.push(dest);
                }
            }
            if (dist.contains(0)) {
                break;
            }
        }
        ret += dist[0];
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
