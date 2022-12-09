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
    vector<vector<bool>> visible;
    while (getline(input, line)) {
        heights.push_back(vector<int>(line.length()));
        visible.push_back(vector<bool>(line.length()));
        for (int i=0; i<line.length(); i++){
            heights[heights.size()-1][i] = line[i]-'0';
            visible[heights.size()-1][i] = false;
        }
    }

    int tallest;
    for (int i=0; i<heights.size(); i++) {
        tallest=-1;
        for (int j=0; j<heights[0].size(); j++) {
            if (heights[i][j] > tallest) {
                visible[i][j] = true;
                tallest = heights[i][j];
            }
        }
    }
    for (int j=0; j<heights[0].size(); j++) {
        tallest=-1;
        for (int i=0; i<heights.size(); i++) {
            if (heights[i][j] > tallest) {
                visible[i][j] = true;
                tallest = heights[i][j];
            }
        }
    }
    for (int i=0; i<heights.size(); i++) {
        tallest=-1;
        for (int j=heights[0].size()-1; j>=0; j--) {
            if (heights[i][j] > tallest) {
                visible[i][j] = true;
                tallest = heights[i][j];
            }
        }
    }
    for (int j=0; j<heights[0].size(); j++) {
        tallest=-1;
        for (int i=heights.size()-1; i>=0; i--) {
            if (heights[i][j] > tallest) {
                visible[i][j] = true;
                tallest = heights[i][j];
            }
        }
    }
    int ret=0;
    for (int i=0; i<heights.size(); i++) {
        for (int j=0; j<heights[0].size(); j++) {
            if (visible[i][j]) {ret++;}
        }
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
