/**************************************
 * Visitor (interface):               *
 * this class implements a Visitor    *
 * (see Visitor pattern).             *
 * It is used to perform different    *
 * kinds of visits on the tree by     *
 * creating different visitors        *
 * implementations.                   *
 **************************************/
#ifndef CONCORDANCE_VISITOR__
#define CONCORDANCE_VISITOR__

#include "globals.h"
class Node;

class Visitor {
protected:
  Visitor() {}
public:
  virtual ~Visitor() {};
  
  virtual void visit() = 0;
  /// Visit methods for nodes
  virtual void visitNode ( Node * ) {};
};

#endif


