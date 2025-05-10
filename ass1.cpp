#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
private:
    int numVertices;
    vector<vector<int>> adj;

public:
    Graph(int vertices) : numVertices(vertices), adj(vertices) {}

    void addEdge(int src, int dest) {
        adj[src].push_back(dest);
        adj[dest].push_back(src); // undirected graph
    }

    void viewGraph() {
        cout << "Graph:\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << " -> ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Parallel BFS using OpenMP (level-wise)
    void bfs(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> nextLevel;

            #pragma omp parallel for
            for (int i = 0; i < levelSize; ++i) {
                int currentVertex;

                #pragma omp critical
                {
                    if (!q.empty()) {
                        currentVertex = q.front();
                        q.pop();
                        cout << currentVertex << " ";
                    }
                }

                for (int neighbor : adj[currentVertex]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            nextLevel.push_back(neighbor);
                        }
                    }
                }
            }

            for (int v : nextLevel)
                q.push(v);
        }
    }

    // Sequential DFS
    void dfs(int startVertex) {
        vector<bool> visited(numVertices, false);
        stack<int> s;
        s.push(startVertex);
        visited[startVertex] = true;

        while (!s.empty()) {
            int currentVertex = s.top();
            s.pop();
            cout << currentVertex << " ";

            for (int neighbor : adj[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    s.push(neighbor);
                }
            }
        }
    }
};

int main() {
    int numVertices, numEdges;
    cout << "Enter the number of vertices in the graph: ";
    cin >> numVertices;

    Graph graph(numVertices);

    cout << "Enter the number of edges in the graph: ";
    cin >> numEdges;

    cout << "Enter the edges (source destination):\n";
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    graph.viewGraph();

    int startVertex;
    cout << "Enter the starting vertex for BFS and DFS: ";
    cin >> startVertex;

    cout << "Breadth First Search (BFS): ";
    graph.bfs(startVertex);
    cout << endl;

    cout << "Depth First Search (DFS): ";
    graph.dfs(startVertex);
    cout << endl;

    return 0;
}
