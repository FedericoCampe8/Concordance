#include "concordance_visitor.h"
#include "content.h"
#include "token.h"

//#define CONCORDANCE_VISITOR_DBG

using namespace std;

ConcordanceVisitor::ConcordanceVisitor ( std::string outf ) :
_str_to_print  ( "" ),
_current_token ( "" )
{
  _out_file = outf;
}//-

ConcordanceVisitor::~ConcordanceVisitor () {
  while ( !_trail_stack.empty() ) {
    _trail_stack.pop();
  }
}//-

void
ConcordanceVisitor::visit() {
}//visit

void
ConcordanceVisitor::visitNode ( Node * node ) {
  int level = node->get_level();
  string current_string;
  if ( _trail_stack.size() == 0 ) {
    _trail_stack.push( make_pair ( (node->get_content())->get_size(), level ));
  }
  else {
    if ( level > ( _trail_stack.top()).second ) {
      current_string = *((string *) (node->get_content())->get_ref_base_obj());
      _current_token += current_string;
      _trail_stack.push( make_pair ( (node->get_content())->get_size(), level ));
    }
    else if ( level == ( _trail_stack.top()).second ) {
      while ( _trail_stack.top().second == level ) {
        _current_token = _current_token.substr( 0, _current_token.length() - _trail_stack.top().first );
        _trail_stack.pop();
      }
      current_string = *((string *) (node->get_content())->get_ref_base_obj());
      _current_token += current_string;
      _trail_stack.push( make_pair ( (node->get_content())->get_size(), level ));
    }
    else {
      while ( _trail_stack.top().second >= level ) {
        _current_token = _current_token.substr( 0, _current_token.length() - _trail_stack.top().first );
        _trail_stack.pop();
      }
      current_string = *((string *) (node->get_content())->get_ref_base_obj());
      _current_token += current_string;
      _trail_stack.push( make_pair ( (node->get_content())->get_size(), level ));
    }
  }
  
  /// Output information as desired for concordance
  if ( node->is_final() ) {
#ifdef CONCORDANCE_VISITOR_DBG
    node->print();
#endif
    if ( _out_file.compare ( "" ) == 0 ) {
      cout << _current_token;
      cout << setw( 25 - (_current_token.length()) );
      (node->get_content())->print_info();
    }
    else {
      stringstream s;
      s << _current_token;
      for ( int i = 0; i < (25 - (_current_token.length())); i++ ) s << " ";
      _str_to_print += s.str();
      _str_to_print += (node->get_content())->get_info();
    }
  }
}//visitNode

void
ConcordanceVisitor::print_to_file () {
  if ( _out_file.compare ( "" ) == 0 ) return;
  
  std::ofstream ofs ( _out_file, std::ofstream::out );
  if ( ofs.is_open() ) {
    ofs << _str_to_print;
    ofs.close();
  }
  else {
    cout << "Can't open file: " << _out_file << endl;
  }
}//print_to_file



