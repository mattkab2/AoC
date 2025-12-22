#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <set>
#include <vector>

using namespace std;

long count_paths(const std::map<std::string, std::vector<std::string>> &connectivity, std::map<std::string, long> &cache, const std::string &curr, const std::string &dest) {
    if (curr == dest) {return 1;}
    if (cache.contains(curr)) {
        return cache[curr];
    }
    if (!connectivity.contains(curr)) {return 0;}
    long ret = 0;
    for (const auto &d : connectivity.at(curr)) {
        ret += count_paths(connectivity, cache, d, dest);
    }
    cache[curr] = ret;
    return ret;
}

long count_total(const std::map<std::string, std::vector<std::string>> &connectivity, const std::set<std::string> &visited, const std::string &start, const std::string &dest) {
    long ret = 0;
    std::stack<std::string> stack{{start}};
    while (!stack.empty()) {
        std::string curr = stack.top();
        stack.pop();
        for (const auto &d : connectivity.at(curr)) {
            if (d == dest) {ret++;}
            else {stack.push(d);}
        }
    }
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    std::map<std::string, std::vector<string>> connectivity;

    while (getline(input, line)) {
        std::istringstream in(line);
        std::string key;
        std::string chunk;
        while (getline(in, chunk, ' ')) {
            if (key.empty()) {
                key = chunk;
            } else {
                connectivity[key].push_back(chunk);
            }
        }
    }

    std::map<std::string, long> cache;
    // Count svr-dac-fft-out
    long path1 = count_paths(connectivity, cache, "svr", "dac");
    cache.clear();
    long path2 = count_paths(connectivity, cache, "dac", "fft");
    cache.clear();
    long path3 = count_paths(connectivity, cache, "fft", "out");
    cache.clear();
    long ret = path1*path2*path3;

    // Count svr-dac-fft-out
    path1 = count_paths(connectivity, cache, "svr", "fft");
    cache.clear();
    path2 = count_paths(connectivity, cache, "fft", "dac");
    cache.clear();
    // From here, all paths lead to out
    path3 = count_paths(connectivity, cache, "dac", "out");
    cache.clear();
    ret += path1*path2*path3;

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
