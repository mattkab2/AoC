#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int headx=0; int heady=0;
    int tailx=0; int taily=0;
    char dir; int steps; int distx; int disty;
    map<pair<int,int>,bool> counter;
    counter[{0,0}]=true;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> dir >> steps;
        while (steps > 0) {
            switch (dir) {
                case 'L':
                    headx--;
                    break;
                case 'R':
                    headx++;
                    break;
                case 'U':
                    heady++;
                    break;
                case 'D':
                    heady--;
                    break;
            }
            distx = abs(headx-tailx);
            disty = abs(heady-taily);
            if (distx==2) {
                tailx += (headx-tailx)/distx;
                if (disty==1) {taily += (heady-taily)/disty;}
            }
            if (disty==2) {
                taily += (heady-taily)/disty;
                if (distx==1) {tailx += (headx-tailx)/distx;}
            }
            if (distx==2 or disty==2) {
                counter[pair<int,int>(tailx,taily)] = true;
                //cout << tailx << "," << taily << "\n";
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
