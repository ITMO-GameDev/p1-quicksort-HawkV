#include "Header.h"

template <typename T>
T *median(T *a, T *b, T *c) { // Нахождение медианы
	if (a < b)
	{
		if (a >= c)
			return a;
		else if (b < c)
			return b;
	}
	else
	{
		if (a < c)
			return a;
		else if (b >= c)
			return b;
	}
	return c;
}

template <typename T, typename Compare>
void insertion_sort(T *first, T *last, Compare&& lambda) { // Сортировка вставками
	size_t length = last - first;
	for (size_t i = 0; i < length; i++)
	{
		for (size_t j = i; j > 0 && !lambda(*(first + j - 1), *(first + j)); j--)
		{
			std::swap(*(first + j - 1), *(first + j));
		}
	}
}

template <typename T, typename Compare>
int partition(T *first, T *last, T* pivot, Compare&& lambda) { // Разбиение Ломуто
	size_t length = last - first;
	size_t i = 0;
	
	T pivotValue = *pivot;

	for (size_t j = 0; j < length - 1; j++)
	{
		if (!lambda(pivotValue, *(first + j))) {
			std::swap(*(first + i), *(first + j));

			i++;
		}
	}

	return i;
}



template <typename T, typename Compare>
void sort(T *first, T *last, Compare&& lambda) { // Основной алгоритм сортировки
	size_t length = last - first;

	while (length > 1) {
		if (length < 90) { // Найденный экспериментально параметр
			insertion_sort(first, last, lambda);

			return;
		}

		T *middle = first + length / 2;
		T *medianValue = median(first, middle, (last - 1));

		int pivotIndex = partition(first, last, medianValue, lambda);

		if (pivotIndex > length - pivotIndex) {
			sort(first + pivotIndex + 1, last, lambda);

			last = first + pivotIndex;
		}
		else {
			sort(first, first + pivotIndex, lambda);

			first += pivotIndex + 1;
		}

		length = last - first;
	}
}
