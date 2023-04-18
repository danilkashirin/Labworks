#include "CCircularBuffer.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(PushBack_Buf, Int) {
  CCircularBuffer<int> a(4);
  a.push_back(1);
  a.push_back(2);
  a.push_back(4);
  a.push_back(3);
  ASSERT_EQ(*a.begin(), 1);
  ASSERT_EQ(a[1], 2);
  ASSERT_EQ(*(a.end() - 1), 3);

  a.push_back(5);
  ASSERT_EQ(*a.begin(), 2);
  ASSERT_EQ(*(a.end() - 1), 5);
}

TEST(CircularTest, Buf) {
  CCircularBuffer<int> a(3);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  int value = 1;
  for (auto it = a.begin(); it != a.end(); it++, value++) {
    ASSERT_EQ(*it, value);
  }
  for (int i = 10; i < 13; i++) {
    a.push_back(i);
  }
  value = 10;
  for (auto it = a.begin(); it != a.end(); it++, value++) {
    ASSERT_EQ(*it, value);
  }

  ASSERT_EQ(a[30], a[0]);
}

TEST(PushBack_Buf, Float) {
  CCircularBuffer<float> a(4);
  a.push_back(1.1);
  a.push_back(2.2);
  a.push_back(4.4);
  a.push_back(3.3);
  ASSERT_EQ(*a.begin(), float(1.1));
  ASSERT_EQ(a[1], float(2.2));
  ASSERT_EQ(*(a.end() - 1), float(3.3));

  a.push_back(5.5);
  ASSERT_EQ(*a.begin(), float(2.2));
  ASSERT_EQ(*(a.end() - 1), float(5.5));
}

TEST(PushBack_Buf, Double) {
  CCircularBuffer<double> a(4);
  a.push_back(1.1563);
  a.push_back(2.253);
  a.push_back(3.3537);
  a.push_back(4.4385);
  ASSERT_EQ(*a.begin(), double(1.1563));
  ASSERT_EQ(a[1], double(2.253));
  ASSERT_EQ(*(a.end() - 1), double(4.4385));

  a.push_back(5.5258);
  a.push_back(6.6385);
  ASSERT_EQ(*a.begin(), double(3.3537));
  ASSERT_EQ(*(a.end() - 1), double(6.6385));
}

TEST(PushBack_Buf, Char) {
  CCircularBuffer<char> a(4);
  a.push_back('a');
  a.push_back('b');
  a.push_back('c');
  a.push_back('d');

  ASSERT_EQ(*a.begin(), 'a');
  ASSERT_EQ(a[1], 'b');
  ASSERT_EQ(*(a.end() - 1), 'd');

  a.push_back('e');
  a.push_back('f');
  ASSERT_EQ(*a.begin(), 'c');
  ASSERT_EQ(*(a.end() - 1), 'f');
}

TEST(PushBack_Buf, String) {
  CCircularBuffer<std::string> a(4);
  a.push_back("abs");
  a.push_back("ABCD");
  a.push_back("Hello world!");
  a.push_back("ITMO");

  ASSERT_EQ(*a.begin(), "abs");
  ASSERT_EQ(a[1], "ABCD");
  ASSERT_EQ(*(a.end() - 1), "ITMO");

  a.push_back("str1");
  a.push_back("str2");
  ASSERT_EQ(*a.begin(), "Hello world!");
  ASSERT_EQ(*(a.end() - 1), "str2");
}

TEST(PushFront_Buf, Int) {
  CCircularBuffer<int> a(2);
  a.push_front(1);
  a.push_front(2);
  ASSERT_EQ(*a.begin(), 2);

  a.push_front(3);
  ASSERT_EQ(*a.begin(), 3);
  ASSERT_EQ(*(a.end() - 1), 2);
}

TEST(PushFront_Buf, Float) {
  CCircularBuffer<float> a(2);
  a.push_front(1.1);
  a.push_front(2.2);
  ASSERT_EQ(*a.begin(), float(2.2));

  a.push_front(3.3);
  ASSERT_EQ(*a.begin(), float(3.3));
  ASSERT_EQ(*(a.end() - 1), float(2.2));
}

TEST(Equality_Buf, String) {
  CCircularBuffer<std::string> a(2);
  CCircularBuffer<std::string> b(2);
  a.push_back("str1");
  a.push_back("str2");

  b.push_back("str1");
  b.push_back("str2");

  ASSERT_TRUE(a == b);
}

TEST(NotEquality_Buf, Char) {
  CCircularBuffer<char> a(2);
  CCircularBuffer<char> b(2);
  a.push_back('a');
  a.push_back('b');
  b.push_back('a');
  b.push_back('c');
  ASSERT_TRUE(a != b);

  a.clear();
  b.clear();
  a.push_back('a');
  a.push_back('b');
  b.push_back('a');
  b.push_back('b');
  a.resize(3);
  ASSERT_TRUE(a != b);
}

TEST(MoveConstructor_Buf, double) {
  CCircularBuffer<double> a(2);
  a.push_back(1.23);
  a.push_front(4.45);
  CCircularBuffer<double> b(a);
  ASSERT_TRUE(a == b);
}

TEST(PopBack_Buf, int) {
  CCircularBuffer<int> a(2);
  a.push_back(3);
  a.push_back(6);
  ASSERT_EQ(*(a.end() - 1), 6);
  a.pop_back();
  ASSERT_EQ(*(a.end() - 1), 3);
}

TEST(NotEmpty, BufExt) {
  CCircularBuffer<int> a(2);
  a.push_back(3);
  a.push_back(6);
  a.pop_back();
  ASSERT_FALSE(a.empty());
}

TEST(Insert, Buf) {
  CCircularBuffer<int> tmp1(4);
  tmp1.push_back(1);
  tmp1.push_back(2);
  tmp1.push_back(3);
  tmp1.push_back(4);
  auto result_1 = tmp1.insert(tmp1.begin() + 2, 1000);
  ASSERT_EQ(2, *(result_1 - 1));
  ASSERT_EQ(1000, *(result_1));
  ASSERT_EQ(3, *(result_1 + 1));

  CCircularBuffer<int> tmp2(4);
  tmp2.push_back(1);
  tmp2.push_back(2);
  tmp2.push_back(3);
  tmp2.push_back(4);
  auto result_2 = tmp2.insert(tmp2.begin() + 2, 3, 1000);
  ASSERT_EQ(2, *(result_2 - 1));
  for (int i = 0; i < 3; ++i, result_2++) {
    ASSERT_EQ(1000, *result_2);
  }

  CCircularBuffer<int> tmp3(4);
  tmp3.push_back(1);
  tmp3.push_back(2);
  tmp3.push_back(3);
  tmp3.push_back(4);
  auto result_3 = tmp3.insert(tmp3.begin() + 1, {10, 100});
  ASSERT_EQ(1, *(result_3 - 1));
  ASSERT_EQ(10, *result_3);
  ASSERT_EQ(100, *(result_3 + 1));
}

TEST(Swap, Buf) {
  CCircularBuffer<int> a(4);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  CCircularBuffer<int> b(3);
  b.push_back(101);
  b.push_back(102);
  b.push_back(103);

  CCircularBuffer<int> tmp1(a);
  CCircularBuffer<int> tmp2(b);
  a.swap(b);
  ASSERT_TRUE(tmp1 == b);
  ASSERT_TRUE(tmp2 == a);
}

