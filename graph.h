//
// Created by solale on 13.08.21.
//
#include <vector>
#include <string>

using namespace std;
#ifndef NCFINAL_GRAPH_H
#define NCFINAL_GRAPH_H

void connected_components(vector<vector<int>> &components);

void calculate_motifs(int cutoff, const string &file_path);

#endif //NCFINAL_GRAPH_H
