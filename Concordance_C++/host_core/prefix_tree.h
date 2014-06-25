/**********************************************
 * Prefix_tree:                               *
 * this class implements a Prefix tree.       *
 * The tree starts from the root, a node      *
 * representing an empty string or content).  *
 * Then, nodes are pushed down the tree       *
 * with the method "add_content".             *
 * Nodes are inserted by insertions and/or    *
 * splitting.                                 *
 * Each node holds a linked list of sorted    *
 * children (i.e., ordered using the ordering *
 * relation defined on the contents).         *
 * Visiting the tree (e.g., using a visitor), *
 * the content of all the tree is printed     *
 * following the sorting of the nodes.        *
 * @note: all operations (insertion, lookup,  *
 * etc.) are performed in O(k) time where k   *
 * is the maximum length of the strings in    *
 * the set. Visiting the tree takes time      *
 * O(nk), where n is the number of the        *
 * strings in the set. Space is O(nk).        *
 **********************************************/
#ifndef CONCORDANCE_PREFIX_TREE__
#define CONCORDANCE_PREFIX_TREE__

#include "globals.h"
#include "data_store.h"
#include "node.h"
#include "content.h"
#include "parser.h"

class Prefix_tree : public Data_store {
private:
  std::string _dbg;
  /// Root: representing an empty content
  Node* _root;
  
  void visit_tree ();
public:
  Prefix_tree  ();
  ~Prefix_tree ();
  
  /* 
   * Size of the tree:
   * number of nodes in the tree.
   */
  int get_size ();
  
  /*
   * Add content to the tree.
   * It pushes down the content starting from
   * the root of the tree.
   */
  void add_content ( Content* );
  
  /// Fill Prefix tree using a parser
  void fill_store ( Parser * );
  
  /* 
   * Visit tree (it prints the sorted list of
   * the contents of the nodes).
   */
  void print ();
  
  /*
   * Prints data structure.
   * In this case, it prints the tree, 
   * i.e., nodes info.
   */
  void print_data_store ();
};

#endif


