//
// Created by sadegh on 17.08.21.
//

#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include "export_data.h"

void export_processed_data(const char *output_address, int n, const std::vector<std::tuple<int, int, double>>& tweet_edges,
                           const std::vector<std::vector<int>>& hashtags, const std::vector<std::string>& unique_hashtags) {
    std::string graph_output_file = (std::string)output_address + ".graph";
    std::string unique_hashtags_output_file = (std::string)output_address + ".unique_hashtags";

    std::ofstream outfile(graph_output_file);
    outfile << n + (int) unique_hashtags.size() << std::endl;

    for (auto &edge : tweet_edges)
        outfile << std::get<0>(edge) << " " << std::get<1>(edge) << " " << 2 - std::get<2>(edge) << std::endl;

    for (int i = 0; i < n; i++)
        for (int hashtag_id : hashtags[i])
            outfile << i << " " << n + hashtag_id << " " << 1 << std::endl;

    outfile.close();

    std::ofstream hashtags_outfile(unique_hashtags_output_file);
    hashtags_outfile << (int) unique_hashtags.size() << std::endl;

    for (auto &hashtag : unique_hashtags)
        hashtags_outfile << hashtag << std::endl;

    hashtags_outfile.close();
}
