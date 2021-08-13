//
// Created by mahdi on 13.08.21.
//

#include "effective_hashtags.h"
#include <fstream>


std::vector<std::vector<std::string>> get_effective_hashtags(int n, char* hashtags_address) {
    std::ifstream in(hashtags_address);
    std::vector<std::vector<std::string>> result(n);
    for(int i = 0; i < n; i++) {
        std::string hashtags;
        in >> hashtags;
        int last = -1;
        while(true) {
            auto next = hashtags.find(',', last + 1);
            if(next == std::string::npos) {
                result[i].emplace_back(hashtags.substr(last + 1));
                break;
            }
            result[i].emplace_back(hashtags.substr(last + 1, next - last - 1));
            last = (int)next;
        }
    }
    return result;
}
