#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

class Monkey {
public:

    string name;
    bool isScalar;
    bool isEval;
    long val;
    char op;

    Monkey* parent;
    Monkey* child1;
    Monkey* child2;


    Monkey(string name_, int val_)
    : name(name_), isScalar(true), isEval(false), val(val_), op(' '), parent(NULL), child1(NULL), child2(NULL) {}

    Monkey(string name_, char op_)
    : name(name_), isScalar(false), isEval(false), val(-1), op(op_), parent(NULL), child1(NULL), child2(NULL) {}

    void set_children(Monkey* first, Monkey* second) {
        child1 = first; child2 = second;
        first->parent = this;
        second->parent = this;
    }

    void eval() {
        if (isScalar) {
            if (name.compare("humn")==0) {return;}
            isEval = true;
            return;
        }

        child1->eval();
        child2->eval();
        if (child1->isEval and child2->isEval) {
            isEval = true;
            switch(op) {
            case '+':
                val = child1->val + child2->val;
                break;
            case '-':
                val = child1->val - child2->val;
                break;
            case '*':
                val = child1->val * child2->val;
                break;
            case '/':
                val = child1->val / child2->val;
                break;
            }
        }
    }

    long solve(long target) {
        if (name.compare("humn")==0) {return target;}
        if (child1->isEval) {
            switch(op) {
            case '+':
                return child2->solve(target-child1->val); break;
            case '-':
                return child2->solve(child1->val-target); break;
            case '*':
                return child2->solve(target/child1->val); break;
            case '/':
                return child2->solve(child1->val/target); break;
            default:
                return -1;
            }
        } else {
            switch (op) {
            case '+':
                return child1->solve(target-child2->val); break;
            case '-':
                return child1->solve(target+child2->val); break;
            case '*':
                return child1->solve(target/child2->val); break;
            case '/':
                return child1->solve(target*child2->val); break;
            default:
                return -1;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    string name,tmp,tmp2,op;
    vector<Monkey> herd;
    vector<pair<string,string>> child_data;
    Monkey* root = NULL;
    Monkey* humn = NULL;

    while (getline(input, line)) {
        istringstream iss(line);
        iss >> name;
        iss >> tmp;
        if (iss.eof()) {
            herd.push_back(Monkey(name,stoi(tmp)));
            child_data.push_back(pair<string,string>("",""));
            continue;
        }
        iss >> op;
        iss >> tmp2;
        herd.push_back(Monkey(name,op[0]));
        child_data.push_back(pair<string,string>(tmp,tmp2));
    }
    input.close();

    for (size_t i=0; i<herd.size(); i++) {
        Monkey* first=NULL;
        Monkey* second=NULL;
        if (herd[i].isScalar) {continue;}
        for (size_t j=0; j<herd.size(); j++) {
            if (herd[j].name.compare(child_data[i].first)==0) {first = &herd[j];}
            if (herd[j].name.compare(child_data[i].second)==0) {second = &herd[j];}
        }
        herd[i].set_children(first,second);
        if (herd[i].name.compare("root")==0) {
            root = &herd[i];
            root->op = '-';
        } else if (name.compare("humn")==0) {
            humn = &herd[i];
        }
    }
    root->eval();

    cout << "#####################\n";
    cout << root->solve(0) << '\n';
    cout << "#####################\n";
    return 0;
}
