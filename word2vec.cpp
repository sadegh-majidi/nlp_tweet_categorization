//
// Created by mahdi on 09.08.21.
//

#include "word2vec.h"
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

std::unordered_map<std::string, int> mp;

const char* crawl_file_path = "/hdd/crawl-300d-2M.vec";

const int cache_limit = 2e5;

std::vector<double> results[cache_limit];

int last_cached_line_position, n, m;

std::pair<std::string, std::vector<double>> get_line(int m, const std::string& l) {
    std::string word = l.substr(0, l.find(' '));
    int last = (int)word.length();
    std::vector<double> result;
    for (int j = 0; j < m; j++) {
        auto new_last = (j + 1 < m ? l.find(' ', last + 1) : l.length());
        result.push_back(std::stod(l.substr(last + 1, new_last)));
        last = (int) new_last;
    }
    return {word, result};
}

void word2vec_setup() {
    std::ifstream f(crawl_file_path);
    f >> n >> m;
    std::string l;
    std::getline(f, l);
    for(int i = 0; i < n && i < cache_limit; i++) {
        std::getline(f, l);
        auto [word, result] = get_line(m, l);
        results[i] = std::move(result);
        mp[word] = i;
    }
    last_cached_line_position = (int)f.tellg();
    std::getline(f, l);
    std::cout << l << std::endl;
    f.close();
}

std::vector<double> get_vector(std::string word) {
    if(mp.empty())
        word2vec_setup();
    if(mp.find(word) != mp.end()) {
        return results[mp[word]];
    }
    std::ifstream f(crawl_file_path);
    f.seekg(last_cached_line_position);
    std::string l;
    for (int i = 0; i < n; i++) {
        std::getline(f, l);
        bool equal = true;
        for (int j = 0; j < (int) word.length() && equal; j++)
            equal &= (word[j] == l[j]);
        if (!equal || l[word.length()] != ' ')
            continue;
        f.close();
        return get_line(m, l).second;
    }
    return {};
}
