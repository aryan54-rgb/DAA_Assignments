#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

struct Edge {
    int from, to;
    double cost;
    bool open;
};

int main() {
    int stages;
    cin >> stages;

    vector<int> stageCount(stages);
    vector<vector<int>> nodesInStage(stages);
    vector<int> stageOf;

    int totalNodes = 0;

    // Nodes are numbered consecutively, starting from stage 0.
    for (int stage = 0; stage < stages; ++stage) {
        cin >> stageCount[stage];

        for (int j = 0; j < stageCount[stage]; ++j) {
            nodesInStage[stage].push_back(totalNodes);
            stageOf.push_back(stage);
            ++totalNodes;
        }
    }

    int edgeCount;
    cin >> edgeCount;

    vector<Edge> edges;
    vector<vector<int>> outgoing(totalNodes);

    // Each edge gets an ID: 0, 1, 2, ... in input order.
    for (int id = 0; id < edgeCount; ++id) {
        int from, to;
        double cost;
        cin >> from >> to >> cost;

        if (from < 0 || from >= totalNodes ||
            to < 0 || to >= totalNodes ||
            stageOf[to] != stageOf[from] + 1) {
            cerr << "Invalid edge: edges must connect consecutive stages.\n";
            return 1;
        }

        edges.push_back({from, to, cost, true});
        outgoing[from].push_back(id);
    }

    int operations;
    cin >> operations;

    cout << fixed << setprecision(2);

    while (operations--) {
        string command;
        cin >> command;

        if (command == "QUERY") {
            int source, destination;
            cin >> source >> destination;

            if (source < 0 || source >= totalNodes ||
                destination < 0 || destination >= totalNodes ||
                stageOf[source] != 0 ||
                stageOf[destination] != stages - 1) {
                cout << "Invalid source or destination\n";
                continue;
            }

            const double INF = numeric_limits<double>::infinity();
            vector<double> best(totalNodes, INF);
            vector<int> nextNode(totalNodes, -1);

            best[destination] = 0;

            // Work backward from the final stage.
            for (int stage = stages - 2; stage >= 0; --stage) {
                for (int node : nodesInStage[stage]) {
                    for (int edgeId : outgoing[node]) {
                        const Edge& edge = edges[edgeId];

                        if (!edge.open || best[edge.to] == INF) {
                            continue;
                        }

                        double candidate = edge.cost + best[edge.to];

                        if (candidate < best[node]) {
                            best[node] = candidate;
                            nextNode[node] = edge.to;
                        }
                    }
                }
            }

            if (best[source] == INF) {
                cout << "No valid route\n";
                continue;
            }

            cout << "Minimum cost: " << best[source] << '\n';
            cout << "Route: ";

            int node = source;
            while (node != -1) {
                cout << node;
                node = nextNode[node];

                if (node != -1) {
                    cout << " -> ";
                }
            }
            cout << '\n';
        }
        else if (command == "UPDATE") {
            int edgeId;
            double newCost;
            cin >> edgeId >> newCost;

            if (edgeId < 0 || edgeId >= edgeCount) {
                cout << "Invalid edge ID\n";
            } else {
                edges[edgeId].cost = newCost;
            }
        }
        else if (command == "CLOSE") {
            int edgeId;
            cin >> edgeId;

            if (edgeId < 0 || edgeId >= edgeCount) {
                cout << "Invalid edge ID\n";
            } else {
                edges[edgeId].open = false;
            }
        }
        else if (command == "OPEN") {
            int edgeId;
            cin >> edgeId;

            if (edgeId < 0 || edgeId >= edgeCount) {
                cout << "Invalid edge ID\n";
            } else {
                edges[edgeId].open = true;
            }
        }
    }

    return 0;
}