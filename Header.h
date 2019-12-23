#pragma once
#include <iostream>
#include <algorithm>

template <typename T>
T *median(T *a, T *b, T *c);

template <typename T, typename Compare>
void insertion_sort(T *first, T *last, Compare&& lambda);

template <typename T, typename Compare>
int partition(T *first, T *last, T* pivot, Compare&& lambda);

template <typename T, typename Compare>
void sort(T *first, T *last, Compare&& lambda);
