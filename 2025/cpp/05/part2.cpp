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
    std::vector<std::pair<long,long>> ranges;
    bool readPairs = true;
    while (getline(input, line)) {
        if (line.empty()) {
            break;
        }
        size_t dash = line.find('-', 0);
        long lo = std::stol(line.substr(0, dash));
        long hi = std::stol(line.substr(dash+1, line.size() - dash));
        ranges.emplace_back(lo,hi);
    }

    long ret = 0;
    std::vector<std::pair<long, long>> patches;
    for (const auto &[lo, hi] : ranges) {
        if (patches.empty()) {
            patches.emplace_back(lo, hi);
            continue;
        }
        // If there's no overlap, insert front or back
        if (hi < patches.front().first) {
            patches.insert(patches.begin(), std::make_pair(lo, hi));
            continue;
        } else if (lo > patches.back().second) {
            patches.emplace_back(lo, hi);
            continue;
        }

        size_t patchInd = 0;
        while (patchInd < patches.size() && patches[patchInd].second < lo) {
            patchInd++;
        }
        // Figure out if we insert this range as a patch, or if there is overlap with the above and/or below
        if (hi < patches[patchInd].first - 1) {
            // Insert, no modifications
            patches.insert(patches.begin() + patchInd, std::make_pair(lo, hi));
        } else if (hi <= patches[patchInd].second) {
            // Fix up current patch with new minimum
            long loTmp = std::min(lo, patches[patchInd].first);
            patches[patchInd] = std::make_pair(loTmp, patches[patchInd].second);
        } else {
            // Figure out how many patches to overwrite
            size_t upperPatchInd = patchInd;
            while (upperPatchInd < patches.size() - 1 && hi > patches[upperPatchInd+1].first - 1) {
                upperPatchInd++;
            }
            // Erase the range and insert the new one
            long loTmp = std::min(lo, patches[patchInd].first);
            long hiTmp = std::max(hi, patches[upperPatchInd].second);
            patches.erase(patches.begin() + patchInd, patches.begin() + upperPatchInd + 1);
            patches.insert(patches.begin() + patchInd, std::make_pair(loTmp, hiTmp));
        }
    }

    for (const auto &[lo, hi] : patches) {
        ret += hi - lo + 1;
    }

    input.close();
    cout << "#####################\n";
    cout << ret << '\n';
    cout << "#####################\n";
    return 0;
}
