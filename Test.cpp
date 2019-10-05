#include "./Header.h"
#include "./Lab1.cpp"

TEST(SortTest, ArraySorted) { // Проверить, что отсортированный массив не испортится
	const int n = 20;

	int arr[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = i;
	}

	int unmodified_arr[n];
	std::copy(arr, arr + n, unmodified_arr);

	sort(arr, arr + n, [](int a, int b) { return a < b; });
	
	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(unmodified_arr[i] == arr[i]) << arr[i];
	}
}

TEST(SortTest, ArraySortedBackwards) { // Массив, отсортированный в обратном порядке
	const int n = 20;

	int arr[n];
	int sortedArr[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = n - i - 1;
		sortedArr[i] = i;
	}

	sort(arr, arr + n, [](int a, int b) { return a < b; });

	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(sortedArr[i] == arr[i]) << arr[i];
	}
}

TEST(SortTest, ChangedSortingFunction) { // Проверка того, что переданная лямбда работает
	const int n = 20;

	int arr[n];
	int sortedArr[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = i;
		sortedArr[i] = n - i - 1;
	}

	sort(arr, arr + n, [](int a, int b) { return a > b; });

	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(sortedArr[i] == arr[i]) << arr[i];
	}
}

TEST(SortTest, ArraySimilarNumbers) { // Массив с одинаковыми элементами
	const int n = 20;

	int arr[n];
	int librarySortedArray[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = i % 2;
	}

	sort(arr, arr + n, [](int a, int b) { return a < b; });

	std::copy(arr, arr + n, librarySortedArray);
	std::sort(std::begin(librarySortedArray), std::end(librarySortedArray));

	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(librarySortedArray[i] == arr[i]) << arr[i];
	}
}

TEST(SortTest, GeneralCase) { // Общий случай
	const int n = 1000;

	double arr[n];
	double sortedArr[n];

	for (size_t i = 0; i < n; i++)
	{
		arr[i] = (std::rand() % 10000) * 0.1;
	}

	double librarySortedArray[n];
	std::copy(arr, arr + n, librarySortedArray);

	sort(arr, arr + n, [](double a, double b) { return a < b; });
	std::sort(std::begin(librarySortedArray), std::end(librarySortedArray));

	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(librarySortedArray[i] == arr[i]) << arr[i];
	}
}
