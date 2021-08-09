//
// Created by sadegh on 09.08.21.
//

#include <tuple>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "utils.h"

int partition(std::vector<std::tuple<int, int>> &list_of_tuples, unsigned long low, unsigned long high) {
//    srand(time(nullptr));
//    int random = low + rand() % (high - low);
//
//    swap(list_of_tuples[random], list_of_tuples[low]);

    int pivot = std::get<1>(list_of_tuples[low]);

    unsigned long i = low - 1, j = high + 1;

    while (true) {

        do {
            i++;
        } while (std::get<1>(list_of_tuples[i]) < pivot);

        do {
            j--;
        } while (std::get<1>(list_of_tuples[j]) > pivot);

        if (i >= j)
            return j;

        std::swap(list_of_tuples[i], list_of_tuples[j]);
    }
}


void quickSort(std::vector<std::tuple<int, int>> &arr, unsigned long low, unsigned long high) {

    while (low < high) {
        int pi = partition(arr, low, high);

        if (pi - low < high - pi) {
            quickSort(arr, low, pi - 1);
            low = pi + 1;
        } else {
            quickSort(arr, pi + 1, high);
            high = pi - 1;
        }
    }

}

void sort_tuple(std::vector<std::tuple<int, int>> &list_of_tuples) {
    quickSort(list_of_tuples, 0, list_of_tuples.size() - 1);
}

double get_median(std::vector<double> l) {
    std::nth_element(l.begin(), l.begin() + l.size() / 2, l.end());
    return l[l.size() / 2];
}


