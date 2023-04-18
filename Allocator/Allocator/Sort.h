#include "Pool.h"
#include <iostream>
#include <vector>

template<typename T>
int Partition(std::vector<Pool<T>*>& pools, int l, int r) {
  auto v = pools[(l + r) / 2];
  int ind_l = l, ind_r = r;
  while (ind_l <= ind_r) {
    while (pools[ind_l] < v) ind_l++;
    while (pools[ind_r] > v) ind_r--;
    if (ind_l >= ind_r) break;
    std::swap(pools[ind_l], pools[ind_r]);
    ind_l++;
    ind_r--;
  }
  return ind_r;
}

template<typename T>
//void sort(std::vector<Pool<T>*>& pools, int left = 0, int right = pools.size() - 1) {
void sort(std::vector<Pool<T>*>& pools, int left, int right) {
  if (left < right) {
    int q = Partition(pools, left, right);
    sort(pools, left, q);
    sort(pools, q + 1, right);
  }
}
