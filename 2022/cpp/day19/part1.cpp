#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <map>

using namespace std;

enum Resource { ore, clay, obsidian, geode };

class Recipe {
public:
    int output;
    list<pair<Resource,int>> parts_list;

    Recipe() {
        output = 0;
        list<pair<Resource,int>> parts_list;
    }

    Recipe(int output_, int ore_count, int clay_count, int obsidian_count)
    : output(output_) {
        if (ore_count > 0) {parts_list.emplace_back(pair<Resource,int>(ore,ore_count));}
        if (clay_count > 0) {parts_list.emplace_back(pair<Resource,int>(clay,clay_count));}
        if (obsidian_count > 0) {parts_list.emplace_back(pair<Resource,int>(obsidian,obsidian_count));}
    }

    map<Resource,int> use_material(const map<Resource,int> inventory) const {
        auto ret = map<Resource,int>(inventory);
        for (auto mat : parts_list) {
            ret[mat.first] -= mat.second;
        }
        return ret;
    }
};

class Blueprint {
public:
    map<Resource,Recipe> recipe_list;

    int optimize(int minutes) {
        map<Resource,int> inventory;
        inventory[ore] = 0;
        inventory[clay] = 0;
        inventory[obsidian] = 0;
        inventory[geode] = 0;
        map<Resource,int> robots;
        inventory[ore] = 1;
        inventory[clay] = 0;
        inventory[obsidian] = 0;
        inventory[geode] = 0;

        return evolve(inventory, robots, minutes);
    }

    int evolve(map<Resource,int> inventory, map<Resource,int> robots, int minutes) {
        // validity check
        for (auto const& [key, val] : inventory) {if (val<0) {return 0;}}
        if (minutes==0) {return inventory[geode];}
        if (minutes > 18) {
            cout << "On minute: " << minutes << endl;
        }
        // Update inventory
        for (auto const& [key, val] : robots) {
            inventory[key] += val;
        }

        int ret=0;
        // build robot
        for (auto const& [key,val] : recipe_list) {
            auto new_inv = val.use_material(inventory);
            robots[key]++;
            ret = max(ret,evolve(new_inv, robots, minutes-1));
            robots[key]--;
        }
        // pass
        ret = max(ret,evolve(inventory,robots,minutes-1));
        return ret;
    }
};

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;
    int ore_cost, clay_cost, obsidian_cost, nGeodes;
    while (getline(input, line)) {
        Blueprint bp;
        istringstream iss(line);
        iss >> ore_cost;
        bp.recipe_list[ore] = Recipe(1,ore_cost,0,0);

        iss >> ore_cost;
        bp.recipe_list[clay] = Recipe(1,ore_cost,0,0);

        iss >> ore_cost >> clay_cost;
        bp.recipe_list[obsidian] = Recipe(1,ore_cost,clay_cost,0);

        iss >> ore_cost >> obsidian_cost;
        bp.recipe_list[geode] = Recipe(1,ore_cost,0,obsidian_cost);

        nGeodes = bp.optimize(24);
        cout << nGeodes << endl;
    }

    input.close();
    cout << "#####################\n";
    cout << 0 << '\n';
    cout << "#####################\n";
    return 0;
}
