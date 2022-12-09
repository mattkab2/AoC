#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<int> ropex(10),ropey(10);
    for (int i=0; i<ropex.size(); i++) {
        ropex[i]=0;
        ropey[i]=0;
    }
    char dir; int steps; int distx; int disty;
    map<pair<int,int>,bool> counter;
    counter[{0,0}]=true;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> dir >> steps;
        while (steps > 0) {
            switch (dir) {
                case 'L':
                    ropex[0]--;
                    break;
                case 'R':
                    ropex[0]++;
                    break;
                case 'U':
                    ropey[0]++;
                    break;
                case 'D':
                    ropey[0]--;
                    break;
            }
            for (int i=1; i<ropex.size(); i++) {
                distx = abs(ropex[i-1]-ropex[i]);
                disty = abs(ropey[i-1]-ropey[i]);
                if (distx==2) {
                    ropex[i] += (ropex[i-1]-ropex[i])/distx;
                    if (disty==1) {ropey[i] += (ropey[i-1]-ropey[i])/disty;}
                }
                if (disty==2) {
                    ropey[i] += (ropey[i-1]-ropey[i])/disty;
                    if (distx==1) {ropex[i] += (ropex[i-1]-ropex[i])/distx;}
                }
                if (i==ropex.size()-1 and (distx==2 or disty==2)) {
                    counter[pair<int,int>(ropex[i],ropey[i])] = true;
                    cout << ropex[i] << "," << ropey[i] << "\n";
                }
            }
            steps--;
        }
    }

    input.close();
    cout << "#####################\n";
    cout << counter.size() << '\n';
    cout << "#####################\n";
    return 0;
}
