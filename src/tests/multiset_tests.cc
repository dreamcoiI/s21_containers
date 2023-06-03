#include "test_header.h"

namespace {
TEST(Multiset, Constructor_Default) {
  s21::multiset<int> s21_multiset;
  std::multiset<int> std_multiset;
  EXPECT_EQ(s21_multiset.size(), std_multiset.size());
  EXPECT_EQ(s21_multiset.empty(), std_multiset.empty());
}

TEST(Multiset, Constructor_Initializer_list) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4};
  std::multiset<int> std_multiset = {1, 2, 3, 4};
  EXPECT_EQ(s21_multiset.size(), std_multiset.size());
  EXPECT_EQ(s21_multiset.empty(), std_multiset.empty());
}

TEST(Multiset, Constructor_Move) {
  s21::multiset<char> s21_multiset_1 = {'a', 'b', 'c', 'd'};
  std::multiset<char> std_multiset_1 = {'a', 'b', 'c', 'd'};
  s21::multiset<char> s21_multiset_2 = std::move(s21_multiset_1);
  std::multiset<char> std_multiset_2 = std::move(std_multiset_1);

  EXPECT_EQ(s21_multiset_2.size(), std_multiset_2.size());
  EXPECT_EQ(s21_multiset_1.size(), std_multiset_1.size());
  EXPECT_EQ(s21_multiset_1.empty(), std_multiset_1.empty());
  auto it1 = s21_multiset_2.begin();
  auto it2 = std_multiset_2.begin();
  while (it1 != s21_multiset_2.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(Multiset, Modifier_Insert_1) {
  s21::multiset<char> s21_multiset_1 = {'a', 'c', 'd'};
  s21::multiset<char> s21_multiset_2 = {'a', 'c', 'd', 'b'};

  auto it = s21_multiset_1.begin();
  ++it;
  s21_multiset_1.insert('b');
  s21_multiset_1.insert('a');
  auto it1 = s21_multiset_1.begin();
  auto it2 = s21_multiset_2.begin();
  while (it1 != s21_multiset_1.end()) {
    ++it1, ++it2;
  }
  EXPECT_NE(s21_multiset_1.size(), s21_multiset_2.size());
}

TEST(Multiset, Modifier_Insert_2) {
  s21::multiset<char> s21_multiset;

  s21_multiset.insert('b');
  s21_multiset.insert('b');
  s21_multiset.insert('b');

  EXPECT_EQ(s21_multiset.size(), size_t(3));
}

TEST(Multiset, Modifier_Insert_3) {
  s21::multiset<float> s21_multiset;
  std::multiset<float> std_multiset;

  s21_multiset.insert(0.0000000000000000000000000000000000000000001);
  s21_multiset.insert(0.000000000000000000000000000000000000000000099);
  std_multiset.insert(0.0000000000000000000000000000000000000000001);
  std_multiset.insert(0.000000000000000000000000000000000000000000099);

  EXPECT_EQ(s21_multiset.size(), std_multiset.size());
}

TEST(Multiset, Modifier_Erase) {
  s21::multiset<char> s21_multiset_1 = {'a', 'c', 'd'};
  s21::multiset<char> s21_multiset_2 = {'a', 'b', 'c', 'd'};

  auto it = s21_multiset_2.begin();
  ++it;
  s21_multiset_2.erase(it);
  auto it1 = s21_multiset_1.begin();
  auto it2 = s21_multiset_2.begin();
  while (it1 != s21_multiset_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_multiset_1.size(), s21_multiset_2.size());
}

TEST(Multiset, Modifier_Swap) {
  s21::multiset<char> s21_multiset_1 = {'a', 'b', 'o', 'b', 'a'};
  s21::multiset<char> s21_multiset_2 = {'s', 'h', 'l', 'e', 'p', 'p', 'a'};
  s21::multiset<char> s21_multiset_3 = {'a', 'b', 'o', 'b', 'a'};
  s21::multiset<char> s21_multiset_4 = {'s', 'h', 'l', 'e', 'p', 'p', 'a'};
  s21_multiset_1.swap(s21_multiset_2);
  auto it1 = s21_multiset_1.begin();
  auto it2 = s21_multiset_2.begin();
  auto it3 = s21_multiset_3.begin();
  auto it4 = s21_multiset_4.begin();
  while (it1 != s21_multiset_1.end()) {
    EXPECT_EQ(*it1, *it4);
    ++it1, ++it4;
  }
  while (it2 != s21_multiset_2.end()) {
    EXPECT_EQ(*it2, *it3);
    ++it2, ++it3;
  }
  EXPECT_EQ(s21_multiset_1.size(), s21_multiset_4.size());
  EXPECT_EQ(s21_multiset_2.size(), s21_multiset_3.size());
}

TEST(Multiset, Lookup_Contains) {
  s21::multiset<int> s21_multiset;
  s21_multiset.insert(1);
  s21_multiset.insert(2);
  s21_multiset.insert(3);
  s21_multiset.insert(4);
  EXPECT_EQ(s21_multiset.contains(1), true);
  EXPECT_EQ(s21_multiset.contains(2), true);
  EXPECT_EQ(s21_multiset.contains(3), true);
  EXPECT_EQ(s21_multiset.contains(4), true);
  EXPECT_EQ(s21_multiset.contains(7), false);
  EXPECT_EQ(s21_multiset.contains(-2), false);
  EXPECT_EQ(s21_multiset.contains(0), false);
}

TEST(Multiset, Lookup_Find) {
  s21::multiset<int> s21_multiset;
  s21_multiset.insert(1);
  s21_multiset.insert(2);
  s21_multiset.insert(3);
  s21_multiset.insert(4);
  auto it_end = s21_multiset.end();
  EXPECT_EQ(*(s21_multiset.find(1)), 1);
  EXPECT_EQ(*(s21_multiset.find(3)), 3);
  EXPECT_EQ(*(s21_multiset.find(4)), 4);
  EXPECT_EQ(*(s21_multiset.find(2)), 2);
  EXPECT_EQ(s21_multiset.find(-4), it_end);
  EXPECT_EQ(s21_multiset.find(0), it_end);
  EXPECT_EQ(s21_multiset.find(120), it_end);
}

TEST(Multiset, Rehash_And_Insert_In_Collision) {
  s21::multiset<std::string> s21_multiset;
  std::unordered_set<std::string> std_multiset;
  s21_multiset.insert("asd");
  s21_multiset.insert("asa");
  s21_multiset.insert("asf");
  s21_multiset.insert("asg");
  s21_multiset.insert("ash");
  s21_multiset.insert("aqw");
  s21_multiset.insert("fgh");
  s21_multiset.insert("ghf");
  s21_multiset.insert("dghfg");
  s21_multiset.insert("dghfdsg");
  s21_multiset.insert("dghfgfsadfsd");
  s21_multiset.insert("dghfgfsadfsdasd");
  s21_multiset.insert("dghfgfsadfsasdasdd");
  s21_multiset.insert("dghfgfsadfsdasdasd");
  s21_multiset.insert("dghfgfdsasadfdsasasdsadsad");
  s21_multiset.insert("dghdsafgfsadfdsasasdsadsad");
  s21_multiset.insert("ådsdghfgfsadfdsasasdsadsad");
  s21_multiset.insert("dghfgfsadfdsasasdsaddsasad");
  s21_multiset.insert("dghfgfsadfdsasassadadsadsad");
  s21_multiset.insert("dghfgfsadfdsasdasasdsadsad");
  s21_multiset.insert("dghasdaasdfgfsadfsasdsadsad");
  s21_multiset.insert("asddghfgfsadfsasaß∂asdsadsad");
  s21_multiset.insert("dghfgfsadfasdsasdsadsad");
  s21_multiset.insert("dghfgfsadfsasdsadsad");
  s21_multiset.insert("dghfgfsasdadfsasdsadsad");
  s21_multiset.insert("asdsadadghfgfsadfsasdsadsad");
  std_multiset.insert("asd");
  std_multiset.insert("asa");
  std_multiset.insert("asf");
  std_multiset.insert("asg");
  std_multiset.insert("ash");
  std_multiset.insert("aqw");
  std_multiset.insert("fgh");
  std_multiset.insert("ghf");
  std_multiset.insert("dghfg");
  std_multiset.insert("dghfdsg");
  std_multiset.insert("dghfgfsadfsd");
  std_multiset.insert("dghfgfsadfsdasd");
  std_multiset.insert("dghfgfsadfsasdasdd");
  std_multiset.insert("dghfgfsadfsdasdasd");
  std_multiset.insert("dghfgfdsasadfdsasasdsadsad");
  std_multiset.insert("dghdsafgfsadfdsasasdsadsad");
  std_multiset.insert("ådsdghfgfsadfdsasasdsadsad");
  std_multiset.insert("dghfgfsadfdsasasdsaddsasad");
  std_multiset.insert("dghfgfsadfdsasassadadsadsad");
  std_multiset.insert("dghfgfsadfdsasdasasdsadsad");
  std_multiset.insert("dghasdaasdfgfsadfsasdsadsad");
  std_multiset.insert("asddghfgfsadfsasaß∂asdsadsad");
  std_multiset.insert("dghfgfsadfasdsasdsadsad");
  std_multiset.insert("dghfgfsadfsasdsadsad");
  std_multiset.insert("dghfgfsasdadfsasdsadsad");
  std_multiset.insert("asdsadadghfgfsadfsasdsadsad");
  for (auto i : std_multiset) {
    EXPECT_TRUE(s21_multiset.find(i) != s21_multiset.end());
  }
  EXPECT_EQ(s21_multiset.size(), std_multiset.size());
}

}  // namespace
