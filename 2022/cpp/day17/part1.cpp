#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;
bool isEmpty(array<bool,7> arr) {
    bool ret = arr[0];
    for (size_t i=1; i<arr.size(); i++) {ret = ret or arr[i];}
    return not ret;
}
bool isFull(array<bool,7> arr) {
    bool ret = arr[0];
    for (size_t i=1; i<arr.size(); i++) {ret = ret and arr[i];}
    return not ret;
}

class Piece {
public:
    int size_x;
    int size_y;
    vector<vector<bool>> shape;
    Piece(int index) {
        switch(index%5) {
        case 0:
            size_x = 4;
            size_y = 1;
            shape = vector<vector<bool>>(size_y);
            shape[0] = vector<bool>({true, true, true, true});
            break;
        case 1:
            size_x = 3;
            size_y = 3;
            shape = vector<vector<bool>>(size_y);
            shape[0] = vector<bool>({false, true, false});
            shape[1] = vector<bool>({true, true, true});
            shape[2] = vector<bool>({false, true, false});
            break;
        case 2:
            size_x = 3;
            size_y = 3;
            shape = vector<vector<bool>>(size_y);
            shape[0] = vector<bool>({false, false, true});
            shape[1] = vector<bool>({false, false, true});
            shape[2] = vector<bool>({true, true, true});
            break;
        case 3:
            size_x = 1;
            size_y = 4;
            shape = vector<vector<bool>>(size_y);
            shape[0] = vector<bool>({true});
            shape[1] = vector<bool>({true});
            shape[2] = vector<bool>({true});
            shape[3] = vector<bool>({true});
            break;
        case 4:
            size_x = 2;
            size_y = 2;
            shape = vector<vector<bool>>(size_y);
            shape[0] = vector<bool>({true, true});
            shape[1] = vector<bool>({true, true});
            break;
        default:
            shape = vector<vector<bool>>();
            break;
        }
    }
};

class Cave {
public:
    list<array<bool,7>> space;
    vector<bool> wind_positive;
    int wind_idx;
    int piece_idx;
    int base_height;

    Cave(vector<bool> wind_positive_)
    : wind_positive(wind_positive_) {
        auto empty = array<bool,7>();
        for (size_t i=0; i<empty.size(); i++) {empty[i]=false;}
        for (size_t i=0; i<4; i++) {space.emplace_back(empty);}
        piece_idx = 0;
        wind_idx = 0;
    }

    void add_space_top() {
        // count number of empty rows
        size_t nEmptyRows = 0;
        auto row = space.begin();
        while (nEmptyRows < space.size() and isEmpty(*row)) {nEmptyRows++; row++;}
        if (nEmptyRows < 4) {
            auto empty = array<bool,7>();
            while (nEmptyRows < 4) {
                space.emplace_front(empty);
                nEmptyRows++;
            }
        }
    }

    void drop_piece() {
        Piece pc(piece_idx);
        auto to_drop = pc.shape;
        int offset_x = 2;
        int offset_y = -pc.size_y+1;
        while (evolve(offset_x,offset_y)) {cout << "curr. pos: " << offset_x << " " << offset_y << '\n';}
        cout << "Placing piece at final pos: " << offset_x << " " << offset_y << '\n';
        place_piece(piece_idx, offset_x, offset_y);
        piece_idx++;
        clear_space();
        add_space_top();
        print();
    }

    bool check_collision_lr(int offset_x, int offset_y, bool positive_side) {
        Piece pc(piece_idx);
        // scan to current line
        int i=0; auto cit = space.begin();
        while (i<offset_y) {
            i++;
            cit++;
        }

        bool ret = false;
        while (i < offset_y + pc.size_y) {
            for (int j=0; j<pc.size_x; j++) {
                if (positive_side) {
                    if (offset_x + j + 1 >= 7) {cout << "Hit R side\n"; return true;}
                    if (pc.shape[i-offset_y][j]) {ret = ret or (*cit)[offset_x + j + 1];}
                } else {
                    if (offset_x + j - 1 < 0 ) {cout << "Hit L side\n"; return true;}
                    if (pc.shape[i-offset_y][j]) {ret = ret or (*cit)[offset_x + j - 1];}
                }
                if (ret) {return true;}
            }
            i++;
            cit++;
        }
        return ret;
    }

    bool check_collision_d(int &offset_x, int &offset_y) {
        Piece pc(piece_idx);
        // scan to current line
        int i=0; auto cit = space.begin();
        while (i<offset_y) {
            i++;
            cit++;
        }

        bool ret = false;
        while (i < offset_y + pc.size_y) {
            if (cit==space.end()) {cout << "Hit bottom\n"; return true;}
            if (ret) {return true;}
            cit++;
            cout << i << " " << offset_y << " " << pc.size_y << '\n';
            for (int j=0; j<pc.size_x; j++) {
                if (pc.shape[i-offset_y][j]) {ret = ret or (*cit)[offset_x + j];}
            }
            i++;
        }
        if (ret) {return true;}
        offset_y++;
        return false;
    }

    bool evolve(int &offset_x, int &offset_y) {
        cout << "Wind direction: " << wind_positive[wind_idx] << '\n';
        if (wind_positive[wind_idx]) {
            if (not check_collision_lr(offset_x,offset_y,wind_positive[wind_idx])) {
                offset_x++;
            }
        } else {
            if (not check_collision_lr(offset_x,offset_y,wind_positive[wind_idx])) {
                offset_x--;
            }
        }
        wind_idx = (wind_idx+1)%wind_positive.size();
        return not check_collision_d(offset_x, offset_y);
    }

    void place_piece(int piece_idx, int offset_x, int offset_y) {
        Piece pc(piece_idx);
        int i = 0;
        auto cit = space.begin();
        while (i<offset_y) {
            i++;
            cit++;
        }
        for (int j=0; j<pc.size_y; j++) {
            for (int k=0; k<pc.size_x; k++) {
                if (pc.shape[j][k]) {(*cit)[k+offset_x] = true;}
            }
            cit++;
        }
        print();
    }

    void clear_space() {
        auto cit = space.end();
        int tmp_height = 1;
        while (cit != space.begin() and (not isFull(*cit))) {cit--;tmp_height++;}
        if (cit != space.begin()) {
            space.erase(cit,space.end());
            base_height += tmp_height;
        }
        
    }

    void print() {
        int size = 0;
        for (auto p : space) {size++;}
        cout << "Cave space has size: " << size << '\n';
        for (auto p : space) {
            cout << '|';
            for (size_t i=0; i<p.size(); i++) {
                cout << (p[i] ? '#' : '.');
            }
            cout << "|\n";
        }
        cout << "=========\n";
    }

};

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    getline(input,line);

    vector<bool> wind(line.length());
    for (size_t i=0; i<line.length(); i++) {
        wind[i] = (line[i]=='>');
    }

    Cave cave(wind);
    while (cave.piece_idx<3) {cave.drop_piece();}
    //cave.print();

    input.close();
    cout << "#####################\n";
    cout << cave.base_height << " " << cave.space.size() << '\n';
    cout << "#####################\n";
    return 0;
}
