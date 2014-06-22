#include "token.h"

using namespace std;

int Token::_glb_id = 0;

Token::Token () :
_token   ( "" ),
_counter ( 0 ) {
  _token_id = _glb_id++;
}//-

Token::Token ( string str ) :
_token   ( str ),
_counter ( 1 ) {
  _token_id = _glb_id++;
}//-

Token::Token ( string str, int line, int counter ) :
_token   ( str ),
_counter ( counter ) {
  _idx_line.push_back( line );
  _token_id = _glb_id++;
}//-

Token::Token ( string str, vector <int> lines, int counter ) :
_token   ( str ),
_counter ( counter ) {
  _idx_line = lines;
  _token_id = _glb_id++;
}//-

Token::Token ( const Token& other ) {
  _token    = other._token;
  _idx_line = other._idx_line;
  _counter  = other._counter;
  _token_id = other._token_id;
}//-

Token::~Token () {
  _idx_line.clear();
}//-

Token&
Token::operator= ( const Token& other ) {
  if( this != &other ) {
    _token    = other._token;
    _idx_line = other._idx_line;
    _counter  = other._counter;
    _token_id = other._token_id;
  }
  return *this;
}//=

bool
Token::equal ( const Content* other_content ) const {
  if ( typeid ( *this ) == typeid ( *other_content ) ) {
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(other_content) );
    return ( _token.compare( casted_token->get_string() ) == 0 );
  }
  return false;
}//==


bool
Token::less ( const Content* other_content ) const {
  if ( typeid ( *this ) == typeid ( *other_content ) ) {
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(other_content) );
    return ( _token < (casted_token->get_string()) );
  }
  return false;
}//<

string
Token::get_string () const {
  return _token;
}//get_string

vector<int>
Token::get_lines () const {
  return _idx_line;
}//get_lines

int
Token::get_counter () const {
  return _counter;
}//get_counter

int
Token::get_glb_counter () const {
  return _glb_id;
}//get_glb_counter

int
Token::get_id () const {
  return _token_id;
}//get_glb_counter

void
Token::set_lines ( const vector<int>& new_lines ) {
  _idx_line = new_lines;
}//set_lines

void
Token::set_counter ( int new_counter ) {
  _counter = new_counter;
}//set_counter

bool
Token::match ( const Content * other_content ) const {
  if ( typeid ( *this ) == typeid ( *other_content ) ) {
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(other_content) );
    if ( _token.at( 0 ) == (casted_token->get_string ()).at( 0 ) ) {
      return true;
    }
  }
  return false;
}//match


void
Token::how_match ( Content * other_content, vector< Content * >& matches ) const {
  if ( typeid ( *this ) == typeid ( *other_content ) ) {
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(other_content) );
    string other_str = casted_token->get_string();
    if ( _token.compare( other_str ) == 0  ) {
      matches.clear();
      return;
    }
    /// Find the common substring
    string shorter_str = ( _token.length() < other_str.length() ) ? _token : other_str;
    
    int i = 0;
    bool differ = false;
    for ( ; i < shorter_str.length(); i++ ) {
      if ( _token.at( i ) != other_str.at( i ) ) {
        differ = true;
        break;
      }
    }
    if ( !differ ) { i++; }
    string replacement_str = shorter_str.substr( 0, i );
    
    /// Here _token is a prefix of the other string
    if ( (replacement_str.compare( _token ) == 0) &&
         (_token.length() < other_str.length()) ) {
      ///@note: modifies casted content not the current one!
      casted_token->overwrite_content ( other_str.substr( _token.length(),
                                                          other_str.length() -
                                                          _token.length() ) );
      matches.push_back( casted_token );
      return;
    }
    /// Here other_str is a prefix of _token
    if ( (replacement_str.compare( other_str ) == 0) &&
         (other_str.length() < _token.length()) ) {
      
      /*
       * Need to modify the current content:
       * create a new one.
       */
      Token * split_token = new Token ( _token.substr( 0, other_str.length()), casted_token->get_lines(), 1  );
      matches.push_back( split_token );
      
      /// It is possible to modify directly the other content
      casted_token->overwrite_content ( _token.substr( other_str.length() ) );
      casted_token->set_lines   ( _idx_line );
      casted_token->set_counter ( _counter  );
      matches.push_back( casted_token );
      return;
    }
    
    /// New content for modifying the existing one
    Token * split_token = new Token ( replacement_str );
    matches.push_back( split_token );
    /// New content for its children
    Token * rest_of_token = new Token ( _token.substr ( i ), _idx_line, _counter );
    matches.push_back( rest_of_token );
    /// Update content
    string split_other  = other_str.substr( i );
    casted_token->overwrite_content ( split_other );
    matches.push_back( casted_token );
  }//==
}//how_match

size_t
Token::get_size () const {
  return _token.length();
}//get_size

const
void *
Token::get_ref_base_obj () const {
  return ((void*) &_token);
}//get_ref_base_obj

void
Token::update_content ( Content * other_content ) {
  if ( typeid ( *this ) == typeid ( *other_content ) ) {
    Token * casted_token = static_cast< Token * > ( const_cast<Content *>(other_content) );
    /// If the two strings differ: substitute the old one and return
    if ( _token.compare( casted_token->get_string() ) != 0 ) {
      overwrite_content( casted_token->get_string() );
      
      _idx_line.clear();
      _idx_line = casted_token->get_lines();
      _counter = 1;

      
      return;
    }
    /// Otherwise found a match: add other lines and increase counter
    for ( auto line: (casted_token->get_lines ()) ) {
      _idx_line.push_back ( line );
    }
    _counter++;
  }
  
}//update_content


void
Token::overwrite_content ( string str ) {
  _token = str;
}

void
Token::print() const {
  cout << "TOKEN_" << _token_id << ":\n";
  cout << "\t String:  \"" << _token   << "\"\n";
  cout << "\t Counter: " << _counter << "\n";
  if ( _idx_line.size() > 0 ) {
    cout << "\t Indeces: ";
    for ( int i = 0; i < _idx_line.size() - 1; i++ )
      cout << _idx_line[ i ] << ", ";
    cout << _idx_line[ _idx_line.size() - 1 ] << "\n";
  }
}//print

void
Token::print_info() const {
  cout << "{";
  cout << _counter;
  cout << ":";
  if ( _idx_line.size() > 0 ) {
    for ( int i = 0; i < _idx_line.size() - 1; i++ )
      cout << _idx_line[ i ] << ",";
    cout << _idx_line[ _idx_line.size() - 1 ];
  }
  else {
    cout << "0";
  }
  cout << "}";
  cout << endl;
}//print_info

string
Token::get_info() const  {
  stringstream s;
  s << "{";
  s << _counter;
  s << ":";
  if ( _idx_line.size() > 0 ) {
    for ( int i = 0; i < _idx_line.size() - 1; i++ )
      s << _idx_line[ i ] << ",";
    s << _idx_line[ _idx_line.size() - 1 ];
  }
  else {
    s << "0";
  }
  s << "}";
  s << "\n";
  
  return s.str();
}//get_info


