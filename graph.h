//
// Created by solale on 13.08.21.
//

#ifndef NCFINAL_GRAPH_H

#include <vector>
#include <string>

#define NCFINAL_GRAPH_H

class Graph {
};

Graph generate_graph(int n, const std::vector<std::tuple<int, int, double>>& tweet_edges,
                     const std::vector<std::vector<int>>& hashtags, int unique_hashtags_size);

#endif //NCFINAL_GRAPH_H
