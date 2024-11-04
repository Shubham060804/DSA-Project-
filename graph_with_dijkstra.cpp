#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <queue>
#include <limits>
class Graph
{
private:
    int vertices;                                // Number of vertices
    std::vector<std::vector<int>> adjacencyList; // Adjacency list
    std::vector<std::vector<int>> weights;       // Edge weights
public:
    Graph(int v) : vertices(v)
    {
        if (v <= 0)
        {
            throw std::invalid_argument("Number of vertices must be positive.");
        }
        adjacencyList.resize(v);
        weights.resize(v, std::vector<int>(v, 0)); // weight which initialize between two vertex starting wt. 0
    }
    void addEdge(int src, int dest, int weight)
    {
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices)
        {
            throw std::out_of_range("Vertex index out of range.");
        }
        adjacencyList[src].push_back(dest);
        adjacencyList[dest].push_back(src); // For undirected graph
        weights[src][dest] = weight;        // Set the weight of the edge
        weights[dest][src] = weight;        // Undirected graph: set both directions
    }
    void visualize(const std::string &filename)
    {
        std::ofstream outFile(filename);
        outFile << "graph G {\n";

        for (int i = 0; i < vertices; ++i)
        {
            for (int j : adjacencyList[i])
            {
                if (i < j)
                { // To avoid duplicate edges in undirected graph
                    outFile << "  " << i << " -- " << j << " [weight=\"" << weights[i][j] << "\"];\n";
                }
            }
        }
        outFile << "}\n";
        outFile.close();
        std::cout << "Graph visualized in " << filename << std::endl;
    }
    void printGraph()
    {
        for (int i = 0; i < vertices; ++i)
        {
            std::cout << i << ": ";
            for (int j : adjacencyList[i])
            {
                std::cout << j << "(" << weights[i][j] << ") ";
            }
            std::cout << std::endl;
        }
    }
    void dijkstra(int start)
    {
        std::vector<int> distance(vertices, std::numeric_limits<int>::max());
        std::vector<bool> visited(vertices, false);
        distance[start] = 0;

        using Pair = std::pair<int, int>; // (distance, vertex)
        std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
        pq.push({0, start});

        while (!pq.empty())
        {
            int current = pq.top().second;
            pq.pop();

            if (visited[current])
                continue;
            visited[current] = true;

            for (size_t i = 0; i < adjacencyList[current].size(); ++i)
            {
                int neighbor = adjacencyList[current][i];
                int edgeWeight = weights[current][neighbor];

                if (!visited[neighbor] && distance[current] + edgeWeight < distance[neighbor])
                {
                    distance[neighbor] = distance[current] + edgeWeight;
                    pq.push({distance[neighbor], neighbor});
                }
            }
        }
        std::cout << "Shortest paths from vertex " << start << ":\n";
        for (int i = 0; i < vertices; ++i)
        {
            if (distance[i] == std::numeric_limits<int>::max())
            {
                std::cout << "Vertex " << i << ": Infinity\n";
            }
            else
            {
                std::cout << "Vertex " << i << ": " << distance[i] << "\n";
            }
        }
    }
};
int main()
{
    int vertices = 5;
    Graph graph(vertices);
    // Adding edges with weights
    try
    {
        graph.addEdge(0, 1, 10);
        graph.addEdge(0, 2, 5);
        graph.addEdge(1, 2, 2);
        graph.addEdge(1, 3, 1);
        graph.addEdge(2, 1, 3);
        graph.addEdge(2, 3, 9);
        graph.addEdge(2, 4, 2);
        graph.addEdge(3, 4, 4);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    graph.printGraph();
    graph.visualize("graph.dot");
    graph.dijkstra(0);
    return 0;
}
