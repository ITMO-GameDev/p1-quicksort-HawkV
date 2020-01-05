#include "Containers.cpp"

TEST(ContainerTest, Insert) { // Вставка в контейнер
	Array<int> array;
  const size_t n = 10;

	for (size_t i = 0; i < n; i++)
	{
		array.insert(i);
	}
  
	for (size_t i = 0; i < n; i++)
	{
		EXPECT_TRUE(array[i] == i) << array[i];
	}
  
  EXPECT_TRUE(array.size() == 10);
}

TEST(ContainerTest, Removal) { // Удаление из контейнера
	Array<int> array;
  const size_t n = 10;

	for (size_t i = 0; i < n; i++)
	{
		array.insert(i);
	}
  
  array.remove(0);
 
  EXPECT_TRUE(array[0] == 1) << array[0];
  EXPECT_TRUE(array.size() == 9);
}

TEST(ContainerTest, Iterator) { // Проверка итератора
	Array<int> array;
  const size_t n = 10;

	for (size_t i = 0; i < n; i++)
	{
		array.insert(i);
	}
  
  auto it = array.iterator();
  
  EXPECT_TRUE(it.get() == 0) << it.get();
  EXPECT_TRUE(it.hasNext());
  EXPECT_TRUE(!it.hasPrev());
  
  it.set(n);
  EXPECT_TRUE(it.get() == n) << it.get();
  
  it.next();
  EXPECT_TRUE(it.get() == 1) << it.get();
  EXPECT_TRUE(it.hasPrev());
  EXPECT_TRUE(it.hasNext());
  
  it.insert(15);
  EXPECT_TRUE(it.get() == 15) << it.get();
  EXPECT_TRUE(array.size() == (n + 1));
  
  it.next();
  it.remove();
  EXPECT_TRUE(it.get() == 2) << it.get();
  EXPECT_TRUE(array.size() == n);
}
