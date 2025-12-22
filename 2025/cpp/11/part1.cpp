#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

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

    for (const auto &[key, arr] : connectivity) {
        std::cout << key << ": ";
        for (const auto &dest : arr) {
            std::cout << dest << " ";
        }
        std::cout << std::endl;
    }

    // DFS search from 'you' to 'out'
    long ret = 0;
    //std::unordered_set<std::string> visited{"you"};
    std::stack<std::string> stack{{"you"}};
    while (!stack.empty()) {
        std::string curr = stack.top();
        stack.pop();
        //visited.insert(curr);
        if (connectivity.contains(curr)) {
            for (const auto &d : connectivity[curr]) {
                if (d.compare("out") == 0) {ret++;}
                else {
                    stack.push(d);}
            }
        }
    }


    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
