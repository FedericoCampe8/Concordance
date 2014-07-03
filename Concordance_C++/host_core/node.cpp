#include "node.h"

//#define NODE_DBG
//#define INSERT_CHILD_DBG

using namespace std;

int Node::_gbl_node_id = 0;

Node::Node () :
_final      ( false ),
_content    ( nullptr ),
_node_level ( 0 ) {
  _node_id = _gbl_node_id++;
}//-

Node::Node ( Content* content ) :
_final      ( true ),
_content    ( content ),
_node_level ( 0 ) {
 _node_id = _gbl_node_id++;
}//-

//@todo: use shared_ptr
Node::~Node () {
  delete _content;
  _content = nullptr;
  std::list< Node * >::iterator it;
  for ( it = _children.begin(); it != _children.end(); ++it ) {
      delete (*it);
      (*it) = nullptr;
  }
  _children.clear();
}//-

bool
Node::is_final () const {
  return _final;
}//is_final

void
Node::set_final ( bool final ) {
  _final = final;
}//set_final

int
Node::get_level () const {
  return _node_level;
}//get_level

void
Node::set_level ( int lvl ) {
  _node_level = lvl;
}//set_level

void
Node::increase_children_level ( int increment ) {
  for ( auto &child: _children ) {
    child->set_level ( child->get_level() + increment );
    child->increase_children_level ( increment );
  }
}//increase_children_level

int
Node::get_glb_id () const {
  return _gbl_node_id;
}//get_glb_id

int
Node::get_id () const {
  return _node_id;
}//get_id

void
Node::replace_children ( list < Node* > new_childrend_list ) {
  _children.clear();
  _children = new_childrend_list;
}//replace_children

const Content *
Node::get_content() {
  return _content;
}//get_content

bool
Node::add_content ( Content * content ) {
#ifdef NODE_DBG
  std::cout << "#log: add_content - Add Contents...\n";
  content->print();
#endif
  /// Base case: current content is empty
  /// Case 1: Add here.
  if ( _content == nullptr ) {
#ifdef NODE_DBG
    std::cout << "#log: add_content - Case 1: Add new content\n";
#endif
    _content = content;
    return true;
  }
  /// The current content is not empty -> check
  /// Case 2: check wheter to add a content to the current one
  if ( _content->equal ( content ) ) {
#ifdef NODE_DBG
    std::cout << "#log: add_content - Case 2: Update\n";
#endif
    _content->update_content( content );
    return true;
  }
  /// Case 3: new node if no prefix is found in the list of children
  std::list< Node * >::iterator it = is_root_present ( content );
  if ( it == _children.end() ) {
    /// No match
#ifdef NODE_DBG
    std::cout << "#log: add_content - Case 3: Not root found in the list of children\n";
#endif
    /*
     * Insert it as a new child (new node) the right position
     * in the list of children.
     */
    Node * node_to_add = new Node( content );
    node_to_add->set_level ( get_level() + 1 );
    insert_child ( node_to_add );
  }
  else {
    /// There is a match
#ifdef NODE_DBG
    std::cout << "#log: add_content - Case 4: Root found\n";
#endif
    /*
     * @note: it is the iterator which corresponds to the child
     * where a matching has been found.
     * Fix a node: proceed by sub-cases
     */
    (*it)->fix_node ( content );
  }
  return true;
}//add_content

