/**************************************************
 * Red Black Tree:                                *
 * this class implement a RB-Tree together        *
 * with the operations for inserting nodes in it. *
 * RB-Tree is implemented using a C++ stl map.    *
 * The key of the elements are the tokens while   *
 * the values stored are the infos about the      *
 * tokens returned by the parser.                 *
 * Operations are performed in O(log n) time      *
 * where n is the number of tokens in the set.    *
 * Filling the tree takes time O(n log n).        *
 * Space is O(n log n).                           *
 * Printing the elements of the tree takes        *
 * O(n) time and the elements are printed         *
 * following the ordering used for insertion.     *
 * Prefix tree.                                   *
 * It starts from a root (node rep.               *
 * an empty string or content) and then it adds   *
 * nodes performing insertion and/or splitting.   *
 * Visint the tree, the content of the tree       *
 * is printed following the ordering of the       *
 * nodes.                                         *
 **************************************************/
#ifndef CONCORDANCE_RB_TREE__
#define CONCORDANCE_RB_TREE__

#include "globals.h"
#include "data_store.h"
#include "content.h"
#include "parser.h"

class RB_Tree : public Data_store {
private:
  std::string _dbg;
  /// STL map representing the tree
  std::map < std::string, std::vector < Content* > > _rb_tree;
  void visit_tree ();
public:
  RB_Tree  ();
  ~RB_Tree ();
  
  /* 
   * Size of the tree:
   * number of nodes in the tree.
   */
  int get_size ();
  
  /*
   * Add content to the tree.
   * Inset a new string with the info
   * specified in the content.
   * Otherwise update the content if the key is
   * already present.
   */
  void add_content ( Content* );
  
  /// Fill RB-tree using a parser
  void fill_store ( Parser * );
  
  /* 
   * Visit tree (it prints the sorted list of
   * the contents of the nodes).
   */
  void print ();
};

#endif


