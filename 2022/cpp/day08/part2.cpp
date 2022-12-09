#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    vector<vector<int>> heights;
    while (getline(input, line)) {
        heights.push_back(vector<int>(line.length()));
        for (int i=0; i<line.length(); i++){
            heights[heights.size()-1][i] = line[i]-'0';
        }
    }

    int ret=0;
    int ii,jj,tmp,tmp_score;
    for (int i=1; i<heights.size()-1; i++) {
        for (int j=1; j<heights[0].size()-1; j++) {
            tmp_score=1;
            tmp = 0;
            ii=i; jj=j;
            do {
                ii--;
                tmp++;
            } while(ii>=1 and heights[ii][jj] < heights[i][j]);

            tmp_score = tmp_score*tmp;
            tmp = 0;
            ii=i; jj=j;
            do {
                ii++;
                tmp++;
            } while (ii<heights.size()-1 and heights[ii][jj] < heights[i][j]);

            tmp_score = tmp_score*tmp;
            tmp = 0;
            ii=i; jj=j;
            do {
                jj--;
                tmp++;
            } while(jj>=1 and heights[ii][jj] < heights[i][j]);

            tmp_score = tmp_score*tmp;
            tmp = 0;
            ii=i; jj=j;
            do {
                jj++;
                tmp++;
            } while(jj<heights.size()-1 and heights[ii][jj] < heights[i][j]);
            tmp_score = tmp_score*tmp;

            if (tmp_score > ret) {ret = tmp_score;}
        }
    }
    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
