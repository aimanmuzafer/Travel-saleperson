#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <stack>
#include <unordered_set>

using namespace std;

// Structure to represent a city
struct City {
    double lat;
    double lon;
};

// Function to calculate the distance between two cities
double calculateDistance(City city1, City city2) {
    double R = 6371.0; // Earth radius in kilometers
    double dlat = (city2.lat - city1.lat) * M_PI / 180.0;
    double dlon = (city2.lon - city1.lon) * M_PI / 180.0;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(city1.lat * M_PI / 180.0) * cos(city2.lat * M_PI / 180.0) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

// Function to solve TSP using Branch and Bound
pair<vector<int>, double> tspBranchBound(vector<vector<double>>& distances) {
    int n = distances.size();
    vector<int> bestPath;
    double bestCost = numeric_limits<double>::infinity();
    stack<tuple<int, vector<int>, unordered_set<int>, double>> s;
    s.push(make_tuple(0, vector<int>{0}, unordered_set<int>{0}, 0));
    while (!s.empty()) {
        auto node = s.top();
        s.pop();
        if (get<1>(node).size() == n) {
            double cost = get<3>(node) + distances[get<1>(node).back()][0];
            if (cost < bestCost) {
                bestPath = get<1>(node);
                bestCost = cost;
            }
        } else {
            unordered_set<int> unvisited;
            for (int i = 0; i < n; ++i) {
                if (get<2>(node).find(i) == get<2>(node).end()) {
                    unvisited.insert(i);
                }
            }
            for (int i : unvisited) {
                vector<int> childPath = get<1>(node);
                childPath.push_back(i);
                double childCost = get<3>(node) + distances[get<1>(node).back()][i];
                if (childCost < bestCost) {
                    unordered_set<int> childSet = get<2>(node);
                    childSet.insert(i);
                    s.push(make_tuple(i, childPath, childSet, childCost));
                }
            }
        }
    }
    return make_pair(bestPath, bestCost);
}

int main() {
    // Coordinates of cities
    vector<City> cities = {
        {52.5200, 13.4050}, // Berlin
        {53.5511, 9.9937},  // Hamburg
        {48.1351, 11.5820}, // Munich
        {50.9375, 6.9603},  // Cologne
        {50.1109, 8.6821}   // Frankfurt
    };

    // Calculate distances between cities
    vector<vector<double>> distances(cities.size(), vector<double>(cities.size()));
    for (int i = 0; i < cities.size(); ++i) {
        for (int j = 0; j < cities.size(); ++j) {
            distances[i][j] = calculateDistance(cities[i], cities[j]);
        }
    }

    // Solve TSP using Branch and Bound
    pair<vector<int>, double> result = tspBranchBound(distances);

    // Output the best path and its cost
    cout << "Best Path: ";
    for (int i = 0; i < result.first.size(); ++i) {
        cout << result.first[i] << " ";
    }
    cout << endl;
    cout << "Best Cost: " << result.second << endl;

    return 0;
}
