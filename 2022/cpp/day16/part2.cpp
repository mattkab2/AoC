#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>

using namespace std;
class Room {
public:
    static size_t nRooms;
    static size_t nValves;
    size_t room_id,valve_id;
    string name;
    int flow;
    list<string> path_str;
    list<Room*> paths;
    Room() {
        path_str = list<string>();
        name = "";
        valve_id = -1;
    }
    Room(string name_, int flow_, istringstream &line)
    : name(name_), flow(flow_) {
        path_str = list<string>();
        string tmp;
        while (line >> tmp) {path_str.emplace_back(tmp);}
        room_id = nRooms;
        valve_id = nValves;
        nRooms++;
        if (flow>0) {nValves++;}
    }
    void update_paths(vector<Room> &rooms) {
        for (auto s : path_str) {
            for (size_t i=0; i<rooms.size(); i++) {
                if (i==room_id) {continue;}
                if (rooms[i].name.compare(s)==0) {
                    paths.emplace_back(&(rooms[i]));
                    break;
                }
            }
        }
    }
};
size_t Room::nRooms = 0;
size_t Room::nValves = 1;

using namespace std;
vector<int> dist(Room* start) {
    auto dist = vector<int>(Room::nRooms);
    auto visited = vector<bool>(Room::nRooms);
    dist[start->room_id] = 0;
    visited[start->room_id] = true;
    queue<Room*> bfs;

    bfs.push(start);
    while (bfs.size()!=0) {
        Room* curr = bfs.front();
        bfs.pop();
        for (auto r : curr->paths) {
            if (not visited[r->room_id]) {
                dist[r->room_id] = dist[curr->room_id]+1;
                bfs.push(r);
                visited[r->room_id] = true;
            }
        }
    }
    return dist;
}

int path_search(const vector<Room*> &valves,
                const vector<vector<int>> &dists,
                vector<bool> valve_used,
                pair<int,int> curr_valve, int curr_outflow,
                pair<int,int> time_left, int valves_left) {
    
    if (time_left.first<=0 and time_left.second<=0) {return curr_outflow;}
    if (time_left.first>0) {
        valve_used[curr_valve.first]=true;
        curr_outflow += valves[curr_valve.first]->flow * (time_left.first-1);
        valves_left--;
    }
    if (time_left.second>0) {
        valve_used[curr_valve.second]=true;
        curr_outflow += valves[curr_valve.second]->flow * (time_left.second-1);
        valves_left--;
    }
    if (valves_left==0) {return curr_outflow;}
    if (valves_left==1) {
        int target_valve = 0;
        for (size_t i=0; i<valves.size(); i++) {
            if (not valve_used[i]) {target_valve = i; break;}
        }
        auto t1 = pair<int,int>({0,time_left.second});
        auto t2 = pair<int,int>({time_left.first,0});
        int ret = max(
            path_search(
                valves, dists, valve_used, pair<int,int>(target_valve,target_valve),
                curr_outflow, t1, valves_left
            ),
            path_search(
                valves, dists, valve_used, pair<int,int>(target_valve,target_valve),
                curr_outflow, t2, valves_left-1
            )
        );
        return ret;
    }
    
    int ret = 0;
    for (size_t i=0; i<valves.size(); i++) {
        if (valve_used[i]) {continue;}
        for (size_t j=0; j<valves.size(); j++) {
            if (j==i or valve_used[i]) {continue;}
            auto tl = pair<int,int>(time_left.first-dists[curr_valve.first][i], time_left.second-dists[curr_valve.second][j]);
            ret = max(ret,path_search(
                valves, dists, valve_used, pair<int,int>(i,j),
                curr_outflow, tl, valves_left)
            );
        }
    }
    return ret;
}

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);

    string line;
    string name;
    int flow;
    vector<Room> pipe_system;

    while (getline(input, line)) {
        istringstream iss(line);
        iss >> name >> flow;
        if (name.compare("AA")==0) {
            pipe_system.push_back(Room(name,flow,iss));
            break;
        }
    }
    input.clear();
    input.seekg(0);
    while (getline(input, line)) {
        istringstream iss(line);
        iss >> name >> flow;
        if (name.compare("AA")!=0) {
            pipe_system.push_back(Room(name,flow,iss));
        }
    }
    
    cout << pipe_system[0].name;
    for (size_t i=0; i<pipe_system.size(); i++) {
        pipe_system[i].update_paths(pipe_system);
    }

    // Find valves and create matrix of distances
    cout << Room::nRooms << " " << Room::nValves << '\n';
    auto dist_matrix = vector<vector<int>>(Room::nValves);
    auto valves = vector<Room*>(Room::nValves);
    int idx=0;
    for (size_t i=0; i<pipe_system.size(); i++) {
        if (i!=0 and pipe_system[i].flow==0) {continue;}
        valves[idx] = &pipe_system[i];
        auto dists = dist(&pipe_system[i]);
        int jdx=0;
        dist_matrix[idx] = vector<int>(Room::nValves);
        for (size_t j=0; j<pipe_system.size(); j++) {
            if (j!=0 and pipe_system[j].flow==0) {continue;}
            dist_matrix[idx][jdx] = dists[j];
            jdx++;
        }
        idx++;
    }
    for (size_t i=0; i<Room::nValves; i++) {
        for (size_t j=0; j<Room::nValves; j++) {
            if (dist_matrix[i][j] < 10) {cout << "0";}
            cout << dist_matrix[i][j] << " ";
        }
        cout << '\n';
    }

    auto path = vector<pair<string,int>>(0);
    auto valve_used = vector<bool>(Room::nValves);
    for (size_t i=1; i<Room::nValves; i++) {
        valve_used[i] = false;
    }
    int ret = path_search(valves, dist_matrix, valve_used,
                          pair<int,int>(0,0),
                          0,
                          pair<int,int>(27,27),
                          Room::nValves);


    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
