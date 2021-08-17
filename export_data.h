//
// Created by sadegh on 17.08.21.
//

#ifndef NCFINAL_EXPORT_DATA_H
#define NCFINAL_EXPORT_DATA_H

#include <vector>
#include <string>

void export_processed_data(const char *output_address, int n, const std::vector<std::tuple<int, int, double>>& tweet_edges,
                           const std::vector<std::vector<int>>& hashtags, const std::vector<std::string>& unique_hashtags);

#endif //NCFINAL_EXPORT_DATA_H
