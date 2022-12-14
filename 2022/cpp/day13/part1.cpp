#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
class Packet {
public:
    int val;
    bool scalar;
    vector<Packet> children;

    Packet() {
        val = -1;
        scalar=false;
        children = vector<Packet>(0);
    }
    Packet(int n) {
        val = n;
        scalar=true;
        children = vector<Packet>(0);
    }
    Packet(string line) {
        scalar = false;
        val = -1;
        if (line=="[]") {
            children = vector<Packet>(0);
            return;
        }
        int hi;
        //cout << line << "\n";
        for (int i=1; i<line.length()-1; i++) {
            //cout << "ON CHARACTER " << i << "/" << line[i] << '\n';
            switch(line[i]) {
                case '[':
                    hi=line.find(']', i);
                    //cout << "A" << i << "-" << hi << " " << line.substr(i,hi-i+1) << '\n';
                    children.push_back(Packet(line.substr(i,hi-i+1)));
                    i = hi;
                    break;
                case ']':
                    break;
                case ',':
                    break;
                default:
                    hi=line.find_first_of(",]", i);
                    //cout << "B" << i << "-" << hi << " " << line.substr(i,hi-i) << '\n';
                    children.push_back(Packet(stoi(line.substr(i,hi-i))));
                    i = hi;
            }
        }
    }

    Packet promote() const {
        Packet ret = Packet();
        ret.children.push_back(Packet(val));
        return ret;
    }

    void print() const {
        if (scalar) {cout << val; return;}

        cout << "[";
        for (int i=0; i<children.size(); i++) {
            children[i].print();
            if (i<children.size()-1) {cout << ",";}
        }
        cout << "]";
        return;
    }

    bool operator==(const Packet& p) {
        if (scalar and p.scalar) {return val==p.val;}
        if ((not scalar) and p.scalar) {return (*this)==p.promote();}
        if (scalar and (not p.scalar)) {return this->promote()==p;}
        if (children.size()!=p.children.size()) {return false;}
        bool ret = true;
        for (int i=0; i<children.size(); i++) {ret = ret and (children[i]==p.children[i]);}
        return ret;
    }
    bool operator!=(const Packet& p) {return not ((*this)==p);}

    bool operator<(const Packet& p) {
        if (scalar and p.scalar) {return val<p.val;}
        if ((not scalar) and p.scalar) {return (*this)<p.promote();}
        if (scalar and (not p.scalar)) {return this->promote()<p;}
        for (int i=0; i<max(children.size(),p.children.size()); i++) {
            if (i >= children.size()) {return true;}
            if (i >= p.children.size()) {return false;}
            if (children[i]!=p.children[i]) {return children[i] < p.children[i];}
        }
        cout << "You done fucked up" << '\n';
        return false;
    }
};

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int pair_idx=1;
    int ret = 0;
    while (getline(input, line)) {
        if (line=="") {continue;}
        if (line=="STOP") {return 0;}
        Packet first = Packet(line);
        first.print(); cout << '\n';
        getline(input, line);
        Packet second = Packet(line);
        second.print(); cout << '\n';

        if (first<second) {ret += pair_idx;}
        pair_idx++;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
