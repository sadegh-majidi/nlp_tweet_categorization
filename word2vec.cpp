//
// Created by mahdi on 09.08.21.
//

#include "word2vec.h"
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include "global_constants.h"

static std::unordered_map<std::string, int> mp;

const int maxn = 2e6 + 20;
const int cache_limit = 1e5 + 20;

std::streamoff offset[maxn];
std::vector<double> results[cache_limit];

int n;

void get_vector_from_line(const std::string& l, std::vector<double> *result) {
    int last = (int)l.find(' ');
    for (int j = 0; j < VECTOR_COLUMNS; j++) {
        auto new_last = (j + 1 < VECTOR_COLUMNS ? l.find(' ', last + 1) : l.length());
        result->push_back(std::stod(l.substr(last + 1, new_last)));
        last = (int) new_last;
    }
}

void word2vec_setup() {
    std::ifstream f(word2vec_crawl_file_path);
    int _;
    f >> n >> _;
    std::string l;
    std::getline(f, l);
    std::streamoff last = 0;
    for(int i = 0; i < n; i++) {
        std::getline(f, l);
        mp[l.substr(0, l.find(' '))] = i;
        offset[i] = last;
        last = f.tellg();
        if(i < cache_limit)
            get_vector_from_line(l, &results[i]);
    }
    f.close();
}

std::vector<double> get_vector(const std::string& word) {
    if(mp.empty())
        word2vec_setup();
    if(mp.find(word) == mp.end())
        return {};
    int index = mp[word];
    if(index < cache_limit)
        return results[index];
    std::ifstream f(word2vec_crawl_file_path);
    f.seekg(offset[index]);
    std::string l;
    std::getline(f, l);
    f.close();
    std::vector<double> result;
    get_vector_from_line(l, &result);
    return result;
}
