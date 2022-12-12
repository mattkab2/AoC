#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>


using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<vector<int>> height_map;
    int iend,jend;

    while (getline(input, line)) {
        height_map.push_back(vector<int>(line.length()));
        for (int i=0; i<line.length(); i++) {
            switch(line[i]) {
                case 'S':
                    height_map[height_map.size()-1][i] = 0;
                    break;
                case 'E':
                    iend = height_map.size()-1;
                    jend = i;
                    height_map[height_map.size()-1][i] = 'z'-'a';
                    break;
                default:
                    height_map[height_map.size()-1][i] = line[i]-'a';
            }
        }
    }
    input.close();

    for (int i=0;i<height_map.size();i++) {
        for (int j=0;j<height_map[0].size();j++) {
            cout << char(height_map[i][j]+'a');
        }
        cout << '\n';
    }

    vector<vector<bool>> visited(height_map.size());
    vector<vector<pair<int,int>>> path_storage(height_map.size());
    for (int i=0; i<height_map.size(); i++) {
        visited[i] = vector<bool>(height_map[0].size());
        path_storage[i] = vector<pair<int,int>>(height_map[0].size());
        for (int j=0;j<height_map[0].size();j++) {
            visited[i][j] = false;
        }
    }
    
    queue<pair<int,int>> to_visit;
    int moves[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
    to_visit.push(pair<int,int>(iend,jend));
    visited[iend][jend]=true;
    cout << "Searching for elev" << 0 << '\n';
    cout << "  starting from " << iend << ',' << jend << '\n';
    int ci,cj;
    while (to_visit.size()>0) {
        ci = to_visit.front().first;
        cj = to_visit.front().second;
        to_visit.pop();
        //cout << "Examining point: " << ci << ',' << cj << '\n';
        if (height_map[ci][cj]==0) {cout << "Arrived\n"; break;}
        for (int i=0; i<4; i++) {
            int ni = ci+moves[i][0];
            int nj = cj+moves[i][1];
            if (ni<0 or nj<0 or ni>=height_map.size() or nj >= height_map[0].size()) {continue;}

            if ((not visited[ni][nj]) and
                (height_map[ni][nj] - height_map[ci][cj] >= -1)) {
                //cout << "Adding neighbor to queue: " << ni << ',' << nj << '\n';
                to_visit.push(pair<int,int>(ni,nj));
                path_storage[ni][nj] = pair<int,int>(ci,cj);
                visited[ni][nj] = true;
            }
        }
    }

    int i,j,ret=0;
    while (ci!=iend or cj!=jend) {
        i = path_storage[ci][cj].first;
        j = path_storage[ci][cj].second;
        cout << ci << "," << cj << "<-" << i << ',' << j << '\n';
        ret++;
        ci=i; cj=j;
    }

    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
