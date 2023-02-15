#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <list>

using namespace std;

int step[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};

bool inBnds(const vector<vector<int>> &maze, const array<size_t,2> &pos) {
    return not (pos[0]<0 or pos[0]>=maze.size()
           or pos[1]<0 or pos[1]>=maze[0].size()
           or maze[pos[0]][pos[1]]==2);
}
void wrap(const vector<vector<int>> &maze, array<size_t,2> &pos, const int facing) {
    do {
        pos[0] -= step[facing][0];
        pos[1] -= step[facing][1];
    } while (inBnds(maze, pos));
    pos[0] += step[facing][0];
    pos[1] += step[facing][1];
}

void print_maze(const vector<vector<int>> &maze) {
    for (int i=0; i<maze.size(); i++) {
        for (int j=0; j<maze[0].size(); j++) {
            switch (maze[i][j]) {
            case 0:
                cout << '.';
                break;
            case 1:
                cout << '#';
                break;
            case 2:
                cout << ' ';
                break;
            default:
                cout << '@';
                break;
            }
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    list<string> maze_list;
    while (getline(input, line)) {
        if (line.compare("")==0) {getline(input, line); break;}
        maze_list.emplace_back(line);
    }
    list<int> directions;
    string tmp;
    istringstream iss(line);
    while (iss >> tmp) {
        switch(tmp[0]) {
        case 'L':
            directions.emplace_back(-1);
            break;
        case 'R':
            directions.emplace_back(-2);
            break;
        default:
            directions.emplace_back(stoi(tmp));
            break;
        }
    }
    cout << "Directions: ";
    for (auto p : directions) {cout << p << " ";}
    cout << endl;

    size_t maze_horz = 0;
    for (auto row : maze_list) {
        cout << row << endl;
        maze_horz = max(maze_horz,row.length());
    }
    vector<vector<int>> maze(maze_list.size());
    auto row_it = maze_list.begin();
    for (size_t i=0; i<maze.size(); i++) {
        maze[i] = vector<int>(maze_horz);
        for (size_t j=0; j<maze_horz; j++) {
            if (j >= row_it->size()) {maze[i][j] = 2;}
            else {
                switch((*row_it)[j]) {
                case ' ':
                    maze[i][j] = 2;
                    break;
                case '.':
                    maze[i][j] = 0;
                    break;
                case '#':
                    maze[i][j] = 1;
                    break;
                default:
                    maze[i][j] = -1;
                }
            }
        }
        row_it++;
    }
    // Find starting position
    array<size_t,2> pos;
    for (size_t i=0; i<maze[0].size(); i++) {
        if (maze[0][i]==0) {pos = {0,i}; break;}
    }
    int facing = 1;
    cout << "Starting position: " << pos[0] << "," << pos[1] << endl;

    print_maze(maze);

    for (auto instr : directions) {
        if (instr < 0) {
            cout << "Turning: ";
            if (instr==-1) {cout << "left "; facing = (facing-1)%4;}
            else if (instr==-2) {cout << "right " << endl; facing = (facing+1)%4;}
            cout << "at current position " << pos[0] << "," << pos[1] << endl;
            continue;
        }

        cout << "Moving from: " << pos[0] << "," << pos[1] << "(" << inBnds(maze,pos) << ")" << endl;
        int tmp = instr;
        cout << instr << " steps" << endl;
        while (tmp > 0) {
            if (not inBnds(maze, pos)) {
                wrap(maze,pos,facing);
            } else {
                pos[0] += step[facing][0];
                pos[1] += step[facing][1];
                if (not inBnds(maze,pos)) {cout << "WRAPPING" << endl; wrap(maze,pos,facing);}
            }
            if (maze[pos[0]][pos[1]] == 1) {
                cout << "WALL COLLISION " << pos[0] << "," << pos[1] << endl;
                pos[0] -= step[facing][0];
                pos[1] -= step[facing][1];
                if (not inBnds(maze, pos)) {cout << "WRAPPING POST-COLLISION" << endl; wrap(maze,pos,(facing+2)%4);}
                break;
            }
            tmp--;
        }
    }

    cout << "Ended at: " << pos[0] << "," << pos[1] << " " << facing << endl;

    input.close();
    cout << "#####################\n";
    cout << (pos[0]+1)*1000 + (pos[1]+1)*4 + (facing-1)%4 << '\n';
    cout << "#####################\n";
    return 0;
}
