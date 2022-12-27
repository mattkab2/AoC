#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <list>
#include <map>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    string name;
    string tmp,op,tmp2;
    list<array<string,4>> to_do;
    map<string,long> monkey_values;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> name;
        iss >> tmp;
        if (iss.eof()) {
            monkey_values[name] = stoi(tmp);
            continue;
        }
        iss >> op;
        iss >> tmp2;
        to_do.emplace_back(array<string,4>({name,tmp,op,tmp2}));
    }
    while (monkey_values.count("root")==0) {
        for (auto m=to_do.begin(); m!=to_do.end(); m++) {
            if (monkey_values.count((*m)[0])==1) {continue;} 
            if ((monkey_values.count((*m)[1])==1) and (monkey_values.count((*m)[3])==1)) {
                switch((*m)[2][0]) {
                case '+':
                    monkey_values[(*m)[0]] = monkey_values[(*m)[1]]+monkey_values[(*m)[3]];
                    break;
                case '-':
                    monkey_values[(*m)[0]] = monkey_values[(*m)[1]]-monkey_values[(*m)[3]];
                    break;
                case '*':
                    monkey_values[(*m)[0]] = monkey_values[(*m)[1]]*monkey_values[(*m)[3]];
                    break;
                case '/':
                    monkey_values[(*m)[0]] = monkey_values[(*m)[1]]/monkey_values[(*m)[3]];
                    break;
                }
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << monkey_values["root"] << '\n';
    cout << "#####################\n";
    return 0;
}
