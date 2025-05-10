

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int v) : V(v), adj(v) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    void display() {
        for (int i = 0; i < V; ++i) {
            cout << "Vertex " << i << ": ";
            for (int nbr : adj[i])
                cout << nbr << " ";
            cout << "\n";
        }
    }

    void bfs(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int sz = q.size();
            vector<int> next;

            //pragma omp parallel for
            for (int i = 0; i < sz; ++i) {
                int node;
                //pragma omp critical
                {
                    if (!q.empty()) {
                        node = q.front();
                        q.pop();
                        cout << node << " ";
                    }
                }

                for (int nbr : adj[node]) {
                    //pragma omp critical
                    {
                        if (!visited[nbr]) {
                            visited[nbr] = true;
                            next.push_back(nbr);
                        }
                    }
                }
            }

            for (int v : next) q.push(v);
        }
    }

    void dfs(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int node = s.top(); s.pop();
            cout << node << " ";
            for (int nbr : adj[node]) {
                if (!visited[nbr]) {
                    visited[nbr] = true;
                    s.push(nbr);
                }
            }
        }
    }
};

int main() {
    int V, E;
    cout << "Vertices: "; cin >> V;
    Graph g(V);

    cout << "Edges: "; cin >> E;
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.display();

    int start;
    cout << "Start Vertex: "; cin >> start;

    cout << "BFS: "; g.bfs(start); cout << "\n";
    cout << "DFS: "; g.dfs(start); cout << "\n";
    return 0;
}
