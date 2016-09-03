#ifndef __DEDGE_H__
#define __DEDGE_H__

#include <stdlib.h>
#include <map>

namespace tools {
typedef unsigned long edgeId;

template<typename N, typename E> class dnode;
template<typename N, typename E> class dgraph;

////////////////////////////////////////////////
/// Edge for dgraph
////////////////////////////////////////////////
template<typename N, typename E = N> class dedge
{
 public:

  typedef std::map<edgeId, dedge<N, E> *> dedgeMap;
  friend class dgraph<N, E>;

  inline edgeId id()
  {
    return _id;
  }
  inline bool isConnected()
  {
    return (_input && _output);
  }
  inline bool isLoop()
  {
    return (_input && _input == _output);
  }

  dnode<N, E>* inNode()
  {
    return _input;
  }
  dnode<N, E>* outNode()
  {
    return _output;
  }

  inline E getProperty()
  {
    return _property;
  }
  inline void setProperty(E property)
  {
    _property = property;
  }

 protected:

  edgeId _id;
  E _property;

  dedge(const edgeId& id, const E& prop)
  {
    _id = id;
    _input = NULL;
    _output = NULL;
    _property = prop;
  }
  ~dedge() {}

  void connect(dnode<N, E>* input, dnode<N, E>* output)
  {
    connectInput(input);
    connectOutput(output);
  }

  inline void connectInput(dnode<N, E>* input)
  {
    _input = input;
  }
  inline void delInput()
  {
    _input = NULL;
  }

  inline void connectOutput(dnode<N, E>* output)
  {
    _output = output;
  }
  inline void delOutput()
  {
    _output = NULL;
  }

  dnode<N, E>* _input;
  dnode<N, E>* _output;

};
}

#endif //__DEDGE_H__
