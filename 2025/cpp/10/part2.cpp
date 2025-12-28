#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Spec {
    int state;
    std::vector<std::vector<int>> operations;
    std::vector<int> joltage;
};

std::vector<std::set<size_t>> identify_matches(const Spec& machine, const std::vector<int> &pattern, std::vector<int> state, std::set<size_t> path, size_t start) {
    if (start == machine.operations.size()) {
        bool match = true;
        for (size_t i = 0; i < state.size(); i++) {
            match = match && (state[i]%2 == pattern[i]%2);
        }
        if (match) {return {path};}
        return {};
    }
    // Component from skipping current pattern
    std::vector<std::set<size_t>> ret = identify_matches(machine, pattern, state, path, start+1);
    // Apply current step
    path.insert(start);
    for (int ind : machine.operations.at(start)) {
        state[ind]++;
    }
    auto tmp = identify_matches(machine, pattern, state, path, start+1);
    ret.insert(ret.end(), tmp.begin(), tmp.end());
    return ret;
}

const long NO_MATCH = 99999999999;
long parity_recursion(const Spec &machine, const std::vector<int> &pattern, std::map<std::vector<bool>, std::vector<std::set<size_t>>> &pressCache) {
    if (std::all_of(pattern.begin(), pattern.end(), [](int i){return i == 0;})) {
        return 0;
    }
    std::vector<bool> key(pattern.size(), false);
    std::transform(pattern.begin(), pattern.end(), key.begin(), [](int i){return i%2 == 1;});
    std::vector<std::set<size_t>> validPresses;
    if (pressCache.contains(key)) {validPresses = pressCache.at(key);}
    else {
        validPresses = identify_matches(machine, pattern, std::vector<int>(pattern.size(), 0), {}, 0);
        pressCache[key] = validPresses;
    }
    if (validPresses.empty()) {
        return NO_MATCH;
    }
    // For each valid sequence of presses, recurse on (half) the even parity part
    long ret = NO_MATCH;
    for (const auto &set : validPresses) {
        std::vector<int> newPattern = pattern;
        for (const auto &press : set) {
            for (const auto &ind : machine.operations.at(press)) {
                newPattern[ind]--;
            }
        }
        // If any of the newPattern < 0, continue
        if (std::any_of(newPattern.begin(), newPattern.end(), [](int i){return i < 0;})) {continue;}
        for (int & i : newPattern) {
            i = i/2;
        }
        ret = std::min(ret, static_cast<long>(set.size()) + 2*parity_recursion(machine, newPattern, pressCache));
    }
    return ret;
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    std::vector<Spec> machines;

    while (getline(input, line)) {
        if (line.front() == '!') {continue;}
        std::istringstream in(line);
        std::string chunk;
        while (getline(in, chunk, ' ')) {
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
        // Identify the odd-parity possibilities
        std::vector<bool> pattern(machine.joltage.size());
        std::map<std::vector<bool>, std::vector<std::set<size_t>>> cache;
        auto tmp = parity_recursion(machine, machine.joltage, cache);
        std::cout << "FINISHED MACHINE" << std::endl;
        ret += tmp;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
