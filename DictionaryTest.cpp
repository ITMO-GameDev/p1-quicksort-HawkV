#include "Dictionary.cpp"

TEST(DictionaryTest, Insert) { //вставка значений в словарь
  Dictionary<int, int> dict;

	dict.put(6, 1);
	dict.put(4, 7);
	dict.put(3, 3);
  
  EXPECT_TRUE(dict[4] == 7) << dict[4];
  EXPECT_TRUE(dict.size() == 3) << dict.size();
  
  dict[9] = 12;
  
  EXPECT_TRUE(dict[9] == 12) << dict[9]; // вставка с использованием оператора []
  EXPECT_TRUE(dict[50] == 0) << dict[50]; // значение по умолчанию
  
  EXPECT_TRUE(dict.size() == 5) << dict.size();
}

TEST(DictionaryTest, Iterator) { //вставка значений в дерево и проход по нему (inorder)
  Dictionary<int, std::string> dict;

	dict.put(5, "a");
	dict.put(2, "b");
	dict.put(3, "c");
	dict.put(1, "d");
	dict.put(4, "e");
  
  int i = 1;
  auto it = dict.iterator();
  
  for (; it.hasNext(); it.next())
	{
	    EXPECT_TRUE(it.key() == i) << it.key();
  
      if(it.key() == 5) {
        it.set("hello");
      }
    
      i++;
  }
  
  EXPECT_TRUE(it.get() == "hello"); //последний элемент итератора было решено делать не фиктивным
}

TEST(DictionaryTest, Delete) { //удаление значений из словаря
  Dictionary<std::string, int> dict;

	dict.put("health", 1);
	dict.put("armour", 2);
	dict.put("attack", 3);  
           
  EXPECT_TRUE(dict.contains("health"));
  EXPECT_TRUE(dict["health"] == 1) << dict["health"];
  
  dict.remove("health");
      
  EXPECT_TRUE(!dict.contains("health"));
  EXPECT_TRUE(dict.size() == 2) << dict.size();
  
  
  dict.remove("armour");
  dict.remove("attack");
  EXPECT_TRUE(dict.size() == 0) << dict.size();
  
  dict["armour"] = 15;
  
  EXPECT_TRUE(dict["armour"] == 15) << dict["armour"];
}
