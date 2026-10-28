
#include <bits/stdc++.h>
using namespace std;

const int N = 7;
const int INF = 1e9;

string place[N] = {
    "Rescue Center",
    "Residential Area A",
    "Residential Area B",
    "Main Junction",
    "Hospital",
    "Safe Shelter",
    "Residential Area C"
};

struct Road {
    int u, v, distance;
    bool open;
};

vector<Road> roads = {
    {0, 1, 4, true},
    {0, 2, 2, true},
    {2, 1, 1, true},
    {1, 3, 5, true},
    {2, 3, 8, true},
    {3, 4, 2, true},
    {1, 4, 10, true},
    {3, 5, 3, true},
    {5, 6, 2, true}
};

// Build the graph using only open roads
vector<vector<pair<int, int>>> buildGraph() {
    vector<vector<pair<int, int>>> adj(N);

    for (const Road &r : roads) {
        if (r.open) {
            adj[r.u].push_back({r.v, r.distance});
            adj[r.v].push_back({r.u, r.distance});
        }
    }

    return adj;
}

// Print a path
void printPath(vector<int> &parent, int destination) {
    vector<int> path;

    for (int v = destination; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());

    for (int i = 0; i < (int)path.size(); i++) {
        cout << place[path[i]];

        if (i + 1 < (int)path.size()) {
            cout << " -> ";
        }
    }
    cout << '\n';
}

// BFS: minimum number of roads
void BFS(int source, int destination) {
    auto adj = buildGraph();

    vector<bool> visited(N, false);
    vector<int> parent(N, -1);
    vector<int> level(N, -1);
    queue<int> q;

    visited[source] = true;
    level[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto edge : adj[u]) {
            int v = edge.first;

            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }

    if (!visited[destination]) {
        cout << "No available route!\n";
        return;
    }

    cout << "Route with minimum number of roads:\n";
    printPath(parent, destination);
    cout << "Number of roads: " << level[destination] << '\n';
}

// DFS: explore reachable locations
void DFS(int source) {
    auto adj = buildGraph();

    vector<bool> visited(N, false);
    stack<int> st;
    st.push(source);

    cout << "Locations reachable from "
         << place[source] << ":\n";

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (visited[u]) continue;

        visited[u] = true;
        cout << place[u] << '\n';

        for (auto edge : adj[u]) {
            int v = edge.first;

            if (!visited[v]) {
                st.push(v);
            }
        }
    }
}

// Dijkstra: minimum total distance
void dijkstra(int source, int destination) {
    auto adj = buildGraph();

    vector<int> dist(N, INF);
    vector<int> parent(N, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d != dist[u]) continue;

        for (auto edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destination] == INF) {
        cout << "No available route!\n";
        return;
    }

    cout << "Shortest safe route:\n";
    printPath(parent, destination);
    cout << "Total distance: " << dist[destination] << '\n';
}

// Disjoint Set Union for Kruskal
vector<int> dsuParent, dsuRank;

void initDSU() {
    dsuParent.resize(N);
    dsuRank.assign(N, 0);
    iota(dsuParent.begin(), dsuParent.end(), 0);
}

int findParent(int x) {
    if (dsuParent[x] == x) return x;
    return dsuParent[x] = findParent(dsuParent[x]);
}

bool unite(int a, int b) {
    a = findParent(a);
    b = findParent(b);

    if (a == b) return false;

    if (dsuRank[a] < dsuRank[b]) {
        swap(a, b);
    }

    dsuParent[b] = a;

    if (dsuRank[a] == dsuRank[b]) {
        dsuRank[a]++;
    }

    return true;
}

// Kruskal: minimum spanning tree
void kruskal() {
    vector<Road> available;

    for (const Road &r : roads) {
        if (r.open) available.push_back(r);
    }

    sort(available.begin(), available.end(),
         [](const Road &a, const Road &b) {
             return a.distance < b.distance;
         });

    initDSU();

    int totalCost = 0;
    int selected = 0;

    cout << "Minimum spanning network:\n";

    for (const Road &r : available) {
        if (unite(r.u, r.v)) {
            cout << place[r.u] << " -- "
                 << place[r.v] << " : "
                 << r.distance << '\n';

            totalCost += r.distance;
            selected++;
        }
    }

    if (selected != N - 1) {
        cout << "Warning: Network is disconnected.\n";
        cout << "Only a minimum spanning forest was found.\n";
    }

    cout << "Total connection distance: "
         << totalCost << '\n';
}

// Display all roads and their status
void showRoads() {
    cout << "\nRoad List:\n";

    for (int i = 0; i < (int)roads.size(); i++) {
        cout << "ID " << i << ": "
             << place[roads[i].u] << " -- "
             << place[roads[i].v]
             << " | Distance: " << roads[i].distance
             << " | Status: "
             << (roads[i].open ? "OPEN" : "FLOODED/BLOCKED")
             << '\n';
    }
}

// Toggle a road between open and blocked
void changeRoadStatus() {
    showRoads();

    int id;
    cout << "Enter road ID to toggle: ";
    cin >> id;

    if (id < 0 || id >= (int)roads.size()) {
        cout << "Invalid road ID!\n";
        return;
    }

    roads[id].open = !roads[id].open;

    cout << "Road status updated: "
         << (roads[id].open ? "OPEN" : "FLOODED/BLOCKED")
         << '\n';
}

int main() {
    int choice;

    do {
        cout << "\n===== FLOODSHIELD =====\n";
        cout << "0. Show all locations\n";
        cout << "1. Find route using BFS\n";
        cout << "2. Explore locations using DFS\n";
        cout << "3. Find shortest route using Dijkstra\n";
        cout << "4. Plan network using Kruskal\n";
        cout << "5. Show road status\n";
        cout << "6. Change road status\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int source, destination;

        switch (choice) {
            case 0:
                for (int i = 0; i < N; i++) {
                    cout << i << ". " << place[i] << '\n';
                }
                break;

            case 1:
                cout << "Enter source and destination IDs: ";
                cin >> source >> destination;

                if (source < 0 || source >= N ||
                    destination < 0 || destination >= N) {
                    cout << "Invalid location ID!\n";
                } else {
                    BFS(source, destination);
                }
                break;

            case 2:
                cout << "Enter starting location ID: ";
                cin >> source;

                if (source < 0 || source >= N) {
                    cout << "Invalid location ID!\n";
                } else {
                    DFS(source);
                }
                break;

            case 3:
                cout << "Enter source and destination IDs: ";
                cin >> source >> destination;

                if (source < 0 || source >= N ||
                    destination < 0 || destination >= N) {
                    cout << "Invalid location ID!\n";
                } else {
                    dijkstra(source, destination);
                }
                break;

            case 4:
                kruskal();
                break;

            case 5:
                showRoads();
                break;

            case 6:
                changeRoadStatus();
                break;

            case 7:
                cout << "Exiting FloodShield. Stay safe!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 7);

    return 0;
}
