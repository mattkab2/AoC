#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>

using namespace std;
void print(vector<vector<bool>> cave, vector<vector<bool>> initial) {
    for (int i=0; i<cave.size(); i++) {
        cout << i;
        for (int j=0; j<cave[0].size(); j++) {
            if (cave[i][j] and not initial[i][j]) {cout << 'o';}
            else if (cave[i][j] and initial[i][j]) {cout << '#';}
            else {cout << '.';}
        }
        cout << '\n';
    }
    return;
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;

    vector<list<pair<int,int>>> walls;
    int tmpx, tmpy;
    int minx=500;
    int maxy=0;
    int maxx=500;
    while (getline(input, line)) {
        walls.push_back(list<pair<int,int>>());
        istringstream iss(line);
        while (iss >> tmpx >> tmpy){
            minx = min(minx,tmpx);
            maxx = max(maxx,tmpx);
            maxy = max(maxy,tmpy);
            walls[walls.size()-1].emplace_back(pair<int,int>(tmpx,tmpy));
        }
    }
    cout << minx <<" "<< maxx <<" "<< maxy;
    maxx++;maxy+=3;
    minx = 500 - maxy;
    maxx = 500 + maxy + 1;
    walls.push_back(list<pair<int,int>>());
    walls[walls.size()-1].emplace_back(pair<int,int>(minx,maxy-1));
    walls[walls.size()-1].emplace_back(pair<int,int>(maxx-1,maxy-1));
    input.close();

    vector<vector<bool>> is_blocked(maxy);
    for (int i=0; i<maxy; i++) {
        is_blocked[i] = vector<bool>(maxx-minx);
        for (int j=0; j<maxx-minx; j++) {is_blocked[i][j]=false;}
    }
    int step;
    for (auto ll : walls) {
        tmpx = ll.front().first;
        tmpy = ll.front().second;
        is_blocked[tmpy][tmpx-minx] = true;
        for (auto pt : ll) {
            //if (pt==ll.front()) {continue;}
            if (tmpy==pt.second) {
                step = (pt.first-tmpx)/abs(pt.first-tmpx);
                while (tmpx != pt.first) {
                    tmpx+=step;
                    is_blocked[tmpy][tmpx-minx] = true;
                }
            } else if (tmpx==pt.first) {
                step = (pt.second-tmpy)/abs(pt.second-tmpy);
                while (tmpy != pt.second) {
                    tmpy+=step;
                    is_blocked[tmpy][tmpx-minx] = true;
                }
            }
        }
    }
    vector<vector<bool>> cave_initial = vector<vector<bool>>(is_blocked);

    // Begin simulating
    int moves[3][2] = {{0,1},{-1,1},{1,1}};
    int sand_count = 0;
    bool simulating = true;
    int settled = false;
    while (simulating) {
        tmpx=500-minx; tmpy=0;
        settled = false;
        while (not settled) {
            settled = true;
            for (int i=0; i<3; i++) {
                if (tmpx+moves[i][0] < 0 or tmpx+moves[i][0] >= maxx-minx) {
                    break;
                }
                if (not is_blocked[tmpy+moves[i][1]][tmpx+moves[i][0]]) {
                    settled = false;
                    tmpx += moves[i][0];
                    tmpy += moves[i][1];
                    break;
                }
            }
            if (simulating and settled) {
                is_blocked[tmpy][tmpx] = true;
                sand_count++;
                if (tmpy==0 and tmpx==500-minx) {simulating = false;}
            }
        }
    }
    print(is_blocked,cave_initial);

    cout << "#####################\n";
    cout << sand_count << '\n';
    cout << "#####################\n";
    return 0;
}
