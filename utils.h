//
// Created by sadegh on 09.08.21.
//

#ifndef NCFINAL_UTILS_H
#define NCFINAL_UTILS_H
#include <vector>
#include <tuple>

void sort_tuple(std::vector<std::tuple<int, int>>& list_of_tuples);

template <class T>
T get_median(std::vector<T> l);

#endif //NCFINAL_UTILS_H
