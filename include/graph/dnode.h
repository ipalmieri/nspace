#ifndef __DNODE_H__
#define __DNODE_H__

#include <string>
#include <vector>
#include <map>
#include "dedge.h"

namespace tools
{
   typedef unsigned long nodeId; 

   ////////////////////////////////////////////////
   /// Node for dgraph
   ////////////////////////////////////////////////
   template<typename N, typename E> class dnode
   {
     public:

      typedef std::map<nodeId, dnode<N, E> *> dnodeMap;
      friend class dgraph<N, E>;
 
      inline nodeId id() { return _id; }
      inline N getProperty() { return _property; }

      inline typename dedge<N, E>::dedgeMap::const_iterator inputEdgesBegin() { return _inputs.begin(); }
      inline typename dedge<N, E>::dedgeMap::const_iterator inputEdgesEnd() { return _inputs.end(); }
     
      inline typename dedge<N, E>::dedgeMap::const_iterator outputEdgesBegin() { return _outputs.begin(); }
      inline typename dedge<N, E>::dedgeMap::const_iterator outputEdgesEnd() { return _outputs.end(); }


     protected:

      nodeId _id;
      N _property;

      dnode(const nodeId &id, const N prop) { _id = id; _property = prop; }
      ~dnode() {}

      typename dedge<N, E>::dedgeMap _inputs;
      typename dedge<N, E>::dedgeMap _outputs;

      void addInput(dedge<N, E> *link) { if (link) _inputs[link->id()] = link; }
      void delInput(dedge<N, E> *link) { _inputs.erase(link->id()); }

      void addOutput(dedge<N, E> *link) { if (link) _outputs[link->id()] = link; }
      void delOutput(dedge<N, E> *link) { _outputs.erase(link->id()); }   

      
      dedge<N, E> *getInEdge(const nodeId &id)
      {
	 for (typename dedge<N, E>::dedgeMap::iterator it = _inputs.begin(); it != _inputs.end(); ++it)
	 {
	    dnode<N, E> *nd = it->second->inNode();
	    if (nd->id() == id) return it->second;
	 }

	 return NULL;
      }


      dedge<N, E> *getOutEdge(const nodeId &id)
      {
	 for (typename dedge<N, E>::dedgeMap::iterator it = _outputs.begin(); it != _outputs.end(); ++it)
	 {
	    dnode<N, E> *nd = it->second->outNode();
	    if (nd->id() == id) return it->second;
	 }
	
	 return NULL;
      }
   };

}

#endif //__DNODE_H__
