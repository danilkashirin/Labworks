#include "CCircularBuffer.h"
#include "CCircularBufferExt.h"
#include <iostream>
#include <algorithm>



int main() {
  CCircularBuffer<int> a(4);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  auto ans1 = a.insert(a.begin() + 2, 9);
  for (auto & i : a) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

//  a.pop_back();
//  for (auto i = a.begin(); i != a.end(); i++) {
//    std::cout << *i << ' ';
//  }
//  std::cout << '\n';


//  auto i = a.begin();
//  std::cout << *(i-1) << ' ' << *i << ' ' << *(i+1) << '\n';
//  std::cout << *(a.end() - 2);
  std::sort(a.begin(), a.end());

}