#include "globals.h"
#include "prefix_tree.h"
#include "concordance_visitor.h"
#include "token.h"

//#define PREFIX_TREE_DBG

Prefix_tree::Prefix_tree () {
  _dbg  = "#log: Prefix_tree - ";
  _root = new Node();
}//-

//@todo: use shared_ptr
Prefix_tree::~Prefix_tree () {
  delete _root;
  _root = nullptr;
}//-

int
Prefix_tree::get_size () {
  return _root->get_glb_id();
}//get_size

void
Prefix_tree::add_content ( Content* content ) {
  if ( content != nullptr ) {
#ifdef PREFIX_TREE_DBG
    content->print();
#endif
    _root->add_content ( content );
  }
}//add_content

void
Prefix_tree::print_data_store () {
  _root->print();
}//print

void
Prefix_tree::print () {
  visit_tree();
}//print

void
Prefix_tree::visit_tree () {
  
  if ( _verbose ) {
    std::cout << _dbg << "Start visiting tree\n";
    std::cout << _dbg << "Create visitor...\n";
  }
  
  /// Create a visitor
  ConcordanceVisitor * concordance_visitor =
  new ConcordanceVisitor ( _out_file );
  
  if ( _timer ) set_timer();
  
  if ( _verbose ) {
    std::cout << _dbg << "...visitor created\n";
    std::cout << _dbg << "Visiting tree...\n";
    std::cout << std::endl;
    std::cout << "---------------- CONCORDANCE ------------------\n";
  }
  
  _root->accept ( *concordance_visitor );
  
  if ( _verbose ) {
    std::cout << "-----------------------------------------------\n";
    std::cout << std::endl;
  }
  
  double tot_time;
  if ( _timer ) tot_time = get_timer();
  
  /// Print to file
  if ( _out_file.compare( "" ) != 0 ) {
    concordance_visitor->print_to_file ();
  }
  
  if ( _timer ) {
    std::cout << _dbg << "Time for visiting the Prefix tree: ";
    std::cout <<  tot_time << " sec.\n";
  }
  
  if ( _verbose ) {
    std::cout << _dbg << "...tree visited\n";
    std::cout << _dbg << "Delete visitor...\n";
  }
  
  /// Delete visitor
  delete concordance_visitor;
  
  if ( _verbose ) {
    std::cout << _dbg << "visitor deleted\n";
  }
}//visit_tree

void
Prefix_tree::fill_store ( Parser * parser ) {
  if ( _verbose ) {
    std::cout << _dbg << "Start filling store...\n";
  }
  
  if ( _timer ) set_timer();
  
  /// Add the root -> empty string
  _root->add_content ( new Token("") );
  /*
   * Invoke parser to get all the
   * contents to add (on-line) into the tree.
   */
  while ( parser->has_more_elements() ) {
    add_content ( parser->get_next_content() );
  }
  
  if ( _timer ) {
    std::cout << _dbg << "Time for filling the Prefix tree: ";
    std::cout << get_timer() << " sec.\n";
  }
  
  if ( _verbose ) {
    std::cout << _dbg << "...Filled store.\n";
  }
}//fill_store
