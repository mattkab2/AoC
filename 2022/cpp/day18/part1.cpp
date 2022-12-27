#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int count_recursive(const vector<vector<vector<bool>>> &occupied,
                    vector<vector<vector<bool>>> &processed,
                    size_t i, size_t j, size_t k) {
    
    if (i<0 or i>=occupied.size()) {return 1;}
    if (j<0 or j>=occupied[0].size()) {return 1;}
    if (k<0 or k>=occupied[0][0].size()) {return 1;}
    if (processed[i][j][k]) {
        return occupied[i][j][k] ? 0 : 1;
    }
    processed[i][j][k] = true;
    int ret=0;
    ret += count_recursive(occupied, processed, i-1, j, k);
    ret += count_recursive(occupied, processed, i+1, j, k);
    ret += count_recursive(occupied, processed, i, j-1, k);
    ret += count_recursive(occupied, processed, i, j+1, k);
    ret += count_recursive(occupied, processed, i, j, k-1);
    ret += count_recursive(occupied, processed, i, j, k+1);
    return ret;
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int cube_x,cube_y,cube_z;
    int sz_x=0;
    int sz_y=0;
    int sz_z=0;
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> cube_x >> cube_y >> cube_z;
        sz_x = max(sz_x,cube_x);
        sz_y = max(sz_y,cube_y);
        sz_z = max(sz_z,cube_z);
    }
    sz_x++; sz_y++; sz_z++;
    vector<vector<vector<bool>>> occupied(sz_x);
    for (int i=0; i<sz_x; i++) {
        occupied[i] = vector<vector<bool>>(sz_y);
        for (int j=0; j<sz_y; j++) {
            occupied[i][j] = vector<bool>(sz_z);
            for (int k=0; k<sz_z; k++) {
                occupied[i][j][k] = false;
            }
        }
    }
    input.clear();
    input.seekg(0);

    while (getline(input, line)) {
        istringstream iss(line);
        iss >> cube_x >> cube_y >> cube_z;
        occupied[cube_x][cube_y][cube_z] = true;
    }

    vector<vector<vector<bool>>> processed(occupied);
    for (int i=0; i<sz_x; i++) {
        for (int j=0; j<sz_y; j++) {
            for (int k=0; k<sz_z; k++) {
                processed[i][j][k] = not processed[i][j][k];
            }
        }
    }

    int ret = 0;
    for (int i=0; i<sz_x; i++) {
        for (int j=0; j<sz_y; j++) {
            for (int k=0; k<sz_z; k++) {
                if (not processed[i][j][k]) {ret += count_recursive(occupied, processed, i, j, k);}
            }
        }
    }
    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
