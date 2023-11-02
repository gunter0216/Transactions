#include "tests.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "../hashtable/hash_table.h"
#include "../tree/self_balancing_binary_search_tree.h"
#include "hash_table_test.inl"
#include "tree_test.inl"

void GenTable(const std::string& filename, int size) {
  std::vector<std::string> towns{
      "Novosibirsk",   "Moscow",     "Kazan",        "Omsk",
      "Tomsk",         "Vologda",    "Perm",         "Kemerovo",
      "Sochi",         "Novgorod",   "Sydney",       "London",
      "Budapest",      "Paris",      "Rom",          "Amsterdam",
      "Berdsk",        "Warner",     "Lyon",         "Conyersville",
      "Willaha",       "Center",     "Mittenlane",   "Coltman",
      "Scottsville",   "Hebron",     "Longview",     "Emerson",
      "Ipswich",       "Storms",     "Kalauao",      "Farwell",
      "Mitchell",      "Kreutzberg", "Cimarron",     "Brookhaven",
      "Sacramento",    "Rabbitown",  "Fairland",     "Gaskil",
      "Stanardsville", "Curriers",   "Skookumchuck", "Edgerton",
      "Slater"};

  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> distr(4, 9);
  std::uniform_int_distribution<int> year(1970, 2005);
  std::uniform_int_distribution<int> town_ind(0, (int)towns.size() - 1);
  std::uniform_int_distribution<int> coins(0, 4000);
  std::uniform_int_distribution<int> names(0, (int)Names.size() - 1);
  std::uniform_int_distribution<int> surnames(0, (int)Surnames.size() - 1);
  std::unordered_set<std::string> set;

  while ((int)set.size() < size) {
    set.insert(RandStr(distr(engine)));
  }

  //  cout << RandStr(15);
  std::ofstream file(filename, std::ios_base::out);

  for (const auto& i : set) {
    file << i << ' ' << Surnames[surnames(engine)] << ' '
         << Names[names(engine)] << ' ' << year(engine) << " "
         << towns[town_ind(engine)] << " " << coins(engine) << std::endl;
  }
  //  for (int i = 0; i < 10000; ++i) {
  //    cout << RandStr(distr(engine)) << "\t" << year(engine) << " "
  //         << towns[town_ind(engine)] << " " << coins(engine) << endl;
  //  }
  file.close();
}

int main() {
  std::vector<int> sizes{10, 100, 1000};
  for (auto i : sizes) GenTable(std::to_string(i) + ".dat", i);

  testing::InitGoogleTest();
  auto res = RUN_ALL_TESTS();
  for (auto i : sizes)
    system(std::string{"rm " + std::to_string(i) + ".dat"}.c_str());
  return res;
}