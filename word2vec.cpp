//
// Created by mahdi on 09.08.21.
//

#include "word2vec.h"
#include <vector>
#include <string>
#include <fstream>

std::vector<double> get_vector(std::string word)
{
    std::ifstream f("/hdd/crawl-300d-2M.vec");
    int n, m;
    f >> n >> m;
    std::string l;
    while(true)
    {
        f >> l;
        std::vector<double> v;
        for(int i = 0; i < m; i++)
        {
            double tmp;
            f >> tmp;
            v.push_back(tmp);
        }
        if(l == word) {
            f.close();
            return v;
        }
    }
}
