#ifndef __UFDS_H__
#define __UFDS_H__

#include <algorithm>
#include <climits>
#include <vector>
#include <unordered_map>

namespace advtec {

struct ufdsNode {
  int rank = 0;
  int parent = 0;
};


//////////////////////////////////////////
// Fast Union-Find Disjoint Set Forest
// Integers mapped in O(1) to set position
//////////////////////////////////////////

class fastUFDSForest {

 public:
  
  fastUFDSForest(const int& n) {
    int ns = std::max(0, std::min(n, INT_MAX));
    _nodes.resize(ns);
    for (int i = 0; i < ns; i++) { _nodes[i].rank = 0; _nodes[i].parent = i; }
    _setCount = n;
  }

  ~fastUFDSForest() {}

  int findSet(const int& x) {
    if (_nodes[x].parent == x) return x;
    else {
      _nodes[x].parent = findSet(_nodes[x].parent);
      return _nodes[x].parent;
    }
  }

  bool isSameSet(const int& x, const int& y) {
    return findSet(x) == findSet(y);
  }

  void unionSet(const int& x, const int& y) {
    int sx = findSet(x);
    int sy = findSet(y);
    if (sx == sy) return;
    if (_nodes[sx].rank > _nodes[sy].rank) {
      _nodes[sy].parent = sx;
    }
    else {
      _nodes[sx].parent = sy;
      if (_nodes[sx].rank == _nodes[sy].rank) _nodes[sy].rank++;
    }
    _setCount--;
  }

  inline int setCount() { return _setCount; }
  inline int elementCount() { return _nodes.size(); }
  
 protected:
  int _setCount;
  std::vector<ufdsNode> _nodes;
};



//////////////////////////////////////////
// Hash Union-Find Disjoint Set Forest
// Generic class mapped using hash func
//////////////////////////////////////////

template <class T> class hashUFDSForest : public fastUFDSForest {

 public:

  hashUFDSForest() : fastUFDSForest(0) { _setCount = 0; }
  
  hashUFDSForest(const std::vector<T>& elements) {
    int ns = elements.size();
    _setCount = 0;
    for (int i = 0; i < ns; i++) {
      if (_nmap.find(elements[i]) != _nmap.end()) continue;
      ufdsNode nd;
      nd.rank = 0; nd.parent = i;
      _nodes.push_back(nd);
      _nmap[elements[i]] = _nodes.size() - 1;
      _setCount++;
    }
  }

  ~hashUFDSForest() {}

  int findSet(const T& x) {
    int xi = addElement(x);
    return fastUFDSForest::findSet(xi);
  }


  bool isSameSet(const T& x, const T& y) {
    int xi = addElement(x);
    int yi = addElement(y);
    return fastUFDSForest::findSet(xi) == fastUFDSForest::findSet(yi);
  }

  void unionSet(const T& x, const T& y) {
    int xi = addElement(x);
    int yi = addElement(y);
    return fastUFDSForest::unionSet(xi, yi);
  }

  int addElement(const T& x) {
    auto it = _nmap.find(x);
    if (it != _nmap.end()) return it->second;
    int xi = _nodes.size();
    ufdsNode nd;
    nd.rank = 0; nd.parent = xi;
    _nodes.push_back(nd);
    _nmap[x] = xi;
    _setCount++;
    return xi;
  }

  protected:

  std::unordered_map<T, int> _nmap;
};

}

#endif //__UFDS_H__
