//
// Created by mahdi on 13.08.21.
//

#include "effective_hashtags.h"
#include <fstream>
#include <unordered_map>


int insert_to_strings(int &id, std::unordered_map<std::string, int> &unique_strings_set,
                      std::vector<std::string> &unique_strings, const std::string &str) {
    if (unique_strings_set.find(str) == unique_strings_set.end()) {
        unique_strings.emplace_back(str);
        unique_strings_set[str] = id++;
    }
    return unique_strings_set[str];
}


std::pair<std::vector<std::vector<int>>, std::vector<std::string>>
get_effective_hashtags(int n, char *hashtags_address) {
    std::ifstream in(hashtags_address);
    std::vector<std::vector<int>> result(n);
    std::unordered_map<std::string, int> unique_strings_set;
    std::vector<std::string> unique_strings;
    int id = 0;
    for(int i = 0; i < n; i++) {
        std::string hashtags;
        in >> hashtags;
        int last = -1;
        while (true) {
            auto next = hashtags.find(',', last + 1);
            if (next == std::string::npos) {
                result[i].push_back(
                        insert_to_strings(
                                id,
                                unique_strings_set,
                                unique_strings,
                                hashtags.substr(last + 1)
                        )
                );
                break;
            }
            result[i].push_back(
                    insert_to_strings(
                            id,
                            unique_strings_set,
                            unique_strings,
                            hashtags.substr(last + 1, next - last - 1)
                    )
            );
            last = (int) next;
        }
    }
    in.close();
    unique_strings_set.clear();
    return std::make_pair(result, unique_strings);
}
