/**************************************
 * Visitor (interface):               *
 * this class implements a Visitor    *
 * (see Visitor pattern).             *
 * It is used to perform different    *
 * kinds of visits on the tree by     *
 * creating different visitors        *
 * implementations.                   *
 **************************************/
package concordance;

public interface Visitor {
  void visit();
  void visitNode( Node node );
}//Visitor
