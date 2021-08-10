//
// Created by sadegh on 09.08.21.
//

#include <tuple>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <chrono>
#include <random>


std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

bool cmp_tuple_second_element(std::tuple<int, int> a, std::tuple<int, int> b) {
    return std::get<1>(a) < std::get<1>(b);
}

template <class T>
int partition(std::vector<T> &arr, int low, int high, std::function<bool(T, T)> cmp) {
    int random = low + (int)(rng() % (high - low + 1));

    swap(arr[random], arr[low]);

    T pivot = arr[low];

    int i = low - 1, j = high + 1;

    while (true) {

        do {
            i++;
        } while (cmp(arr[i], pivot) && i < high);

        do {
            j--;
        } while (!cmp(arr[j], pivot) && j > low);

        if (i >= j) {
            if(i == low && j == low)
                return -1;
            return i;
        }

        std::swap(arr[i], arr[j]);
    }
}

template <class T>
void quick_sort(std::vector<T> &arr, int low, int high, std::function<bool(T, T)> cmp) {

    while (low < high) {
        int pi = partition<T>(arr, low, high, cmp);

        if (pi == -1) {
            return;
        }
        if (pi - low < high - pi) {
            quick_sort<T>(arr, low, pi - 1, cmp);
            low = pi;
        } else {
            quick_sort<T>(arr, pi, high, cmp);
            high = pi - 1;
        }
    }

}

void sort_tuple(std::vector<std::tuple<int, int>> &list_of_tuples) {
    quick_sort<std::tuple<int, int>>(list_of_tuples, 0, (int) list_of_tuples.size() - 1, &cmp_tuple_second_element);
}

template <class T>
T get_median(std::vector<T> l) {
    std::nth_element(l.begin(), l.begin() + l.size() / 2, l.end());
    return l[l.size() / 2];
}
