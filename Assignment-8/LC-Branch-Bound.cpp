#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <limits>
#include <cstdint>
using namespace std;

const double INF = numeric_limits<double>::infinity();

struct State {
    vector<int> path;
    uint64_t visited;
    int last;
    double cost;
    double time;
    double bound;
};

struct Compare {
    bool operator()(const State& a, const State& b) const {
        if (a.bound != b.bound)
            return a.bound > b.bound;
        return a.path > b.path;
    }
};

int main() {
    int n, roads;
    double timeLimit;

    cout << "Enter number of cities: ";
    cin >> n;
    cout << "Enter number of directed roads: ";
    cin >> roads;
    cout << "Enter delivery time limit: ";
    cin >> timeLimit;

    if (n < 1 || n > 63 || roads < 0 || timeLimit < 0) {
        cout << "Invalid input\n";
        return 1;
    }

    if (n == 1) {
        cout << "\nMinimum fuel cost: 0.00\n";
        cout << "Total time: 0.00\n";
        cout << "Route: 1 -> 1\n";
        return 0;
    }

    vector<vector<double>> cost(n, vector<double>(n, INF));
    vector<vector<double>> travelTime(n, vector<double>(n, INF));

    cout << "Enter each road: from to distance fuel_cost_per_km time\n";

    for (int i = 0; i < roads; i++) {
        int from, to;
        double distance, rate, time;
        cin >> from >> to >> distance >> rate >> time;

        from--;
        to--;

        if (from < 0 || from >= n || to < 0 || to >= n ||
            from == to || distance < 0 || rate < 0 || time < 0) {
            cout << "Invalid road\n";
            return 1;
        }

        cost[from][to] = distance * rate;
        travelTime[from][to] = time;
    }

    vector<double> minOut(n, INF);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && cost[i][j] < minOut[i])
                minOut[i] = cost[i][j];
        }
    }

    // Optimistic cost of completing a partial route
    auto lowerBound = [&](const State& state) {
        double result = state.cost + minOut[state.last];

        for (int city = 0; city < n; city++) {
            if ((state.visited & (uint64_t(1) << city)) == 0)
                result += minOut[city];
        }

        return result;
    };

    // Priority queue explores the lowest-bound state first
    priority_queue<State, vector<State>, Compare> pq;

    State start{{0}, 1, 0, 0.0, 0.0, 0.0};
    start.bound = lowerBound(start);
    pq.push(start);

    double bestCost = INF;
    double bestTime = INF;
    vector<int> bestPath;

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.bound >= bestCost)
            continue;

        // All cities visited: return to the starting city
        if ((int)current.path.size() == n) {
            if (cost[current.last][0] == INF)
                continue;

            double totalTime =
                current.time + travelTime[current.last][0];
            double totalCost =
                current.cost + cost[current.last][0];

            if (totalTime <= timeLimit && totalCost < bestCost) {
                bestCost = totalCost;
                bestTime = totalTime;
                bestPath = current.path;
                bestPath.push_back(0);
            }
            continue;
        }

        for (int next = 1; next < n; next++) {
            if (current.visited & (uint64_t(1) << next))
                continue;
            if (cost[current.last][next] == INF)
                continue;

            double newTime =
                current.time + travelTime[current.last][next];

            if (newTime > timeLimit)
                continue;

            State child = current;
            child.path.push_back(next);
            child.visited |= uint64_t(1) << next;
            child.last = next;
            child.cost += cost[current.last][next];
            child.time = newTime;
            child.bound = lowerBound(child);

            if (child.bound < bestCost)
                pq.push(child);
        }
    }

    if (bestPath.empty()) {
        cout << "\nNo route visits every city and meets the time limit.\n";
        return 0;
    }

    cout << fixed << setprecision(2);
    cout << "\nMinimum fuel cost: " << bestCost << '\n';
    cout << "Total time: " << bestTime << '\n';
    cout << "Route: ";

    for (int i = 0; i < (int)bestPath.size(); i++) {
        if (i > 0)
            cout << " -> ";
        cout << bestPath[i] + 1;
    }
    cout << '\n';

    return 0;
}