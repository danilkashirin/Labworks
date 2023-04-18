#include "CCircularBufferExt.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(PushBack_BufExt, Int) {
  CCircularBufferExt<int> a(1);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  ASSERT_EQ(*a.begin(), 1);
  ASSERT_EQ(a[1], 2);
  ASSERT_EQ(*(a.end() - 1), 4);

  a.push_back(5);
  a.push_back(6);
  ASSERT_EQ(*a.begin(), 1);
  ASSERT_EQ(*(a.end() - 1), 6);
}

TEST(PushBack_BufExt, Float) {
  CCircularBufferExt<float> a(1);
  a.push_back(1.1);
  a.push_back(2.2);
  a.push_back(3.3);
  a.push_back(4.4);
  ASSERT_EQ(*a.begin(), float(1.1));
  ASSERT_EQ(a[1], float(2.2));
  ASSERT_EQ(*(a.end() - 1), float(4.4));

  a.push_back(5.5);
  a.push_back(6.6);
  ASSERT_EQ(*a.begin(), float(1.1));
  ASSERT_EQ(*(a.end() - 1), float(6.6));
}

TEST(PushBack_BufExt, Double) {
  CCircularBufferExt<double> a(1);
  a.push_back(1.1563);
  a.push_back(2.253);
  a.push_back(3.3537);
  a.push_back(4.4385);
  ASSERT_EQ(*a.begin(), double(1.1563));
  ASSERT_EQ(a[1], double(2.253));
  ASSERT_EQ(*(a.end() - 1), double(4.4385));

  a.push_back(5.5258);
  a.push_back(6.6385);
  ASSERT_EQ(*a.begin(), double(1.1563));
  ASSERT_EQ(*(a.end() - 1), double(6.6385));
}

TEST(PushBack_BuExt, Char) {
  CCircularBufferExt<char> a(1);
  a.push_back('a');
  a.push_back('b');
  a.push_back('c');
  a.push_back('d');

  ASSERT_EQ(*a.begin(), 'a');
  ASSERT_EQ(a[1], 'b');
  ASSERT_EQ(*(a.end() - 1), 'd');

  a.push_back('e');
  a.push_back('f');
  ASSERT_EQ(*a.begin(), 'a');
  ASSERT_EQ(*(a.end() - 1), 'f');
}

TEST(PushBack_BufExt, String) {
  CCircularBufferExt<std::string> a(1);
  a.push_back("abs");
  a.push_back("ABCD");
  a.push_back("Hello world!");
  a.push_back("ITMO");

  ASSERT_EQ(*a.begin(), "abs");
  ASSERT_EQ(a[1], "ABCD");
  ASSERT_EQ(*(a.end() - 1), "ITMO");

  a.push_back("str1");
  a.push_back("str2");
  ASSERT_EQ(*a.begin(), "abs");
  ASSERT_EQ(*(a.end() - 1), "str2");
}

TEST(PushFront_BufExt, Int) {
  CCircularBufferExt<int> a(2);
  a.push_front(1);
  a.push_front(2);
  ASSERT_EQ(*a.begin(), 2);

  a.push_front(3);
  ASSERT_EQ(*a.begin(), 3);
  ASSERT_EQ(*(a.end() - 1), 1);
}

TEST(PushFront_BufExt, Float) {
  CCircularBufferExt<float> a(2);
  a.push_front(1.1);
  a.push_front(2.2);
  ASSERT_EQ(*a.begin(), float(2.2));

  a.push_front(3.3);
  ASSERT_EQ(*a.begin(), float(3.3));
  ASSERT_EQ(*(a.end() - 1), float(1.1));
}

TEST(Equality_BufExt, String) {
  CCircularBufferExt<std::string> a(2);
  CCircularBufferExt<std::string> b(2);
  a.push_back("str1");
  a.push_back("str2");

  b.push_back("str1");
  b.push_back("str2");

  ASSERT_TRUE(a == b);
}

TEST(CircularTest, BufExt) {
  CCircularBufferExt<int> a(3);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  int value = 1;
  for (auto it = a.begin(); it != a.end(); it++, value++) {
    ASSERT_EQ(*it, value);
  }
  for (int i = value; i < value + 3; i++) {
    a.push_back(i);
  }
  value = 1;
  for (auto it = a.begin(); it != a.end(); it++, value++) {
    ASSERT_EQ(*it, value);
  }

}

TEST(NotEquality_BufExt, Char) {
  CCircularBufferExt<char> a(2);
  CCircularBufferExt<char> b(2);
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

TEST(MoveConstructor_BufExt, double) {
  CCircularBufferExt<double> a(2);
  a.push_back(1.23);
  a.push_front(4.45);
  CCircularBufferExt<double> b(a);
  ASSERT_TRUE(a == b);
}

TEST(PopFront_Buf, int) {
  CCircularBuffer<int> a(2);
  a.push_back(3);
  a.push_back(6);
  ASSERT_EQ(*(a.begin()), 3);
  a.pop_front();
  ASSERT_EQ(*(a.begin()), 6);
}

TEST(Empty, BufExt) {
  CCircularBuffer<int> a(2);
  a.push_back(3);
  a.push_back(6);
  a.pop_back();
  a.pop_front();
  ASSERT_TRUE(a.empty());
}

TEST(Insert, BufExt) {
  CCircularBufferExt<int> tmp1(2);
  tmp1.push_back(1);
  tmp1.push_back(2);
  tmp1.push_back(3);
  tmp1.push_back(4);
  auto result_1 = tmp1.insert(tmp1.begin() + 2, 1000);
  ASSERT_EQ(2, *(result_1 - 1));
  ASSERT_EQ(1000, *(result_1));
  ASSERT_EQ(3, *(result_1 + 1));

  CCircularBufferExt<int> tmp2(1);
  tmp2.push_back(1);
  tmp2.push_back(2);
  tmp2.push_back(3);
  tmp2.push_back(4);
  auto result_2 = tmp2.insert(tmp2.begin() + 2, 300, 1000);
  ASSERT_EQ(2, *(result_2 - 1));
  for (int i = 0; i < 300; ++i, result_2++) {
    ASSERT_EQ(1000, *result_2);
  }

  CCircularBufferExt<int> tmp3(1);
  tmp3.push_back(1);
  tmp3.push_back(2);
  tmp3.push_back(3);
  tmp3.push_back(4);
  auto result_3 = tmp3.insert(tmp3.begin() + 1, {10, 1000});
  ASSERT_EQ(1, *(result_3 - 1));
  ASSERT_EQ(10, *result_3);
  ASSERT_EQ(1000, *(result_3 + 1));
}

TEST(Assign, BufferExt) {
  CCircularBufferExt<int, std::allocator<int>> a(4);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.assign(2, 1);
  for (int & i : a) {
    ASSERT_EQ(1, i);
  }
  CCircularBufferExt<int, std::allocator<int>> b(4);
  b.push_back(1);
  b.push_back(2);
  b.push_back(3);
  b.push_back(4);
  CCircularBufferExt<int, std::allocator<int>> c(2);
  c.push_back(9);
  c.push_back(9);
  b.assign(c.begin(), c.end());
  ASSERT_TRUE(b == c);

  CCircularBufferExt<int, std::allocator<int>> d(4);
  d.push_back(1);
  d.push_back(2);
  d.push_back(3);
  d.push_back(4);
  d.assign({0, 0, 0});
  ASSERT_EQ(0, *d.begin());
  ASSERT_EQ(0, *(d.begin() + 1));
  ASSERT_EQ(0, *(d.begin() + 2));
}
