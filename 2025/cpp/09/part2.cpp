#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

void set_quadrant(std::vector<std::vector<int>> &bounds, const std::vector<int> &xDiv, const std::vector<int> &yDiv, const std::pair<int, int> &coords, int lastDir, int currDir) {
    auto xIt = std::find(xDiv.begin(), xDiv.end(), coords.first);
    auto yIt = std::find(yDiv.begin(), yDiv.end(), coords.second);
    if ((lastDir == -1 && currDir == -2) || (lastDir == 2 && currDir == 1)) {
        bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)] = 3;
    } else if ((lastDir == -1 && currDir == 2) || (lastDir == -2 && currDir == 1)) {
        bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)] = 4;
    } else if ((lastDir == -2 && currDir == -1) || (lastDir == 1 && currDir == 2)) {
        bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)] = 1;
    } else {
        bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)] = 2;
    }
}

int main(int argc, char *argv[]) {
    ifstream input;
    input.open(argv[1], ios::in);
    string line;

    std::vector<std::pair<int, int>> coords;
    std::set<int> xDivS;
    std::set<int> yDivS;
    while (getline(input, line)) {
        auto c1 = line.find(',');
        coords.emplace_back(std::stoi(line.substr(0, c1)), std::stoi(line.substr(c1+1, line.size() - c1 - 1)));
        xDivS.insert(std::stoi(line.substr(0, c1)));
        yDivS.insert(std::stoi(line.substr(c1+1, line.size() - c1 - 1)));
    }
    std::vector<int> xDiv(xDivS.begin(), xDivS.end());
    std::vector<int> yDiv(yDivS.begin(), yDivS.end());
    for (const auto &x : xDiv) {
        std::cout << x << ", ";
    }
    std::cout << std::endl;
    for (const auto &y : yDiv) {
        std::cout << y << ", ";
    }
    std::cout << std::endl;

    // Go back through the coords and fill out the array of boundaries
    //   we only need vertical OR horizontal boundaries; choose vertical
    std::vector<std::vector<int>> bounds(xDiv.size(), std::vector<int>(yDiv.size(), 0));

    int currDir = 0;
    int lastDir = 0;
    int finalDir = 0;
    for (size_t cIdx = 0; cIdx < coords.size(); cIdx++) {
        if (coords[cIdx].first == coords[(cIdx+1)%coords.size()].first) {
            int yLo = std::min(coords[cIdx].second, coords[(cIdx+1)%coords.size()].second);
            int yHi = std::max(coords[cIdx].second, coords[(cIdx+1)%coords.size()].second);
            currDir = (yLo == coords[cIdx].second) ? 2 : -2;
            auto xIt = std::find(xDiv.begin(), xDiv.end(), coords[cIdx].first);
            if (xIt == xDiv.end()) {throw std::runtime_error("Search failed");}
            auto yIt = std::find(yDiv.begin(), yDiv.end(), yLo);
            if (yIt == yDiv.end()) {throw std::runtime_error("Search failed");}
            while (*yIt <= yHi && yIt != yDiv.end()) {
                bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)]--;
                yIt++;
            }
        } else if (coords[cIdx].second == coords[cIdx].second) {
            int xLo = std::min(coords[cIdx].first, coords[(cIdx+1)%coords.size()].first);
            int xHi = std::max(coords[cIdx].first, coords[(cIdx+1)%coords.size()].first);
            currDir = (xLo == coords[cIdx].first) ? 1 : -1;
            auto yIt = std::find(yDiv.begin(), yDiv.end(), coords[cIdx].second);
            if (yIt == yDiv.end()) {throw std::runtime_error("Search failed");}
            auto xIt = std::find(xDiv.begin(), xDiv.end(), xLo);
            if (xIt == xDiv.end()) {throw std::runtime_error("Search failed");}
            while (*xIt <= xHi && xIt != xDiv.end()) {
                bounds[std::distance(xDiv.begin(), xIt)][std::distance(yDiv.begin(), yIt)]--;
                xIt++;
            }
        } else {
            throw std::runtime_error("Invalid input");
        }
        if (lastDir == 0) {
            finalDir = currDir;
        } else {
            set_quadrant(bounds, xDiv, yDiv, coords[cIdx], lastDir, currDir);
        }
        lastDir = currDir;
    }
    set_quadrant(bounds, xDiv, yDiv, coords[0], lastDir, finalDir);
    for (size_t i = 0; i < xDiv.size(); i++) {
        for (size_t j = 0; j < yDiv.size(); j++) {
            std::cout << bounds[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    // Build the cache for in/out
    std::vector<std::vector<bool>> inside(xDiv.size()-1, std::vector<bool>(yDiv.size()-1, false));
    ofstream of;
    of.open("inside.txt");
    for (size_t i = 0; i < xDiv.size() - 1; i++) {
        // Here we imagine this as drawing a line through the shape along a given vertical line x = xDiv[]
        bool isInside = false;
        for (size_t j = 0; j < yDiv.size() - 1; j++) {
            if ((bounds[i][j] == -1 && bounds[i+1][j] == -1) ||
                (bounds[i][j] == -1 && (bounds[i+1][j] == 1 || bounds[i+1][j] == 2)) ||
                (bounds[i+1][j] == -1 && (bounds[i][j] == 3 || bounds[i][j] == 4)) ||
                ((bounds[i][j] == 4 || bounds[i][j] == 3) && (bounds[i+1][j] == 1 || bounds[i+1][j] == 2))) {
                isInside = !isInside;
            }
            inside[i][j] = isInside;
            of << (isInside ? 100 : 0) << ",";
        }
        of << std::endl;
    }
    of.close();

    for (size_t i = 0; i < inside.size(); i++) {
        for (size_t j = 0; j < inside.front().size(); j++) {
            std::cout << (inside[i][j] ? '#' : '.');
        }
        std::cout << std::endl;
    }

    long maxArea = 0;
    for (size_t i = 0; i < coords.size(); i++) {
        const auto &[x1, y1] = coords.at(i);
        for (size_t j = 0; j < i; j++) {
            const auto &[x2, y2] = coords.at(j);
            if ((long(std::abs(x2 - x1)) + 1) * (long(std::abs(y2 - y1)) + 1) <= maxArea) {
                continue;
            }
            // Make sure every tile between x1, y1 and x2, y2 is green
            auto xLo = std::distance(xDiv.begin(), std::lower_bound(xDiv.begin(), xDiv.end(), std::min(x1, x2)));
            auto xHi = std::distance(xDiv.begin(), std::lower_bound(xDiv.begin(), xDiv.end(), std::max(x1, x2)));
            auto yLo = std::distance(yDiv.begin(), std::lower_bound(yDiv.begin(), yDiv.end(), std::min(y1, y2)));
            auto yHi = std::distance(yDiv.begin(), std::lower_bound(yDiv.begin(), yDiv.end(), std::max(y1, y2)));
            bool allInside = true;
            for (size_t ii = xLo; ii < xHi; ii++) {
                for (size_t jj = yLo; jj < yHi; jj++) {
                    allInside = allInside && inside[ii][jj];
                }
                if (!allInside) {break;}
            }
            if (allInside) {
                std::cout << "Accepted rectangle with area: " << (long(std::abs(x2 - x1)) + 1) * (long(std::abs(y2 - y1)) + 1) << std::endl;
                std::cout << "  " << xDiv[xLo] << "," << yDiv[yLo] << " x " << xDiv[xHi] << "," << yDiv[yHi] << std::endl;
                std::cout << "  " << x1 << "," << y1 << " x " << x2 << "," << y2 << std::endl;
                maxArea = (long(std::abs(x2 - x1)) + 1) * (long(std::abs(y2 - y1)) + 1);
            }
        }
    }

    input.close();
    cout << "#####################\n";
    cout << maxArea << '\n';
    cout << "#####################\n";
    return 0;
}
