#include <iostream>
#include <climits>
using namespace std;

const int MAX = 1000;
const int INF = INT_MAX;

int graph[MAX][MAX];
int distanceArr[MAX];
int parent[MAX];
bool visited[MAX];
int vertices;

int findMinDistance()
{
    int minDistance = INF;
    int minIndex = -1;

    for (int i = 0; i < vertices; i++)
    {
        if (!visited[i] && distanceArr[i] < minDistance)
        {
            minDistance = distanceArr[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void printPath(int node)
{
    if (node == -1)
        return;

    printPath(parent[node]);
    cout << node << " ";
}

void dijkstra(int source, int destination)
{
    for (int i = 0; i < vertices; i++)
    {
        distanceArr[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    distanceArr[source] = 0;

    for (int count = 0; count < vertices - 1; count++)
    {
        int u = findMinDistance();

        if (u == -1)
            break;

        visited[u] = true;

        if (u == destination)
            break;

        for (int v = 0; v < vertices; v++)
        {
            if (!visited[v] &&
                graph[u][v] != INF &&
                distanceArr[u] != INF &&
                distanceArr[u] + graph[u][v] < distanceArr[v])
            {
                distanceArr[v] = distanceArr[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    cout << "\nQuickest Ambulance Route\n";
    cout << "------------------------\n";

    if (distanceArr[destination] == INF)
    {
        cout << "No route available.\n";
    }
    else
    {
        cout << "Ambulance Source: " << source << endl;
        cout << "Hospital Destination: " << destination << endl;
        cout << "Minimum Travel Time: " << distanceArr[destination] << " minutes" << endl;
        cout << "Optimal Route: ";
        printPath(destination);
        cout << endl;
    }
}

int main()
{
    int roads;
    int source, destination;

    cout << "Smart Traffic Management for Emergency Vehicles\n";
    cout << "Enter number of intersections: ";
    cin >> vertices;

    if (vertices > MAX)
    {
        cout << "Maximum " << MAX << " intersections allowed!\n";
        return 0;
    }

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    cout << "Enter number of roads: ";
    cin >> roads;

    cout << "Enter Junction1 Junction2 TravelTime:\n";

    for (int i = 0; i < roads; i++)
    {
        int u, v, time;

        cin >> u >> v >> time;

        if (u < 0 || u >= vertices ||
            v < 0 || v >= vertices ||
            time < 0)
        {
            cout << "Invalid input!\n";
            i--;
            continue;
        }

        graph[u][v] = time;
        graph[v][u] = time;
    }

    cout << "Enter ambulance source junction: ";
    cin >> source;

    cout << "Enter hospital destination junction: ";
    cin >> destination;

    dijkstra(source, destination);

    char update;

    cout << "\nDo you want to update traffic conditions? (Y/N): ";
    cin >> update;

    while (update == 'Y' || update == 'y')
    {
        int u, v, newTime;

        cout << "Enter road to update: ";
        cin >> u >> v;

        cout << "Enter new travel time: ";
        cin >> newTime;

        graph[u][v] = newTime;
        graph[v][u] = newTime;

        cout << "Traffic condition updated successfully!\n";

        dijkstra(source, destination);

        cout << "\nDo you want to update another road? (Y/N): ";
        cin >> update;
    }

    cout << "\nEmergency Route Optimization Completed Successfully!\n";

    return 0;
}