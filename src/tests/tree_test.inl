#ifndef A6_TREE_TEST_H
#define A6_TREE_TEST_H
TEST(tree, set_get) {
  s21::SelfBalancingBinarySearchTree storage;
  for (int i = 0; i < 100; ++i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(storage.Get(std::to_string(i))->number_of_current_coins, i);
    ASSERT_EQ(storage.Get(std::to_string(i))->year_of_birth, i);
  }
}

TEST(tree, exist) {
  s21::SelfBalancingBinarySearchTree storage;
  for (int i = 0; i < 100; ++i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
    ASSERT_TRUE(storage.Exists(std::to_string(i)));
  }
  ASSERT_FALSE(storage.Exists("ghjkas"));
  ASSERT_TRUE(storage.Exists("51"));
  ASSERT_TRUE(storage.Exists("1"));
  ASSERT_TRUE(storage.Exists("99"));
  ASSERT_TRUE(storage.Exists("24"));
}

TEST(tree, update) {
  s21::SelfBalancingBinarySearchTree storage;
  for (int i = 0; i < 10; ++i)
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  storage.Update("4", "Ivanovich", "Ivan", 1984, "MSK", 2);
  auto ivan = storage.Get("4");
  ASSERT_EQ(ivan->first_name, "Ivan");
  ASSERT_EQ(ivan->year_of_birth, 1984);
  ASSERT_EQ(ivan->number_of_current_coins, 2);
}

TEST(tree, del) {
  s21::SelfBalancingBinarySearchTree storage;
  storage.Set({"2", "1", "1", 1, "1", 1});
  storage.Set({"1", "1", "1", 1, "1", 1});
  ASSERT_TRUE(storage.Del("2"));
  ASSERT_FALSE(storage.Del("sdifhg"));
  for (int i = 0; i < 10; ++i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  }
  ASSERT_TRUE(storage.Exists("3"));
  ASSERT_TRUE(storage.Del("3"));
  ASSERT_FALSE(storage.Del("3"));
  ASSERT_FALSE(storage.Exists("3"));
  ASSERT_TRUE(storage.Exists("7"));
  ASSERT_TRUE(storage.Del("7"));
  ASSERT_FALSE(storage.Del("7"));
  ASSERT_FALSE(storage.Exists("7"));
  ASSERT_EQ(storage.Keys().size(), 8);
  for (int i = 10; i >= 0; --i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  }
  storage.Set({std::to_string(11), std::to_string(11), std::to_string(11), 11,
                 std::to_string(11), 11});
  ASSERT_TRUE(storage.Del("10"));               
  ASSERT_TRUE(storage.Del("11"));
  ASSERT_TRUE(storage.Del("1"));
  ASSERT_TRUE(storage.Del("6"));
  ASSERT_TRUE(storage.Del("9"));
  ASSERT_TRUE(storage.Del("7"));
}

TEST(tree, rename) {
  s21::SelfBalancingBinarySearchTree storage;
  for (int i = 0; i < 10; ++i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  }
  ASSERT_TRUE(storage.Exists("3"));
  storage.Rename("3", "33");
  ASSERT_TRUE(storage.Exists("33"));
  ASSERT_FALSE(storage.Exists("3"));
  ASSERT_TRUE(storage.Exists("7"));
  storage.Rename("7", "77");
  ASSERT_TRUE(storage.Exists("77"));
  ASSERT_FALSE(storage.Exists("7"));
}

TEST(tree, upload_keys) {
  s21::SelfBalancingBinarySearchTree storage10;
  storage10.Upload("10.dat");
  ASSERT_EQ(storage10.Keys().size(), 10);

  s21::SelfBalancingBinarySearchTree storage100;
  storage100.Upload("100.dat");
  ASSERT_EQ(storage100.Keys().size(), 100);

  s21::SelfBalancingBinarySearchTree storage1000;
  storage1000.Upload("1000.dat");
  ASSERT_EQ(storage1000.Keys().size(), 1000);
}

TEST(tree, find) {
  s21::SelfBalancingBinarySearchTree storage;
  for (int i = 0; i < 10; ++i) {
    storage.Set({std::to_string(i), std::to_string(i), std::to_string(i), i,
                 std::to_string(i), i});
  }
  for (int i = 0; i < 10; ++i) {
    ASSERT_EQ(storage.Find(std::to_string(i), std::to_string(i), i)[0],
              std::to_string(i));
  }
  ASSERT_TRUE(storage.Find("kshadgf").empty());
  ASSERT_TRUE(storage.Find("q", "w", 1, "e", 5).empty());
  ASSERT_EQ(storage.Find("1", "1", 1, "1", 1).size(), 1);
  ASSERT_EQ(storage.Find().size(), 10);
}

TEST(tree, showall_export) {
  s21::SelfBalancingBinarySearchTree storage10;
  storage10.Upload("10.dat");
  auto all10 = storage10.ShowAll();
  ASSERT_EQ(all10.size(), 10);
  auto filename = RandStr(18);

  storage10.ExportData(filename);

  s21::SelfBalancingBinarySearchTree st10;
  st10.Upload(filename);
  auto All10 = st10.ShowAll();
  for (int i = 0; i < (int)all10.size(); ++i)
    ASSERT_EQ(all10[i]->key, All10[i]->key);

  s21::SelfBalancingBinarySearchTree storage100;
  storage100.Upload("100.dat");
  auto all100 = storage100.ShowAll();
  ASSERT_EQ(all100.size(), 100);

  s21::SelfBalancingBinarySearchTree storage1000;
  storage1000.Upload("1000.dat");
  auto all1000 = storage1000.ShowAll();
  ASSERT_EQ(all1000.size(), 1000);

  storage1000.ExportData(filename);
  s21::SelfBalancingBinarySearchTree st1000;
  st1000.Upload(filename);
  auto All1000 = st1000.ShowAll();
  for (int i = 0; i < (int)all1000.size(); ++i)
    ASSERT_EQ(all1000[i]->key, All1000[i]->key);

  system(std::string{"rm " + filename}.c_str());
}

TEST(tree, ttl) {
  s21::SelfBalancingBinarySearchTree storage;
  storage.Set({"1", "1", "1", 1, "1", 1}, 1);
  storage.Set({"1", "1", "1", 1, "1", 1}, 1);
  storage.Set({"2", "1", "1", 1, "1", 1}, 1);
  storage.Set({"21", "1", "1", 1, "1", 1}, 1);
  storage.Set({"3", "1", "1", 1, "1", 1}, 1);
  ASSERT_GE(storage.TTL("1"), 0);
  ASSERT_TRUE(storage.Exists("1"));
  ASSERT_FALSE(storage.Keys().empty());
  std::this_thread::sleep_for(std::chrono::milliseconds(1200));
  ASSERT_EQ(storage.TTL("1"), 0);
  ASSERT_TRUE(storage.Keys().empty());
  ASSERT_FALSE(storage.Exists("1"));
}

#endif  // A6_TREE_TEST_H
