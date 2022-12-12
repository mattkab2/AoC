#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <functional>
#include <numeric>
#include <algorithm>

using namespace std;
class Monkey {
public:
    inline static long modular_limit=0;
    queue<long> items;
    int test_div, test_true, test_false;
    function<long(long)> operation;

    Monkey(vector<int> items, function<long(long)> operation, int _test_div, int _test_true, int _test_false)
    : test_div(_test_div), test_true(_test_true), test_false(_test_false) {
        this->operation = operation;
        for (int i=0; i<items.size(); i++) {
            this->items.push(long(items[i]));
        }
    }

    long test(long &item) {
        item = operation(item)%modular_limit;
        return (item%test_div==0) ? test_true : test_false;
    }
};

using namespace std;
int main(int argc, char *argv[]) {
    vector<Monkey> monkeys;

    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int tmp;
    vector<int> inventory;
    istringstream iss;
    while (getline(input, line)) {
        if (line.length()==0) {continue;}
        if (line.rfind("Monkey", 0) == 0) {
            // Read inventory
            getline(input,line);
            iss = istringstream(line);
            inventory.clear();
            while(iss >> tmp) {inventory.push_back(tmp);}

            // Read function
            getline(input,line);
            string arg1,op_s,arg2;
            int a1, a2;
            iss = istringstream(line);
            iss >> arg1 >> op_s >> arg2;
            if (arg1.compare("old")==0) {
                a1 = 9999;
            } else {a1 = stoi(arg1);}
            if (arg2.compare("old")==0) {
                a2 = 9999;
            } else {a2 = stoi(arg2);}

            char op = op_s[0];
            function<long(long)> func;
            switch(op) {
                case '+':
                    func = [a1, a2](long x) {return ((a1==9999) ? x : a1) + ((a2==9999) ? x : a2);};
                    break;
                case '*':
                    func = [a1, a2](long x) {return ((a1==9999) ? x : a1) * ((a2==9999) ? x : a2);};
                    break;
                default:
                    func = [a1, a2](long x) {return -1l;};
                    break;
            }

            // Read other vars
            int test_div,test_true,test_false;
            getline(input,line);
            test_div=stoi(line);
            getline(input,line);
            test_true=stoi(line);
            getline(input,line);
            test_false=stoi(line);

            // Create monkey
            monkeys.push_back(Monkey(inventory, func, test_div, test_true, test_false));
        }
    }
    // compute lcm of monkeys
    int modular_limit = 1;
    for (int i=0; i<monkeys.size(); i++) {
        modular_limit = lcm(modular_limit,monkeys[i].test_div);
    }
    Monkey::modular_limit = long(modular_limit);

    vector<long> inspect(monkeys.size());
    int monkey_dest;
    long item;
    for (int i=0; i<monkeys.size();i++) {inspect[i]=0;}
    for (int round=0; round<10000; round++) {
        for (int i=0; i<monkeys.size(); i++) {
            inspect[i] += monkeys[i].items.size();
            while (monkeys[i].items.size() > 0) {
                item = monkeys[i].items.front();
                monkey_dest = monkeys[i].test(item);
                if (item < 0) {return 1;}
                monkeys[monkey_dest].items.push(item);
                monkeys[i].items.pop();
            }
        }
    }
    for (int i=0; i<monkeys.size(); i++) {cout << inspect[i] << " ";}
    cout << '\n';
    sort(inspect.begin(), inspect.end(), greater<int>());

    input.close();
    cout << "#####################\n";
    cout << inspect[0]*inspect[1] << '\n';
    cout << "#####################\n";
    return 0;
}
