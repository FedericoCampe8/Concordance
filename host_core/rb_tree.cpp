#include "rb_tree.h"
#include "token.h"

//#define RB_TREE_DBG

using namespace std;

RB_Tree::RB_Tree () {
  _dbg  = "#log: RB_Tree - ";
}//-

RB_Tree::~RB_Tree () {
  _rb_tree.clear();
}//-

int
RB_Tree::get_size() {
  return _rb_tree.size();
}//get_size

void
RB_Tree::add_content ( Content* content ) {
  if ( content != nullptr ) {
#ifdef RB_TREE_DBG
    content->print();
#endif
    
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(content) );
    string key = *((string *) content->get_ref_base_obj());
    _rb_tree[ key ].push_back( content );
  }
}//add_content

void
RB_Tree::fill_store ( Parser * parser ) {
  if ( _verbose ) {
    std::cout << _dbg << "Start filling store...\n";
  }
  
  if ( _timer ) set_timer();

  /*
   * Invoke parser to get all the
   * contents to add (on-line) into the map.
   */
  while ( parser->has_more_elements() ) {
    add_content ( parser->get_next_content() );
  }
  
  if ( _timer ) {
    std::cout << _dbg << "Time for filling the tree: ";
    std::cout << get_timer() << " sec.\n";
  }
  
  if ( _verbose ) {
    std::cout << _dbg << "...Filled store.\n";
  }
}//fill_store

void
RB_Tree::print () {
  
  if ( _verbose ) {
    std::cout << _dbg << "Start visiting RB-Tree\n";
  }
  
  bool first_print;
  stringstream str_stream;
  string token_string;
  string lines;
  
  if ( _verbose ) {
    std::cout << _dbg << "Visiting RB-Tree...\n";
    std::cout << std::endl;
    std::cout << "---------------- CONCORDANCE ------------------\n";
  }
  
  if ( _timer ) set_timer();
  
  for ( auto mapped_value : _rb_tree ) {
    first_print = true;
    Content * cont;
    int size = mapped_value.second.size();
    for ( int i = 0; i < size; i++ ) {
      cont = (mapped_value.second)[ i ];
    //for ( auto cont : mapped_value.second ) {
      Token * casted_token = static_cast< Token * > ( const_cast<Content *>(cont) );
      
      if ( first_print ) {
        token_string = casted_token->get_string();
        str_stream << token_string;
        for ( int i = 0; i < (25 - (token_string.length())); i++ ) str_stream << " ";
        str_stream << "{";
        str_stream << mapped_value.second.size();
        str_stream << ":";
        first_print = false;
      }
      
      vector <int> lines = casted_token->get_lines();
      for ( int ii = 0; ii < lines.size(); ii++ ) {
        if (i < size - 1) {
          str_stream << lines[ ii ] <<",";
        }
        else {
          str_stream << lines[ ii ];
        }
      }
      lines.clear();
    }
    str_stream << "}\n";
  }
  
  if ( _verbose ) {
    std::cout << "-----------------------------------------------\n";
    std::cout << std::endl;
  }
  
  double tot_time;
  if ( _timer ) tot_time = get_timer ();
  
  string out_str = str_stream.str();
  /// Print the resulting string
  if ( _out_file.compare ( "" ) ) {
    std::ofstream ofs ( _out_file, std::ofstream::out );
    if ( ofs.is_open() ) {
      ofs << out_str;
      ofs.close();
    }
    else {
      cout << "Can't open file: " << _out_file << endl;
    }
  }
  else {
    cout << out_str;
  }
  
  if ( _timer ) {
    std::cout << _dbg << "Time for visiting the RB-Tree: ";
    std::cout << tot_time << " sec.\n";
  }
}//print
