//
// Created by solale on 13.08.21.
//

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>


std::vector<int> intersection(std::vector<int> v1,
                              std::vector<int> v2) {
    std::vector<int> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          back_inserter(v3));
    return v3;
}

class Edge {
    int u;
    int v;
    double w;

public:
    Edge(int u, int v, int w) : u(u), v(v), w(w) {}

    [[nodiscard]] int get_u() const {
        return u;
    }

    [[nodiscard]] int get_v() const {
        return v;
    }

    [[nodiscard]] double get_w() const {
        return w;
    }
};

class Graph {
    //array of vectors to store adjacency list
    std::vector<Edge> edges;
    int vertices;
    std::vector<std::vector<int>> adjacency;

public:
    explicit Graph(int n) : vertices(n) {
        for (int i = 0; i < vertices; ++i) {
            std::vector<int> d;
            adjacency.push_back(d);
        }
//        adjacency = new std::vector<int>[vertices]; //dynamic allocation
    }

    void add_edge(int u, int v, int w) {
        edges.emplace_back(u, v, w);
        adjacency[u].push_back(v);
        adjacency[v].push_back(u);
    }

    [[nodiscard]] int overlap(int u, int v) const {
        return intersection(adjacency[u], adjacency[v]).size();
    }

    void calculate_motifs(int cutoff, const std::string &file_path) {
        std::ofstream output_file(file_path);
        std::vector<Edge> filtered_edges;
        for (auto &edge : edges) {
            if (overlap(edge.get_u(), edge.get_v()) > cutoff) {
                filtered_edges.push_back(edge);
            }
        }
        output_file << vertices << " " << filtered_edges.size() << '\n';
        for (auto &filtered_edge : filtered_edges) {
            output_file << filtered_edge.get_u() << " " << filtered_edge.get_v() << " "
                        << filtered_edge.get_w() << '\n';
        }
        output_file.close();
    }

    void connected_components(std::vector<std::vector<int>> &components) {
        std::vector<bool> visited;
        visited.resize(vertices, false);
        int c=0;
        for (int i = 0; i < vertices; ++i) {
            if (!visited[i]) {
                std::vector<int> component;
                components.push_back(component);
                find_component_recursive(i, visited, components, c);
                c++;
            }
        }

    }

    void find_component_recursive(int i, std::vector<bool> &visited, std::vector<std::vector<int>> &components, int c) {
        if (visited[i])
            return;
        visited[i] = true;
        components[c].push_back(i);
        for (int j : adjacency[i]) {
            find_component_recursive(j, visited, components, c);
        }

    }

};
