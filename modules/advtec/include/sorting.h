#ifndef __SORTING_H__
#define __SORTING_H__

#include <algorithm>
#include <vector>
#include "aux.h"

namespace advtec {

//========== QUICK SORT ==========
//UNSTABLE
//TIME - avg: O(nlogn) max: O(n^2)
//SPACE - max: O(C) (in place) STACKSPACE: O(logn)
template <typename T> void _quicksort(std::vector<T>& vec,
                                      const unsigned& start,
                                      const unsigned& end)
{
  if (start < end) {
    unsigned pivotIndex = tools::randU(start, end);
    T pivotValue = vec[pivotIndex];

    std::swap(vec[pivotIndex], vec[end]);

    pivotIndex = start;

    for (unsigned i = start; i < end; i++) {
      if (vec[i] < pivotValue) {
        std::swap(vec[i], vec[pivotIndex]);
        pivotIndex++;
      }
    }

    std::swap(vec[pivotIndex], vec[end]);

    _quicksort(vec, start, pivotIndex);
    _quicksort(vec, pivotIndex + 1, end);

  }
}

template <typename T> void quicksort(std::vector<T>& vec)
{
  _quicksort(vec, 0, vec.size() - 1);
}


//========== MERGE SORT ==========
//STABLE
//TIME - avg: O(nlogn) max: O(nlogn)
//SPACE - max: O(n)  STACKSPACE: O(logn)
template <typename T> void mergesort(std::vector<T>& vec)
{
  vec = _mergesort(vec);
}

template <typename T> std::vector<T> _mergesort(std::vector<T>& vec)
{
  if (vec.size() == 1) {
    return vec;
  }

  std::vector<T> vec1, vec2;
  unsigned ldiv = vec.size()/2;

  for (unsigned i = 0; i < ldiv; i++) {
    vec1.push_back(vec[i]);
  }

  for (unsigned i = ldiv; i < vec.size(); i++) {
    vec2.push_back(vec[i]);
  }

  vec1 = _mergesort(vec1);
  vec2 = _mergesort(vec2);

  //now merge the solutions
  std::vector<T> vret;
  unsigned u1, u2;

  u1 = u2 = 0;
  while (u1 < vec1.size() || u2 < vec2.size()) {
    if (u1 < vec1.size() && u2 < vec2.size()) {
      if (vec1[u1] < vec2[u2]) {
        vret.push_back(vec1[u1]);
        u1++;
      } else {
        vret.push_back(vec2[u2]);
        u2++;
      }
    } else if (u1 < vec1.size()) {
      vret.push_back(vec1[u1]);
      u1++;
    } else if (u2 < vec2.size()) {
      vret.push_back(vec2[u2]);
      u2++;
    }

  }

  return vret;
}


//========== INSERTION SORT ==========
//STABLE
//TIME - avg: O(n^2) max: O(n^2)
//SPACE - max: O(1)  STACKSPACE: O(1)
template <typename T> void insertionsort(std::vector<T>& vec)
{
  for (unsigned i = 1; i < vec.size(); i++) {
    T item = vec[i];
    unsigned ihole = i;

    while (ihole > 0 && vec[ihole - 1] > item) {
      vec[ihole] = vec[ihole - 1];

      ihole = ihole - 1;
    }

    vec[ihole] = item;
  }
}

}

#endif
