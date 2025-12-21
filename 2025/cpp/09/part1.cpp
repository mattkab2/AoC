#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

std::vector<std::pair<int, int>> get_top_pts(const std::map<int, std::vector<int>, std::greater<int>> &quadrant, const std::vector<std::pair<int, int>> &coords) {
    std::vector<std::pair<int, int>> ret;
    for (const auto &[_, lst] : quadrant) {
        for (const auto &pt : lst) {
            ret.push_back(coords[pt]);
        }
        if (ret.size() > 10) {
            break;
        }
    }
    return ret;
}

long get_max_area(const std::vector<std::pair<int, int>> &p, const std::vector<std::pair<int, int>> &q) {
    long ret = 0;
    for (size_t i = 0; i < p.size(); i++) {
        for (size_t j = i; j < q.size(); j++) {
            std::cout << "Checking pt " << p[i].first << "," << p[i].second << " x " << q[j].first << "," << q[j].second << std::endl;
            ret = std::max(ret, std::abs(long(p[i].first - q[j].first+1)*long(p[i].second - q[j].second+1)));
        }
    }
    return ret;
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;

    std::vector<std::pair<int, int>> coords;
    std::pair<int, int> centroid;
    while (getline(input, line)) {
        auto c1 = line.find(',');
        coords.emplace_back(std::pair<int, int>{std::stoi(line.substr(0, c1)), std::stoi(line.substr(c1+1, line.size() - c1 - 1))});
        centroid = std::make_pair(centroid.first + coords.back().first, centroid.second + coords.back().second);
    }
    centroid = std::make_pair(centroid.first/coords.size(), centroid.second/coords.size());
    const auto &[cx, cy] = centroid;

    std::map<int, std::vector<int>, std::greater<int>> q1Norm;
    std::map<int, std::vector<int>, std::greater<int>> q2Norm;
    std::map<int, std::vector<int>, std::greater<int>> q3Norm;
    std::map<int, std::vector<int>, std::greater<int>> q4Norm;
    for (size_t i = 0; i < coords.size(); i++) {
        const auto &[x, y] = coords[i];
        if (x - cx >= 0 && y - cy >= 0) {
            q1Norm[x + y].push_back(i);
        } else if (x - cx < 0 && y - cy >= 0) {
            q2Norm[y - x].push_back(i);
        } else if (x - cx < 0 && y - cy < 0) {
            q3Norm[-x -y].push_back(i);
        } else {
            q4Norm[x - y].push_back(i);
        }
    }

    long maxArea = 0;
    std::vector<std::pair<int, int>> corner1;
    std::vector<std::pair<int, int>> corner2;

    // Iterate through the top 10 entries of Q1/3
    corner1 = get_top_pts(q1Norm, coords);
    corner2 = get_top_pts(q3Norm, coords);
    // Find the maximum
    maxArea = get_max_area(corner1, corner2);
    // Iterate through the top 10 entries of Q2,4
    corner1 = get_top_pts(q2Norm, coords);
    corner2 = get_top_pts(q4Norm, coords);
    // Find the maximum
    maxArea = std::max(maxArea, get_max_area(corner1, corner2));


    input.close();
    cout << "#####################\n";
    cout << maxArea << '\n';
    cout << "#####################\n";
    return 0;
}
