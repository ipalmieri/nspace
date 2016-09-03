#include "tree/tnode.h"

using namespace std;
using namespace tools;

tnode::tnode(tnode* parent)
{
  if (parent) {
    parent->addChild(this);
  } else {
    _parent = NULL;
  }
}

tnode::~tnode()
{
  std::vector<tnode*> cBackup = _children;

  if (_parent) {
    _parent->removeChild(this);
  }

  for (std::vector<tnode*>::iterator it = cBackup.begin(); it != cBackup.end();
       ++it) {
    delete (*it);
  }

  _children.clear();

}

void tnode::addChild(tnode* child)
{
  bool has = false;

  for (std::vector<tnode*>::iterator it = _children.begin();
       it != _children.end(); ++it) {
    if (*it == child) {
      has = true;
      break;
    }
  }

  if (!has) {
    _children.push_back(child);
  }

  if (child->_parent) {
    child->removeChild(child);
  }

  child->_parent = this;
}

void tnode::removeChild(tnode* child)
{
  for (std::vector<tnode*>::iterator it = _children.begin();
       it != _children.end(); ++it) {
    if (*it == child) {
      (*it)->_parent = NULL;
      _children.erase(it);
      break;
    }
  }
}

void tnode::setParent(tnode* parent)
{
  if (_parent) {
    _parent->removeChild(this);
  }

  if (parent) {
    parent->addChild(this);
  }
}


