#include "Header.h"
#include <chrono>

int main() { // использовалась модифицированная версия sort, с передаваемым параметром insertionSortThreshold
	const int n = 10000;

	double arr[n];
	double sortedArr[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = (std::rand() % 100000) * 0.1;
	}
	
	int min = -1;
	int min_param = -1;
	

	for (int i = 5; i < 100; ++i) {
		auto start = std::chrono::high_resolution_clock::now();
		sort(arr, arr + n, [](double a, double b) { return a < b; }, i);
		auto finish = std::chrono::high_resolution_clock::now();

		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
		auto ms = microseconds.count();
		std::cout << ms << "ms\n";

		if (min == -1 || min > ms) {
			min = ms;
			min_param = i;
		}
	}


	std::cout << "min_ms: " << min << ", min param: " << min_param << "\n"; // После нескольких проходов было установлено, что значения находятся в окрестности 90
}
