#ifndef __DGRAPH_H__
#define __DGRAPH_H__

#include <map>
#include "dnode.h"
#include "dedge.h"

namespace tools
{
   /////////////////////////////////////////
   /// Bidirecional graph class
   /////////////////////////////////////////
   template<typename N, typename E>  class dgraph
   {
     public:

      dgraph() {}

      ~dgraph()
      {
	 typename dnode<N, E>::dnodeMap::iterator nit = _nodes.begin();
	 for (; nit != _nodes.end(); ++nit)
	    delete nit->second;
   
	 typename dedge<N, E>::dedgeMap::iterator sit = _edges.begin();
	 for (; sit != _edges.end(); ++sit)
	    delete sit->second; 
      }

 
      dnode<N, E> *createNode(N prop)
      {
	 //fix this - do overflow check
	 nodeId id = nextNodeId();

	 dnode<N, E> *newnode = new dnode<N, E>(id, prop);

	 _nodes[id] = newnode;
   
	 return newnode;
      }


      inline void deleteNode(const nodeId &id) { deleteNode(getNode(id)); }


      void deleteNode(dnode<N, E> *n)
      {
	 if (n != NULL)
	 {
	    isolate(n);
            
	    _nodes.erase(n->id());

	    delete n;
	 }
      }


      dnode<N, E> *getNode(const nodeId &id)
      {
	 typename dnode<N, E>::dnodeMap::iterator it;
   
	 it = _nodes.find(id);  

	 if (it != _nodes.end())
	    return it->second;
  
	 return NULL;
      }


      inline dedge<N, E> *getEdge(dnode<N, E> *n1, dnode<N, E> *n2) { 
	 return n1->getInEdge(n2->id()); }
	 

      dedge<N, E> *getEdge(const edgeId &id)
      {
	 typename dedge<N, E>::dedgeMap::iterator it;
   
	 it = _edges.find(id);

	 if (it != _edges.end())
	    return it->second;
     
	 return NULL;
      }


      dedge<N, E> *connectNodes(dnode<N, E> *inputn, dnode<N, E> *outputn, const E prop)
      {
	 dedge<N, E> *ret = NULL;

	 //check for nulls
	 if (inputn != NULL && outputn != NULL)
	 {
	    edgeId newid = nextEdgeId();

	    dedge<N, E> *link = new dedge<N, E>(newid, prop);
	    _edges[newid] = link;

	    //sets synapse pointers
	    link->connectInput(outputn);
	    link->connectOutput(inputn);
  
	    //connect to synapses
	    inputn->addInput(link);
	    outputn->addOutput(link);
 
	    ret = link;
	 }

	 return ret; 
      }


      void disconnectNodes(dnode<N, E> *n1, dnode<N, E> *n2)
      {
	 if (n1 != NULL && n2 != NULL)
	 {
	    disconnectEdge(n1->getInEdge(n2->id()));
		    
	    disconnectEdge(n1->getOutEdge(n2->id()));
	 }
      }

      
      inline unsigned int nodeCount() const { return _nodes.size(); }
      inline unsigned int edgeCount() const { return _edges.size(); }
      
      inline typename dnode<N, E>::dnodeMap::const_iterator nodeMapBegin() { return _nodes.begin(); }
      inline typename dnode<N, E>::dnodeMap::const_iterator nodeMapEnd() { return _nodes.end(); }

      inline typename dedge<N, E>::dedgeMap::const_iterator edgeMapBegin() { return _edges.begin(); }
      inline typename dedge<N, E>::dedgeMap::const_iterator edgeMapEnd() { return _edges.end(); }



     protected:
 
      typename dnode<N, E>::dnodeMap _nodes;
      typename dedge<N, E>::dedgeMap _edges;

      static nodeId nextNodeId() { static nodeId nextIdn = 0; return nextIdn++; }
      static edgeId nextEdgeId() { static edgeId nextIde = 0; return nextIde++; }
      inline void deleteEdge(const edgeId &id) { deleteEdge(getEdge(id)); }

      void deleteEdge(dedge<N, E> *link)
      {
	 if (link != NULL)
	 {
	    if (link->inNode() == NULL && link->outNode() == NULL)
	    {
	       _edges.erase(link->id());

	       delete link;
	    }
	 }
      }

      void disconnectEdge(dedge<N, E> *link)
      {
	 if (link == NULL) return;
	 
	 dnode<N, E> *in_node, *out_node;

	 in_node = link->inNode();
	 out_node = link->outNode();
      
	 in_node->delOutput(link);
	 out_node->delInput(link);
      
	 link->delInput();
	 link->delOutput();
      
	 deleteEdge(link);
      }

      void isolate(dnode<N, E> *n)
      {
	 if (n == NULL) return;

	 //disconnect inputs
	 for (unsigned int i = 0; i < n->_inputs.size(); i++)
	    disconnectEdge(n->_inputs[i]);

	 //disconnect outputs
	 for (unsigned int i = 0; i < n->_outputs.size(); i++)
	    disconnectEdge(n->_outputs[i]);
      }



     private: 

   };

}

#endif //__DGRAPH_H__
