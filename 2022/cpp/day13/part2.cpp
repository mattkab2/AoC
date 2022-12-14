#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
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
    vector<Packet> transmission;
    while (getline(input, line)) {
        if (line=="") {continue;}
        if (line=="STOP") {return 0;}
        transmission.push_back(Packet(line));
    }
    transmission.push_back(Packet("[[2]]"));
    transmission.push_back(Packet("[[6]]"));

    auto tStart = transmission.begin();
    auto tEnd = transmission.end();
    sort(tStart, tEnd);

    for (int i=0; i<transmission.size(); i++) {
        transmission[i].print();cout <<'\n';
    }

    int div1 = find(tStart,tEnd,Packet("[[2]]"))-tStart+1;
    int div2 = find(tStart,tEnd,Packet("[[6]]"))-tStart+1;

    input.close();
    cout << "#####################\n";
    cout << div1*div2 << '\n';
    cout << "#####################\n";
    return 0;
}
