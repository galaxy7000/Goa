#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

void parallel_bfs(int start, vector<vector<int>>& graph, vector<int>& visited, int n) {
    queue<int> q;
    q.push(start);
    visited[start] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            if (graph[v][i]) {
                #pragma omp critical
                {
                    if (!visited[i]) {
                        visited[i] = 1;
                        q.push(i);
                    }
                }
            }
        }
    }
}

int main() {
    int n, edges, u, v, start;
    cout << "Enter number of vertices: ";
    cin >> n;

    vector<vector<int>> graph(n, vector<int>(n, 0));
    vector<int> visited(n, 0);

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < edges; i++) {
        cin >> u >> v;
        graph[u][v] = graph[v][u] = 1;
    }

    cout << "Enter starting vertex: ";
    cin >> start;

    cout << "Parallel BFS Traversal: ";
    parallel_bfs(start, graph, visited, n);
    cout << endl;

    return 0;
}
