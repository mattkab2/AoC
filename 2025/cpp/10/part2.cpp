#include <algorithm>
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
    std::vector<std::vector<int>> operations;
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
                machines.back().operations.emplace_back();
                chunk = std::string(chunk.begin()+1, chunk.end()-1);
                std::stringstream ss(chunk);
                while (ss.good()) {
                    std::string substr;
                    getline(ss, substr, ',');
                    machines.back().operations.back().push_back(std::stoi(substr));
                }
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

    long ret = 0;
    for (const auto &machine : machines) {
        // Begin a breadth-first search through space to zero from final state
        std::map<std::vector<int>, int> dist;
        dist[machine.joltage] = 0;
        queue<std::vector<int>> q{{machine.joltage}};
        while (!q.empty()) {
            std::vector<int> curr = q.front();
            q.pop();
            for (const auto &op : machine.operations) {
                std::vector<int> dest(curr);
                for (int ind : op) {
                    dest[ind]--;
                }
                // If any are negative, continue
                if (std::any_of(dest.begin(), dest.end(),[](int i){return i < 0;})) {continue;}
                if (!dist.contains(dest)) {
                    dist[dest] = dist[curr]+1;
                    q.push(dest);
                }
            }
            if (dist.contains(std::vector<int>(machine.joltage.size(), 0))) {
                break;
            }
        }
        std::cout << dist[std::vector<int>(machine.joltage.size(), 0)] << std::endl;
        ret += dist[std::vector<int>(machine.joltage.size(), 0)];
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
