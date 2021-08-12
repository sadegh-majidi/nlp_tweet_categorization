#include <iostream>
#include "process_raw_data.h"
#include <sys/stat.h>
#include <thread>
#include <vector>
#include "word2vec.h"
#include "global_constants.h"

using namespace std;

int main() {
    cout << "cleaning " << sizeof(raw_data_file_names) / sizeof(raw_data_file_names[0]) << " files ..." << endl;
    vector<thread> v;
    for (auto[file_path, dest_path] : raw_data_file_names) {
        auto absolute_file_path = new string((string) absolute_path_to_resources + file_path);
        auto absolute_dest_path = new string((string) absolute_path_to_resources + dest_path);
        struct stat info{};
        if (stat((*absolute_dest_path).data(), &info) != 0 || info.st_mode & S_IFDIR) {
            thread th(process_raw_data, (*absolute_file_path).data(), (*absolute_dest_path).data());
            v.push_back(move(th));
        }
    }
    word2vec_setup();
    for(auto &th : v)
        th.join();
    cout << "done cleaning" << endl;
    return 0;
}
