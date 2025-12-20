#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;

    std::vector<std::array<long, 3>> coords;
    while (getline(input, line)) {
        auto c1 = line.find(',');
        auto c2 = line.find(',', c1+1);
        coords.emplace_back(std::array<long, 3>{std::stol(line.substr(0, c1)), std::stol(line.substr(c1+1, c2 - c1 - 1)), std::stol(line.substr(c2+1, line.size()-c2 - 1))});
    }

    std::map<long, std::pair<long, long>> nearest;
    for (size_t pInd = 1; pInd < coords.size(); pInd++) {
        for (size_t qInd = 0; qInd < pInd; qInd++) {
            long dist = (coords[pInd].at(0) - coords[qInd].at(0))*(coords[pInd].at(0) - coords[qInd].at(0));
            dist += (coords[pInd].at(1) - coords[qInd].at(1))*(coords[pInd].at(1) - coords[qInd].at(1));
            dist += (coords[pInd].at(2) - coords[qInd].at(2))*(coords[pInd].at(2) - coords[qInd].at(2));
            if (nearest.contains(dist)) {
                throw std::runtime_error("Tie for lowest distance");
            }
            nearest[dist] = std::make_pair(qInd, pInd);
        }
    }

    long NUM_CONNECTIONS = 1000;
    long connections = 0;
    long circIndex = 0;
    std::map<long, std::unordered_set<long>> circuits;
    std::map<long, long> boxToCircuit;
    for (const auto &[dist, pair] : nearest) {
        std::string b1 = std::to_string(coords[pair.first].at(0)) + "," + std::to_string(coords[pair.first].at(1)) + "," + std::to_string(coords[pair.first].at(2));
        std::string b2 = std::to_string(coords[pair.second].at(0)) + "," + std::to_string(coords[pair.second].at(1)) + "," + std::to_string(coords[pair.second].at(2));

        if (boxToCircuit.contains(pair.first) && boxToCircuit.contains(pair.second) && boxToCircuit.at(pair.first) == boxToCircuit.at(pair.second)) {
            std::cout << "Skipping connection between " << b1 << " and " << b2 << "; both on circuit " << boxToCircuit[pair.first] << std::endl;
        } else {
            std::cout << "Connecting box " << b1 << " and " << b2 << std::endl;
            if (boxToCircuit.contains(pair.first) && boxToCircuit.contains(pair.second)) {
                std::cout << "  Merging circuits " << boxToCircuit[pair.first] << " and " << boxToCircuit[pair.second] << " into " << circIndex << std::endl;
                // Merge the circuits
                long origCircuit = boxToCircuit[pair.first];
                for (const auto &box : circuits[origCircuit]) {
                    circuits[circIndex].insert(box);
                    boxToCircuit.at(box) = circIndex;
                }
                circuits.erase(origCircuit);
                origCircuit = boxToCircuit[pair.second];
                for (const auto &box : circuits[origCircuit]) {
                    circuits[circIndex].insert(box);
                    boxToCircuit.at(box) = circIndex;
                }
                circuits.erase(origCircuit);
                circIndex++;
            } else if (boxToCircuit.contains(pair.first)) {
                std::cout << "  Adding to circuit: " << boxToCircuit[pair.first] << std::endl;
                boxToCircuit[pair.second] = boxToCircuit[pair.first];
                circuits[boxToCircuit[pair.first]].insert(pair.second);
            } else if (boxToCircuit.contains(pair.second)) {
                std::cout << "  Adding to circuit: " << boxToCircuit[pair.second] << std::endl;
                boxToCircuit[pair.first] = boxToCircuit[pair.second];
                circuits[boxToCircuit[pair.second]].insert(pair.first);
            } else {
                std::cout << "  Creating new circuit: " << circIndex << std::endl;
                boxToCircuit[pair.first] = circIndex;
                boxToCircuit[pair.second] = circIndex;
                circuits[circIndex].insert(pair.first);
                circuits[circIndex].insert(pair.second);
                circIndex++;
            }
        }

        connections++;
        if (connections == NUM_CONNECTIONS) {
            break;
        }
    }

    // Make the list of sizes and sort it
    std::vector<size_t> sizes;
    for (const auto &[circID, set] : circuits) {
        std::cout << circID << ": " << set.size() << std::endl;
        sizes.push_back(set.size());
    }
    std::sort(sizes.begin(), sizes.end(), std::greater<size_t>());

    input.close();
    cout << "#####################\n";
    cout << sizes[0]*sizes[1]*sizes[2] << '\n';
    cout << "#####################\n";
    return 0;
}
