/**************************************
 * Node:                              *
 * this class represents a node in    *
 * Prefix tree.                       *
 * Each node holds a content and it   *
 * can be final or not.               *
 * If final, the content can be       *
 * printed and represents a word as   *
 * read from the input file.          *
 * Otherwise it is an internal node.  *
 * Nodes can be split in children     *
 * during insertion of new tokens     *
 **************************************/
#ifndef CONCORDANCE_NODE__
#define CONCORDANCE_NODE__

#include "globals.h"
#include "content.h"
#include "visitor.h"

class Node {
protected:
  /// Level of the node in the tree
  int _node_level;
  /// Id of the node
  static int _gbl_node_id;
  int _node_id;
  /// List of pointer to children nodes
  std::list < Node* > _children;
  /// Final/non final node
  bool _final;
  /// Content of the node
  Content * _content;
  
  /*
   * Check if a "prefix" of the current content is present
   * in one of the children; i.e., it checks if one of the
   * children can be a root for the current content.
   * It returns x = -1 if the root is not present or x >= 0,
   * where x is the child corresponding to the root.
   */
  std::list< Node * >::iterator is_root_present ( const Content * );
  
  /*
   * Given a content,
   * find the index of the preeceding root
   */
  std::list< Node * >::iterator find_lower_root ( const Content * );
  bool equal_root ( const Content * );
  bool less_root  ( const Content * );
  
  /*
   * Fix the current node by adding children or splitting it.
   * Cases:
   * a - the root is equal to the current content:
   *     update root;
   * b - content can match and extend _content:
   *     add a child with that extension (final node);
   * c - content can match and it is a prefix of _content:
   *     split the current node in the match and make it final;
   * d - content partially mathces _content:
   *     split the node until the match and add new nodes.
   */
  void fix_node ( Content * content );
  
  /*
   * Insert a child in the list of children.
   * It preserves the ordering.
   */
  void insert_child ( Node * node );
  
public:
  Node  ();
  Node  ( Content * );
  ~Node ();
  
  /// Get/set final parameter
  bool is_final  () const;
  void set_final ( bool );
  /// Get/Set the level of the node in the tree
  int  get_level () const;
  void set_level ( int );
  /*
   * Increase the level of the children:
   * useful for splitting edges in the tree.
   */
  void increase_children_level ( int );
  /// Get current (and global) id of the node
  int get_id () const;
  int get_glb_id () const;
  /// Replace children list (used to add/remove nodes in the tree)
  void replace_children ( std::list < Node* > );
  /// Get the current contet
  const Content * get_content ();
  /// Check wheter a content matches or not the current node
  bool match_root ( const Content * );
  
  /*
   * Add content to the node.
   * given the current node it can
   * perform several actions:
   * 1 - Set the current content;
   * 2 - Add a content to the current one;
   * 3 - Create a new node and add it to the list
   *     of children in the correct position;
   * 4 - Push down the content to one of the children;
   * 5 - Split the current node and add the content down
   *     on one of the children.
   * This cases create the Prefix tree.
   * It returns a Boolean value to inform whether the
   * new content has been added or not.
   */
  bool add_content ( Content* );
  
  /*
   * Accept method to accept a visitor:
   * useful to perform different kinds
   * of visits on the tree.
   */
  virtual void accept ( Visitor& );
  
  /// Print only current node
  virtual void print();
  /*
   * Preorder visit: visit current node and
   * then recursively visit children.
   */
  virtual void visit_node();
};

#endif


