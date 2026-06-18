// bfs, T: O(mn), n = routes, m = stops per route, S: O(mn)

#include <vector>
#include <unordered_map>
#include <unordered_set>

class Solution {
public:
    int numBusesToDestination(std::vector<std::vector<int>>& routes, int source, int target) {
        if (source == target) { return 0; }

        std::unordered_map<int, std::vector<int>> stop_to_routes; // stop -> list of routes passing through it
        for (int i = 0; i < static_cast<int>(routes.size()); i++) {
            for (int stop : routes[i]) {
                stop_to_routes[stop].push_back(i);
            }
        }

        std::unordered_set<int> visited_stops;
        std::unordered_set<int> visited_routes;
        std::vector<int> q;
        q.reserve(stop_to_routes.size());
        int head = 0;

        q.push_back(source);
        visited_stops.insert(source);
        int buses = 0;

        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;
            buses++;

            for (int i = 0; i < sz; i++) {
                int stop = q[head];
                head++;

                for (int route : stop_to_routes[stop]) {
                    if (visited_routes.count(route)) { continue; }
                    visited_routes.insert(route);

                    for (int next_stop : routes[route]) {
                        if (next_stop == target) { return buses; }
                        if (!visited_stops.count(next_stop)) {
                            visited_stops.insert(next_stop);
                            q.push_back(next_stop);
                        }
                    }
                }
            }
        }
        return -1;
    }
};

// bellman-ford, T: O(mn^2), S: O(S)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int numBusesToDestination(std::vector<std::vector<int>>& routes, int source, int target) {
        if (source == target) { return 0; }

        int max_stop = std::max(source, target); // seed with source and target
        for (auto& route : routes) {
            for (int stop : route) {
                max_stop = std::max(max_stop, stop);
            }
        }

        std::vector<int> min_buses(max_stop + 1, INT_MAX);
        min_buses[source] = 0;

        int n = static_cast<int>(routes.size());
        bool updated = true;

        while (updated) {
            updated = false;

            for (auto& route : routes) {
                int route_min = n + 1;

                for (int stop : route) {
                    if (min_buses[stop] != INT_MAX) {
                        route_min = std::min(route_min, min_buses[stop]);
                    }
                }
                if (route_min == n + 1) { continue; }
                route_min++;

                for (int stop : stops) {
                    if (min_buses[stop] > route_min) {
                        min_buses[stop] = route_min;
                        updated = true;
                    }
                }
            }
        }
    }
    return (min_buses[target] <= n) ? min_buses[target] : -1;
};

// bfs on routes not stop: bus change = one bfs level, stop_to_routes inverted index map
// visited_routes: prevents route reprocessing
// visited_stops: prevents enqueuing same stop twice