void
Node::fix_node ( Content * content ) {
#ifdef NODE_DBG
  std::cout << "#log: fix_node - Content:\n";
  content->print();
  std::cout << "#log: fix_node - to compare with:\n";
  _content->print();
#endif
  /*
   * It returns a vector of contents:
   * the vector is empty         -> complete match             -> perform update current content
   * the vector as one entry     -> complete match + extension -> add child
   * the vector as two entries   -> sub match                  -> split the previous content
   * the vector as three entries -> partial match              -> split and add
   */
  vector< Content * > new_contents;
  _content->how_match ( content, new_contents );
  
#ifdef NODE_DBG
  std::cout << "#log: fix_node - Sub-cases: " <<
  new_contents.size() << endl;
#endif
  /// Check cases
  if ( new_contents.size() == 0 ) {
    
    /*
     * Update the current node (full match).
     * It updates lines and increase counter.
     */
    _final = true;
    _content->update_content( content );
    /* "content" has been used only for updating the current
     * one, and it won't be used in the future.
     * It can be deleted.
     */
    delete content;
  }
  else if ( new_contents.size() == 1 ) {
    
    /*
     * "content" has the form "xy" where "x" is a prefix matching
     * the current content (_content) while "y" is a suffix.
     * Use new_contents[ 0 ] (i.e., the suffix "y") to recursively
     * go down the tree and find the right place for the remaining
     * part "y".
     * @note: there is a subtle thing here. If "y" is equal to "x"
     * then "y" must be added to the list of children of the current
     * node, otherwise "y" will be inserted in the current node with
     * the recursive call of add_content.
     */
    /// Check which child matches new_contents[ 0 ]
    if ( _content->equal ( new_contents[ 0 ] ) ) {
      bool is_a_match = false;
      for ( auto &child : _children ) {
        if ( child->match_root ( new_contents[ 0 ] ) ) {
          /// A match has been found: proceed recursively with the right child
          child->add_content ( new_contents[ 0 ] );
          is_a_match = true;
          break;
        }
      }//for
      if ( !is_a_match ) {
        /// If no child matches new_contents[ 0 ] add it to _children
        Node * new_child = new Node ( new_contents[ 0 ] );
        new_child->set_level ( get_level() + 1 );
        insert_child ( new_child );
      }
    }
    else {
      /// No match for "y", proceed recursively
      add_content ( new_contents[ 0 ] );
    }
  }
  else if ( new_contents.size() == 2 ) {
    
    /*
     * "content" has the form "z" where "z" is a prefix of the current
     * content "x" = "zt". 
     * Split the node and replace the current content with new_contents[ 0 ]
     * (i.e., "z"), make it final and set counter to 1 and add as child 
     * new_contents[ 1 ] (i.e., "t") with all the info of "x":
     * before: zt -> children -> ...
     * after:  z  -> t -> children -> ...
     */
    _content->update_content ( new_contents[ 0 ] );
    /// new_contents[ 0 ] wont' be used anymore: delete it.
    delete new_contents[ 0 ];
    
    /// Create a new node in the tree for new_contents[ 1 ]
    Node * split_child = new Node ( new_contents[ 1 ] );
    split_child->set_level ( get_level() + 1 );
    increase_children_level ( 1 );
    /// Preserves the _final parameter of the current node
    split_child->set_final( _final );
    /// Link it to the current children
    split_child->replace_children( _children );
    /// Link current node to split_child
    _children.clear();
    insert_child ( split_child );
    
    /// Set the current node as final since "z" is a token
    _final = true;
  }
  else {
    
    /*
     * "content" has the form "xy" where "x" is a prefix
     * of the current content "xt".
     * Split the node and replace the current content with new_contents[ 0 ]
     * (i.e., "x"), make it NOT final and add as child
     * new_contents[ 1 ] (i.e., "t") with all the info of "xt", and 
     * new_contents[ 2 ] (i.e., "y") with lines of "xy" and counter 1.
     * before: xt -> children -> ...
     * after:  x  -> t -> children -> ...
     *            -> y
     */
    _content->update_content ( new_contents[ 0 ] );
    /// new_contents[ 0 ] wont' be used anymore: delete it.
    delete new_contents[ 0 ];
    
    /// Create a new node (i.e., t)
    Node * split_child = new Node ( new_contents[ 1 ] );
    split_child->set_level ( get_level() + 1 );
    increase_children_level ( 1 );
    /// Preserves the _final parameter of the current node
    split_child->set_final( _final );
    /// Link to the new node the current children
    split_child->replace_children( _children );
    _children.clear();
    /// Link the new node to the current node
    insert_child ( split_child );

    /// Create a new node (i.e., y)
    Node * new_child = new Node ( new_contents[ 2 ] );
    new_child->set_level ( get_level() + 1 );
    /// Link the new node to the current node
    insert_child ( new_child );
    
    /// Set the current node as not final (it has been split)
    _final = false;
  }
  /// Clear the list of new/updated contents
  new_contents.clear();
}//fix_node

void
Node::insert_child ( Node * node_to_add ) {
#ifdef INSERT_CHILD_DBG
  cout << "#log: insert_child - Node to insert into the list:\n";
  node_to_add->print();
  cout << "#log: insert_child - Current content of the list:\n";
  for ( auto child: _children ) child->print();
#endif
  
  if ( _children.size() == 0 ) {
    _children.push_back( node_to_add );
  }
  else {
    std::list< Node * >::iterator it = find_lower_root ( node_to_add->get_content() );
    /// Insert the new node in the right position
    _children.insert ( it, node_to_add );
  }
  
#ifdef INSERT_CHILD_DBG
  cout << "#log: insert_child - Updated list:\n";
  for ( auto child: _children ) child->print();
#endif
}//insert_child

std::list< Node * >::iterator
Node::is_root_present ( const Content * content ) {
  
  std::list< Node * >::iterator it;
  for ( it = _children.begin(); it != _children.end(); ++it ) {
    if ( (*it)->match_root ( content ) ) return it;
  }
  it = _children.end();
  return it;
}//is_root_present

std::list< Node * >::iterator
Node::find_lower_root ( const Content * content ) {
  
  bool found = false;
  std::list< Node * >::iterator it;
  for ( it = _children.begin(); it != _children.end(); ++it ) {
    if ( (*it)->less_root ( content ) ) continue;
    
    found = true;
    break;
  }
  if ( !found ) it = _children.end();

  return it;
}//find_lower_root

bool
Node::equal_root ( const Content * content ) {
  return _content->equal ( content );
}//less_root

bool
Node::less_root ( const Content * content ) {
  return _content->less ( content );
}//less_root

bool
Node::match_root ( const Content * content ) {
  return _content->match ( content );
}//get_content

void
Node::accept ( Visitor& visitor ) {
  visitor.visitNode( this );
  for ( auto child: _children ) {
    child->accept ( visitor );
  }
}//accept

void
Node::print () {
  cout << "NODE_"   << _node_id << "\n";
  cout << "Level: " << _node_level << "\n";
  cout << "Final: ";
  if ( _final ) cout << "T\n";
  else          cout << "F\n";
  cout << "Content:\n";
  _content->print();
  for ( auto child : _children ) {
    child->print();
  }
}//print_node

void
Node::visit_node () {
  /// Preorder visit: root - children
  if ( _final ) {
    _content->print();
  }
  for ( auto child : _children ) {
    child->visit_node();
  }
}//visit_node
