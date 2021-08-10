//
// Created by sadegh on 09.08.21.
//

#include <tuple>
#include <vector>
#include "utils.h"

bool cmp_tuple_second_element(std::tuple<int, int> a, std::tuple<int, int> b) {
    return std::get<1>(a) < std::get<1>(b);
}

void sort_tuple(std::vector<std::tuple<int, int>> &list_of_tuples) {
    quick_sort<std::tuple<int, int>>(list_of_tuples, 0, (int) list_of_tuples.size() - 1, &cmp_tuple_second_element);
}

