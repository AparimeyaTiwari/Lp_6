#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <omp.h>

using namespace std;

class Graph {

private:
    int V;
    vector<vector<int>> adj;

public:

    Graph(int vertices) {

        V = vertices;

        adj.resize(vertices);
    }

    void addEdge(int v1, int v2) {

        adj[v1].push_back(v2);

        adj[v2].push_back(v1);
    }

    void parallelBFS(int start) {

        vector<bool> visited(V, false);

        queue<int> q;

        visited[start] = true;

        q.push(start);

        cout << "Parallel BFS starting from vertex "
             << start << ": ";

        while (!q.empty()) {

            int levelSize = q.size();

            vector<int> currLevel;

            for (int i = 0; i < levelSize; i++) {

                currLevel.push_back(q.front());

                q.pop();
            }

            #pragma omp parallel for schedule(dynamic)

            for (int i = 0; i < currLevel.size(); i++) {

                int vertex = currLevel[i];

                #pragma omp critical
                {
                    cout << vertex << " ";
                }

                for (int neighbour : adj[vertex]) {

                    bool wasNotVisited = false;

                    #pragma omp critical
                    {
                        if (!visited[neighbour]) {

                            visited[neighbour] = true;

                            wasNotVisited = true;
                        }
                    }

                    if (wasNotVisited) {

                        #pragma omp critical
                        {
                            q.push(neighbour);
                        }
                    }
                }
            }
        }

        cout << endl;
    }
};

int main() {

    Graph g1(8);

    g1.addEdge(0, 1);
    g1.addEdge(0, 2);
    g1.addEdge(1, 3);
    g1.addEdge(1, 4);
    g1.addEdge(2, 5);
    g1.addEdge(2, 6);
    g1.addEdge(6, 7);

    // Set number of threads
    omp_set_num_threads(4);

    g1.parallelBFS(0);

    return 0;
}