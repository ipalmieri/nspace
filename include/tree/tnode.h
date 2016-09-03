#ifndef __TNODE_H__
#define __TNODE_H__

#include <stdlib.h>
#include <vector>
#include <map>

namespace tools {
class tnode
{
 public:
  typedef std::vector<tnode*> tnodeVector;

  tnode(tnode* parent = NULL);
  virtual ~tnode();

  void addChild(tnode* child);
  void removeChild(tnode* child);
  void setParent(tnode* parent);

  tnodeVector children()
  {
    return _children;
  }

 protected:

  tnodeVector _children;
  tnode* _parent;

};

}

#endif //__TNODE_H__
